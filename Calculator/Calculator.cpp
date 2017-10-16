#include <stdio.h>
#include <conio.h>
#include <ctype.h>

enum EState
{
    OP1,
    OP2,
    RESULT
};

EState s = OP1;
int op1 = 0;
int op2 = 0;
int res = 0;
int op = 0;

bool IsOperator(int ch)
{
    return (ch == '+' || ch == '-' || ch == '*' || ch == '/');
}

int Evaluator( int op1, int op, int op2 )
{
    int result = 0;
    if (op == '+')
        result = op1 + op2;
    else if (op == '-')
        result = op1 - op2;
    else if (op == '*')
        result = op1 * op2;
    else if (op == '/')
        result = op1 / op2;
    return result;
}

void main()
{
    int ch = 0;
    while (ch != 27) {
        ch = _getch();
        if (s == OP1) {
            if ( isdigit(ch) ) {
                int number = ch - '0'; // "convert ch to number";
                op1 = op1 * 10 + number;
            }
            else if ( IsOperator(ch) ) {
                op = ch;
                op2 = 0;
                s = OP2; // state transition
            }
        }
        else if (s == OP2) {
            if (isdigit(ch)) {
                int number = ch - '0';
                op2 = op2 * 10 + number;
            }
            else if (IsOperator(ch)) {
                op1 = Evaluator(op1, op, op2);
                op = ch;
                op2 = 0;
            }
            else if (ch == '=') {
                res = Evaluator(op1, op, op2);
                s = RESULT;
            }
        }
        else if (s == RESULT) {
            if (isdigit(ch)) {
                op1 = 0;
                s = OP1;
            }
            else if (IsOperator(ch)) {
                op1 = res;
                op2 = 0;
                s = OP2;
            }
        }
        printf("%i, %c, %i\r\n", op1, op, op2);
    }
}