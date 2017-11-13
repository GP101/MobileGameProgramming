/////////////////////////////////////////////////////////////////////
// File: kogGen.h: Helpful macros
// Desc: KOG general helpful macros
//       edited by seojt@kogsoft.com
//       modified on 2 April, 2001
//       last modified on 20 April, 2001
//
#if !defined(_kogGen_Defined_)
#define _kogGen_Defined_

//keep a lot of unnecessary code out of our project builds
#define WIN32_LEAN_AND_MEAN 

#if !defined(_MSC_VER)
    #error kogGen.h needs Visual C++
//#else
//    #pragma message("Expanding \'kogGen.h\'")
#endif // !defined(_MSC_VER)

#pragma once

#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#include <assert.h>

// safe delete macros
/*
#ifndef SAFE_DELETE
#define SAFE_DELETE(p) if (p) { delete (p); (p)=NULL;}
#define SAFE_DELETEA(p) if (p) { delete[] (p); (p)=NULL; }
#endif // #ifndef SAFE_DELETE
/**/

// user break point macro
#if defined(_DEBUG)
    //#define BREAK(cond)       if (cond) _asm int 3
    #define BREAK(cond)         if (cond) DebugBreak()
    #define ASSERT(exp)         assert(exp)
    #define ASSERT_VALID(exp)    assert(exp != NULL)
    #define ASSERT_MSG(str)     assert( str == NULL)
#else
    #define BREAK(cond)
    #define ASSERT(exp)
    #define ASSERT_VALID(exp)
    #define ASSERT_MSG(str)
#endif // defined(_DEBUG)
/*
    usage:

    void main()
    {
        int n;

        scanf("%d", &n);
    
        // user break point
        // you must run this program using F5 in debug mode
        BREAK(n==0);
    
        printf("%d\n", 100/n);
    }
*/

// local function
#define BEGIN_FUNCTION(name)    struct name {
#define END_FUNCTION(name)      };
/*
    usage:

    void Test()
    {
        BEGIN_FUNCTION(Test)

            static long Factorial(long n)
            {
                if (n==0) 
                    return 1L;
                else
                    return n*Factorial(n-1);
            }

        END_FUNCTION(Test)

        long l;

        l = Test::Factorial(4);
        printf("Factorial 4=%ld\n", l);
    }
*/

#ifndef OUT 
#define OUT
#endif

#ifndef IN
#define IN
#endif
#endif // !defined(_kogGen_Defined_)
