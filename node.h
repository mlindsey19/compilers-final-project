//
// Created by mlind on 4/29/2019.
//

#ifndef PARSER_NODE_H
#define PARSER_NODE_H

#include "token.h"

#define NT_sz 16
typedef struct LinkToken{
    Token token;
    struct LinkToken * link;
}LinkToken;
typedef struct Node{

    struct LinkToken * linkToken;

    struct Node * child_0;
    struct Node * child_1;
    struct Node * child_2;
    struct Node * child_3;

    char nonTerm[ NT_sz ];

} Node;



#endif //PARSER_NODE_H