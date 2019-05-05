//
// Created by mlind on 4/30/2019.
//

#ifndef PROJECT_3_SEMANTICS_H
#define PROJECT_3_SEMANTICS_H

#include "token.h"

typedef struct Stack{
    int scope;
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
void checkRedefined(Token * );
void checkUndefined(Token *tk);
#endif //PROJECT_3_SEMANTICS_H
