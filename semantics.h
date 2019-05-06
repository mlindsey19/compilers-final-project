//
// Created by mlind on 4/30/2019.
//

#ifndef PROJECT_3_SEMANTICS_H
#define PROJECT_3_SEMANTICS_H

#include "token.h"
#include "node.h"

typedef struct Stack{
    int scope;
    int pos;
    //  struct Stack * next;
 //  struct Stack * top;
    Token * tk;
    struct Stack * previous;
}Stack;

//functions
void startStack();
void popBlock();
void popGlobals();
void push( Token * );
void increaseScope();
void decreaseScope();
int checkRedefined(Token * );
int checkUndefined(Token *tk);
int getVarNum();
void readx( int );
void loadx( int );
void loadi( char * );
void stackw( int );
void stackr( int );
void storex(int);
void writex(int);
void outOf(int , LinkToken * , int);
void outMark(int );
void inMark(int );
int getOutNum();
int getInNum();
void programStop();
void addx( int);
void subx( int);
void divx( int);
void multx( int);
void multNeg( );


#endif //PROJECT_3_SEMANTICS_H
