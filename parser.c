//
// Created by mlind on 4/21/2019.
//

#include "parser.h"
#include "node.h"
#include "scanner.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "semantics.h"

static Node * program();
static Node * vars();
static Node * block();
static Node * stats();
static Node * stat();
static Node * mStat();
static Node * in();
static Node * out();
static Node * iff();
static Node * assign();
static Node * loop();
static Node * expr();
static Node * a();
static Node * k();
static Node * n();
static Node * m();
static Node * r();
static Node * ro();
static LinkToken * createTokenNode();
static int isNotKeyWd( char * check );


static int isInstance( char *, char * );
Token tk;
static void parseError( char * );

Node * parser( Node * node ){
    tk = scanner();
    node->child_0 = program();
    return node;
}


static Node * program(){
    startStack();

    Node * node = createNode( toString( program ) );

    node->child_0 = vars();
    node->child_1 = block();

    //pop global
    popGlobals();

    if ( tk.id == EOFtk )
        printf( "Program ok\n" );
    else
        parseError( toString( EOFtk ) );
    return node;
}
static Node * vars() {
    Node * node = createNode( toString( vars ) );
    if ( isInstance( tk.instance, toString( INT_tk ) ) ) {
        tk = scanner();
        if ( tk.id == IDENTtk ) {
            node->linkToken = createTokenNode();

            //check if valid
            checkRedefined( &tk );
            //push ident to stack
            push( &(node->linkToken->token) );

            tk = scanner();
            if ( tk.id == NUMtk ) {
                node->linkToken->link = createTokenNode();
                tk = scanner();
                node->child_0 = vars();
                return node;
            }else
                parseError( toString( NUMtk ) );
        }else
            parseError( toString( IDENTtk ) );
    }
    return NULL;

}
static Node * block(){
    Node * node = createNode( toString( block ) );

    if ( isInstance( tk.instance, toString( Begin_tk ) ) ) {
        tk = scanner();
    }else
        parseError( toString( Begin_tk ) );

    //local scope
    increaseScope();

    node->child_0 = vars();
    node->child_1 =  stats();

    popBlock();
    decreaseScope();

    if ( isInstance( tk.instance, toString( End_tk ) ) ) {
        tk = scanner();
    }else
        parseError( toString( End_tk ) );
    return node;
}
static Node * stats(){
    Node * node = createNode(toString(stats));

    node->child_0 = stat();
    if (isInstance(tk.instance, toString( COLON_tk ) ) ) {
        tk = scanner();
    }else
        parseError( toString( COLON_tk ) );
    node->child_1 = mStat();
    return node;
}
static Node * stat() {
    Node * node = createNode(toString(stat));

    if ( isInstance( tk.instance, toString( Read_tk ) ) ) {
        node->child_0 =  in();
    } else if ( isInstance( tk.instance, toString( Output_tk ) ) ) {
        node->child_0 = out();
    } else if ( isInstance( tk.instance, toString( IFF_tk ) ) )  {
        node->child_0 = iff();
    } else if ( isInstance( tk.instance, toString( Loop_tk ) ) ) {
        node->child_0 = loop();
    } else if(isInstance( tk.instance, toString( Begin_tk ) ) ){
        node->child_0 = block();
    } else if ( tk.id == IDENTtk && isNotKeyWd(tk.instance)){
        node->child_0 =  assign();
    }else
        parseError( toString( Statment ) );
    return  node;
}
static Node * mStat(){
    Node * node = createNode(toString(mStat));
    if( isInstance(tk.instance, toString(End_tk)))
        return NULL;
    if (   tk.id == IDENTtk ){
        node->child_0 = stat();
        if (isInstance(tk.instance, toString( COLON_tk ) ) ) {
            tk = scanner();
        }else
            parseError( toString( COLON_tk ) );
        node->child_1 = mStat();
        return node;
    }

    return NULL;
}
static Node * out(){
    Node * node = createNode( toString( out ) );
    if (isInstance(tk.instance, toString(Output_tk ) ) ) {
        tk = scanner();
        if (isInstance(tk.instance, toString(OPEN_BRACKET_tk))) {
            tk = scanner();
            node->child_0 = expr();
            if (isInstance(tk.instance, toString(CLOSE_BRACKET_tk))) {
                tk = scanner();
            } else
                parseError(toString(CLOSE_BRACKET_tk));
        } else
            parseError(toString(OPEN_BRACKET_tk));
    } else{
        parseError(toString(Output_tk) );
    }

    return node;
}
static Node * in(){
    Node * node = createNode( toString( in ) );

    if ( isInstance(tk.instance, toString( Read_tk ) )) {
        tk = scanner();
        if (isInstance(tk.instance, toString(OPEN_BRACKET_tk))) {
            tk = scanner();
            if ( tk.id == IDENTtk && isNotKeyWd( tk.instance ) ) {
                node->linkToken= createTokenNode();

                checkUndefined(&tk);

                tk = scanner();
                if (isInstance(tk.instance, toString(CLOSE_BRACKET_tk))) {
                    tk = scanner();
                } else
                    parseError(toString(CLOSE_BRACKET_tk));
            } else
                parseError(toString(IDENTtk));
        } else
            parseError(toString(OPEN_BRACKET_tk));
    }else
        parseError(toString(Read_tk) );
    return node;
}
static Node * iff(){
    Node * node = createNode(toString(iff));

    if ( isInstance( tk.instance, toString( IFF_tk ) ) ){
        tk= scanner();
        if( isInstance( tk.instance, toString( OPEN_BRACKET_tk ) ) ){
            tk = scanner();
            node->child_0 = expr();
            node->child_1 = ro();
            node->child_2 = expr();
            if (isInstance( tk.instance, toString( CLOSE_BRACKET_tk ) ) ){
                tk = scanner();
                node->child_3 = stat();
            } else
                parseError( toString( CLOSE_BRACKET_tk ) );
        } else
            parseError( toString( OPEN_BRACKET_tk ) );
    } else
        parseError( toString( IFF_tk) );
    return node;
}
static Node * assign(){
    Node * node = createNode(toString(assign));

    if( tk.id == IDENTtk  && isNotKeyWd( tk.instance )){
        node->linkToken = createTokenNode();
        checkUndefined(&tk);

        tk = scanner();
        if ( isInstance( tk.instance, toString( EQUAL_tk ) ) ) {
            tk = scanner();
            node->child_0 = expr();
        } else parseError( toString( EQUAL_tk) );
    } else parseError( toString( IDENTtk) );
    return node;
}
static Node * loop(){
    Node * node = createNode(toString(loop));

    if ( isInstance( tk.instance, toString( Loop_tk ) ) ){
        tk = scanner();
        if( isInstance( tk.instance, toString( OPEN_BRACKET_tk ) ) ){
            tk = scanner();
            node->child_0 = expr();
            node->child_1 = ro();
            node->child_2 = expr();
            if (isInstance( tk.instance, toString( CLOSE_BRACKET_tk ) ) ){
                tk = scanner();
                node->child_3 = stat();
            } else
                parseError( toString( CLOSE_BRACKET_tk ) );
        } else
            parseError( toString( OPEN_BRACKET_tk ) );
    } else
        parseError( toString( Loop_tk) );
    return node;
}
static Node * expr(){
    Node * node = createNode(toString(expr));
    node->child_0 =  a();
    node->child_1 =  k();
    return node;
}
static Node * k(){
    Node * node = createNode(toString(k));

    if ( isInstance(tk.instance, toString(MINUS_tk))
         || isInstance( tk.instance, toString( PLUS_tk ) ) ){
        node->linkToken = createTokenNode();
        tk = scanner();
        node->child_0 = expr();
        return node;

    }
    return NULL;
}
static Node * a(){
    Node * node = createNode(toString(a));

    node->child_0 = n();
    if ( isInstance( tk.instance, toString( ASTERISK_tk ) ) ){
        node->linkToken = createTokenNode();
        tk = scanner();
        node->child_1 = a();
    }
    return node;
}
static Node * n(){
    Node * node = createNode(toString(n));

    node->child_0 = m();
    if (isInstance( tk.instance, toString( SLASH_tk ))){
        node->linkToken =createTokenNode();
        tk = scanner();
        node->child_1 = n();
    }
    return node;
}
static Node * m(){
    Node * node = createNode(toString(m));

    if (isInstance( tk.instance, toString( MINUS_tk ))){
        node->linkToken = createTokenNode();
        tk=scanner();
        node->child_0 = m();
    } else
        node->child_0 = r();
    return node;
}
static Node * r(){
    Node * node = createNode(toString(r));

    if ( isInstance( tk.instance, toString( OPEN_BRACKET_tk ))){
        tk = scanner();
        node->child_0 = expr();
        if ( isInstance( tk.instance, toString( CLOSE_BRACKET_tk ))){
            tk = scanner();
        } else
            parseError( toString( OPEN_BRACKET_tk) );
    } else if( tk.id == NUMtk || ( tk.id == IDENTtk && isNotKeyWd( tk.instance ) ) ) {
        node->linkToken = createTokenNode();
       if(tk.id == IDENTtk)
           checkUndefined(&tk);
        tk = scanner();
    }else
        parseError( "expr, identifier or integer" );
    return node;
}
static Node * ro(){
    Node * node = createNode(toString(ro));
    node->linkToken = createTokenNode( tk );
    if (isInstance( tk.instance, toString( LESS_THAN_tk ) ) ||
        isInstance( tk.instance, toString( GREATER_THAN_tk ) ) ){
        tk =scanner();
        return node;
    }else if (isInstance( tk.instance, toString( EQUAL_tk ) ) ){
        tk = scanner();
        node->linkToken->link = createTokenNode( tk );
        if (isInstance( tk.instance, toString( EQUAL_tk ) ) ||
            isInstance( tk.instance, toString( LESS_THAN_tk ) ) ||
            isInstance( tk.instance, toString( GREATER_THAN_tk ) ) ){
            tk = scanner();
            return node;
        }
    } else
        parseError("cond operator");
    return NULL;
}


