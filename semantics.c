//
// Created by mlind on 4/30/2019.
//

#include <stdlib.h>
#include <string.h>
#include "semantics.h"
#include "token.h"
#include "node.h"
#include <stdio.h>
#define STR 16
//extern output file
extern FILE * output;


//vars
static int scope;
static Stack * stack;
static int topOfStack;
static int outStmtNum;
static int inStmtNum;
static int varsNum;


//static functions
static Stack * find( Token * tk, Stack * st, int d ){
    if( st ){
        if( !strcmp( st->tk->instance , tk->instance) )
            return st;
        else
            return find( tk, st->previous, d++ );
    }
    return NULL;
}
static void pop(){
    Stack * temp;
    temp = stack;
    stack = stack->previous;
    topOfStack--;
    varsNum--;
    if (scope > 0)
        fprintf(output, "POP\n");
    free(temp);
}
static void scopeError(Token * tk, char * expected){
    printf("ERROR line: %i -> %s - Scope: %s\n", tk->lineNumber, tk->instance,  expected);
    exit(-17);
}
static void toOutput(char * str, int i, char * num){
    char buf[STR];
    memset(buf,0,STR);
    if( i < 0 )
        sprintf(buf, "%s%s\n", str, num );
    else {
        char X[4];
        memset(X, 0, 8);
        sprintf(X, "X%i", i);
        sprintf(buf, "%s%s\n", str, X);
    }
    fprintf(output, buf);
}

static int isInstance(char * a, char *b ){
    if ( strcmp( a, b ) == 0 )
        return 1;
    return 0;
}

// *********************************************

void startStack(){
    stack = NULL;
    topOfStack = 0;
    scope = 0;
    outStmtNum = 1;
    inStmtNum = 1;
    varsNum = 0;
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
    if ( stack ){
        int temp = varsNum;
        while( temp-- )
            toOutput("", temp, NULL);
        popBlock();
    }

}
void push( Token * tk ){
    Stack * newStackNode = ( Stack * ) malloc( sizeof( Stack ) );
    newStackNode->tk = tk;
    newStackNode->scope = scope;
    newStackNode->previous = stack;
    stack = newStackNode;
    fprintf(output,"PUSH\n");
    newStackNode->pos = topOfStack++;

}
void increaseScope(){
    scope++;
}
void decreaseScope(){
    scope--;
}
int checkRedefined(Token * tk){
    Stack * temp;
    temp = find( tk, stack, 0);
    if( temp  && temp->scope == scope)
        scopeError(tk, "Redefined");

    //  return temp->pos;
}
int checkUndefined(Token *tk){
    Stack * temp = find( tk, stack, 0);
    if ( !temp)
        scopeError( tk, "Undefined");
    return temp->pos;
}
int getVarNum(){
    return varsNum++;
}
void readx(int local){

}
void loadx(int local){
    char * str = "LOAD ";
    toOutput(str, local, NULL);
}
void loadi(char * num){
    char * str = "LOAD ";
    toOutput(str, -1, num);
}
void stackw(int stackPos){
    char * str = "STACKW ";
    char num[STR];
    memset(num,0,STR);
    sprintf(num,"%i",stackPos);
    toOutput(str, -1, num);

}

void stackr(int stackPos) {
    char * str = "STACKR ";
    char num[STR];
    memset(num,0,STR);
    sprintf(num,"%i",stackPos);
    toOutput(str, -1, num);
}
void storex( int local ){
    char * str = "STORE ";
    toOutput(str, local, NULL);
}
void writex( int local ){
    char * str = "WRITE ";
    toOutput(str, local, NULL);
}
void inMark(int inNum){
    char in[STR];
    memset(in, 0, STR);
    sprintf(in, "out%i ", inNum);
    //break to if false
    toOutput(in,-1, ":");
}
int getOutNum(){
    return outStmtNum++;
}
int getInNum(){
    return inStmtNum++;
}
void outOf(int local, LinkToken * link, int outNum) {
    char out[STR];
    memset(out, 0, STR);
    sprintf(out, "out%i", outNum);

    //conditions
    char *SUB = "SUB ";
    toOutput(SUB, local, NULL);

    char *BRNEG = "BRNEG ";
    char *BRPOS = "BRPOS ";
    char *BRZNEG = "BRZNEG ";
    char *BRZPOS = "BRZPOS ";
    if (isInstance(link->token.instance, toString(EQUAL_tk))) {
        if (link->link) {
            if (isInstance(link->link->token.instance, toString(EQUAL_tk))) {
                toOutput(BRNEG, -1, out);
                toOutput(BRPOS, -1, out);
            } else if (isInstance(link->link->token.instance, toString(LESS_THAN_tk))) {
                toOutput(BRPOS, -1, out);
            } else if (isInstance(link->link->token.instance, toString(GREATER_THAN_tk))) {
                toOutput(BRNEG, -1, out);
            }
        }

    } else if (isInstance(link->link->token.instance, toString( GREATER_THAN_tk ))){
        toOutput(BRZPOS, -1, out);
    }
    else if (isInstance(link->link->token.instance ,toString( LESS_THAN_tk ))){
        toOutput(BRZNEG, -1, out);
    }

    //statement if cond is true




}
void outMark(int outNum) {
    char out[STR];
    memset(out, 0, STR);
    sprintf(out, "out%i", outNum);
    //break to if false
    toOutput(out,-1, ":");
}
void programStop(){
    toOutput("STOP", -1, "");
}
void addx( int local ){
    char * ADD = "ADD ";
    toOutput(ADD, local, NULL);
}
void subx( int local ){
    char * SUB = "SUB ";
    toOutput(SUB, local, NULL);
}
void divx( int local ){
    char * DIV = "DIV ";
    toOutput(DIV, local, NULL);
}
void multx( int local ) {
    char *MULT = "MULT ";
    toOutput(MULT, local, NULL);
}
void multNeg(  ) {
    char *MULT = "MULT ";
    toOutput(MULT, -1, "-1");
}