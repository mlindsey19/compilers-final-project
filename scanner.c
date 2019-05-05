//
// Created by mlind on 3/5/2019.
//

#include "scanner.h"
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <ctype.h>

static void finalState();
static Token newToken( enum TokenID );
static void getNextCharacter( FILE *, int );
static void digitState();
static void letterState();
static void operatorState();
static void setInstance( enum TokenID );
static void appendToInstance();
static void commentState( FILE *);
typedef struct { char this, next; } Character;
typedef struct { int  line, tk; } Count;


char instance[ 32 ];
Character character = { SPACE, SPACE };
static enum CharacterRank getRank( char );
enum TokenID currentTkID;
char  keywords[][8] = {"End", "INT", "IFF", "Let", "Loop", "Read", "Void", "Then", "Begin", "Output", "Return", "Program" };
Count count;
static int stop;

static Token token;

extern FILE * file;

Token scanner( ){
    FILE * stream = file;
    if (ftell(stream) == 0) {
        count.line = 1;
        count.tk = 0;
    }
    stop = 1;

    while(stop) {
        if (!stop) break;
        getNextCharacter( stream , 1 );
        switch ( getRank( character.this ) ){
            case digit:
                appendToInstance();
                digitState();
                break;
            case letter:
                appendToInstance();
                letterState();
                break;
            case operator:
                appendToInstance();
                operatorState();
                break;
            case endOfFile:
                finalState();
            case comment:
                commentState(stream);
            default:
                ;
        }
    }
    return token;
}
static void commentState(FILE * stream){
    int rank;
    do{
        getNextCharacter(stream, 0);
        rank = getRank(character.next);
    }while( !( rank == newLine || rank == endOfFile ) ) ;

}

static void digitState(){
    int rank = getRank(character.next);
    if ( rank == whitespace || rank == endOfFile ||
         rank == newLine || rank == comment || rank == operator )
        finalState();
}
static void letterState(){
    int rank = getRank(character.next);
    if ( rank == whitespace || rank == endOfFile || rank == newLine || rank == comment || rank == operator)
        finalState();
}

static void operatorState(){
    int rank = getRank(character.next);
    if ( rank == whitespace || rank == endOfFile || rank == digit
         || rank == letter || rank == operator || rank == newLine || rank == comment )
        finalState();
}
static void compareKw(int a, int b){
    int i;
    for (i = a ; i < b; i++){
        if ( strcmp( instance, keywords[ i ] ) == 0 )
            strcat(instance,"_tk");
    }
}
static void keywordCheck(){

    switch ( strlen(instance) ){
        case 3:
            compareKw(0,4);
            break;
        case 4:
            compareKw(4, 8);
            break;
        case 5:
            compareKw(8,9);
            break;
        case 6:
            compareKw(9,11);
            break;
        case 7:
            compareKw(11,12);
    }
}
/*********************************************
 * get next char and assign character to next
 * if charter was end of
 *********************************************/
static void getNextCharacter( FILE * stream, int isComment){
    if (ftell(stream) == 0)
        character.next = ( char ) fgetc( stream );
    character.this = character.next;
    if ( character.this != EOF )
        character.next = ( char ) fgetc( stream );

    if ( strlen(instance) == 0 && isComment){
        switch ( getRank( character.this ) ) {
            case letter:
                if (islower(character.this)) {
                    printf("ERROR: id token must start with upper case. line -> %i\n", count.line);
                    exit(-4);
                }
                currentTkID = IDENTtk;
                break;
            case operator:
                currentTkID = OPtk;
                break;
            case digit:
                currentTkID = NUMtk;
                break;
            case newLine:
                count.line++;
            case endOfFile:
                currentTkID = EOFtk;
            default: ;
        }
    }
}


/*********************************************
 * final state -
 * check for keywords add to tk array
 ********************************************/