Node * createNode( char * nonTerm ){
    Node * newNode = ( Node * ) malloc( sizeof( Node ) );
    memset( newNode->nonTerm, 0, NT_sz );
    strcpy( newNode->nonTerm, nonTerm );
    newNode->child_0 = NULL;
    newNode->child_1 = NULL;
    newNode->child_2 = NULL;
    newNode->child_3 = NULL;
    newNode->linkToken = NULL;
    return newNode;
}
static LinkToken * createTokenNode(){
    LinkToken * newLink = ( LinkToken * ) malloc( sizeof( LinkToken ) );
    newLink->link = NULL;
    newLink->token = tk;
//    memset( newLink->token.instance, 0, NT_sz );
//    strcpy( newLink->token.instance, tk.instance );
//    newLink->token.lineNumber = tok.lineNumber;
//    newLink->token.id   = tk.id;
    return newLink;
}
static int isInstance(char * a, char *b ){
    if ( strcmp( a, b ) == 0 )
        return 1;
    return 0;
}
static void parseError( char * expected ){
    printf("ERROR line: %i -> %s - Expected %s\n", tk.lineNumber, tk.instance,  expected);
    exit(-15);
}
extern const char keywords[][8];
static int isNotKeyWd( char * check ){
    int i;
    int x = strlen(check);
    char temp[32];
    strcpy(temp,check);
    temp[ x - 3] = 0;
    for (i = 0 ; i < 12; i++){
        if ( strcmp( temp, keywords[ i ] ) == 0 )
            return 0;
    }
    return 1; //true
}