#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>
#include <unordered_set>

struct KVector
{
public:
    KVector(float x_, float y_, float z_)
    {
        x = x_;
        y = y_;
        z = z_;
    }//KVector()

    bool operator==(const KVector& rhs) const
    {
        return x == rhs.x && y == rhs.y && z == rhs.z;
    }//operator==()

    float x;
    float y;
    float z;
};

struct AStarNode
{
public:
    AStarNode(float x_, float y_, float z_)
        : location(x_,y_,z_)
    {
    }//AStarNode()

    void Print() const
    {
        printf("%g,%g,%g\r\n", location.x, location.y, location.z);
    }//Print()

    bool operator==(const AStarNode& rhs) const
    {
        return location == rhs.location;
    }//operator==()

    KVector      location;   // location of node (some location representation)
    float        cost;       // cost to get to this node
    float        heuristic;  // heuristic cost to get to goal node
    float        total;      // total cost (cost + heuristic estimate)
};//class AStarNode

class AStarNodeHash
{
public:
    size_t operator()(const AStarNode& node) const
    {
        size_t h1 = std::hash<float>()(node.location.x);
        size_t h2 = std::hash<float>()(node.location.y);
        size_t h3 = std::hash<float>()(node.location.z);
        return h1 ^ (h2 << 8) ^ (h3 << 16);
    }
};//class AStarNodeHash

int main() {
    std::unordered_set<AStarNode, AStarNodeHash> uset;
    uset.insert(AStarNode(1.f, 0.f, 0.f));
    uset.insert(AStarNode(3.f, 0.f, 0.f));
    uset.insert(AStarNode(5.f, 0.f, 0.f));

    auto sitor = uset.find(AStarNode(3.f, 0.f, 0.f));
    if (sitor != uset.end())
    {
        sitor->Print();
    }//if
}//int main()
