//
// Created by mlind on 4/30/2019.
//

#include <stdlib.h>
#include <string.h>
#include "semantics.h"
#include "token.h"
#include <stdio.h>

//vars
static int scope;
static Stack * stack;


//static functions
static Stack * find( Token * tk, Stack * st ){
    if( st ){
        if( !strcmp( st->tk->instance , tk->instance) )
            return st;
        else
            return find( tk, st->previous );
    }
    return NULL;
}
static void pop(){
    Stack * temp;
    temp = stack;
    stack = stack->previous;
    free(temp);
}
static void scopeError(Token * tk, char * expected){
    printf("ERROR line: %i -> %s - Scope: %s\n", tk->lineNumber, tk->instance,  expected);
    exit(-17);
}

// ***************************

void startStack(){
    stack = NULL;
    //newStack();
}
void popBlock(){
    int isInScope = 1;
    while( isInScope ){
        if(stack && stack->scope == scope )
            pop();
        else
            isInScope = 0;
    }
}
void popGlobals(){
    if ( stack )
        popBlock();
}
void push( Token * tk ){
    Stack * newStackNode = ( Stack * ) malloc( sizeof( Stack ) );
    newStackNode->tk = tk;
    newStackNode->scope = scope;
    newStackNode->previous = stack;
    stack = newStackNode;
}
void increaseScope(){
    scope++;
}
void decreaseScope(){
    scope--;
}
void checkRedefined(Token * tk){
    Stack * temp;
    temp = find( tk, stack );
    if( temp  && temp->scope == scope){
        scopeError(tk, "Redefined");
    }
}
void checkUndefined(Token *tk){
    if ( !find( tk, stack ) )
        scopeError( tk, "Undefined");
}