static void finalState(){
    if ( currentTkID == IDENTtk && strlen(instance) > 2 && strlen(instance) < 8 )
        keywordCheck();
    //tokenList[ count.tk++ ] = newToken( currentTkID );
    stop = 0;
    token = newToken( currentTkID );
    //printf("%s\n",token.instance);
}

static Token newToken( enum TokenID tkid ){
    Token token;
    token.id = tkid;
    setInstance(tkid);
    memset( token.instance, 0, sizeof( token.instance ) );
    strcpy(token.instance, instance);
    token.lineNumber = count.line;
    memset( instance, 0, sizeof( instance ) );
    return token;
}
static void getOperatorString(){
    switch ( *instance ){
        case PLUS_tk:
            strcpy(instance, toString(PLUS_tk) );
            break;
        case LESS_THAN_tk:
            strcpy(instance, toString(LESS_THAN_tk) );
            break;
        case GREATER_THAN_tk:
            strcpy(instance, toString(GREATER_THAN_tk) );
            break;
        case MINUS_tk:
            strcpy(instance, toString(MINUS_tk) );
            break;
        case ASTERISK_tk:
            strcpy(instance, toString(ASTERISK_tk) );
            break;
        case SLASH_tk:
            strcpy(instance, toString(SLASH_tk) );
            break;
        case PERCENT_tk:
            strcpy(instance, toString(PERCENT_tk) );
            break;
        case DOT_tk:
            strcpy(instance, toString(DOT_tk) );
            break;
        case OPEN_BRACE_tk:
            strcpy(instance, toString(OPEN_BRACE_tk) );
            break;
        case OPEN_BRACKET_tk:
            strcpy(instance, toString(OPEN_BRACKET_tk) );
            break;
        case CLOSE_BRACE_tk:
            strcpy(instance, toString(CLOSE_BRACE_tk) );
            break;
        case OPEN_PARENTH_tk:
            strcpy(instance, toString(OPEN_PARENTH_tk) );
            break;
        case CLOSE_PARENTH_tk:
            strcpy(instance, toString(CLOSE_PARENTH_tk) );
            break;
        case CLOSE_BRACKET_tk:
            strcpy(instance, toString(CLOSE_BRACKET_tk) );
            break;
        case SEMICOLON_tk:
            strcpy(instance, toString(SEMICOLON_tk) );
            break;
        case COLON_tk:
            strcpy(instance, toString(COLON_tk) );
            break;
        case COMMA_tk:
            strcpy(instance, toString(COMMA_tk) );
            break;
        case EQUAL_tk:
            strcpy(instance, toString(EQUAL_tk) );
            break;
    }
}
static void setInstance(enum TokenID tkid){
    if (tkid == EOFtk){
        strcpy(instance, toString(EOFtk) );
        stop = 0;
    } else if (tkid == OPtk){
        getOperatorString();
    }
}
static enum CharacterRank getRank( char ch ){
    enum CharacterRank rank;

    if ( ch == 92 ){
        rank = comment;
    } else if ( ch >= 48 && ch <= 57 ){
        rank = digit;
    }else if(( ch >= 65 && ch <= 90 ) ||
             ( ch >= 97 && ch <= 122 ) ){
        rank = letter;
    } else if ( ( ch >= 58 && ch <= 62 )||
                ( ch >= 40 && ch <= 74 )||
                ch == 37 || ch == 123 ||
                ch == 125 || ch == 91 ||
                ch == 93 ){
        rank = operator;
    } else if ( ( ch >= 9 && ch <= 13 )||  //tab,newline,line tab, form feed, carriage return
                ch == 32 ) {
        rank = ch == 10 ? newLine : whitespace;
    }else if( ch == EOF ) {
        rank = endOfFile;
    }
    else
        rank = notInAlphabet;

    return rank;
}


void appendToInstance(){
    char str[2];
    str[0] = character.this;
    str[1] = ( char ) 0;
    strcat(instance, str);
}