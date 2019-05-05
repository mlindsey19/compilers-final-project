//
// Created by mlind on 4/22/2019.
//

#include "testTree.h"
#include <stdio.h>

static int level = -1 ;


static void traverseTokenList(LinkToken * link){
    printf("  %s ",link->token.instance);
    if( link->link != NULL )
        traverseTokenList(link->link);
}

void traverseTree( Node * node ){
    level++;
    int i;
    for ( i = 0; i < level; i++)
        printf("-- ");
    printf("< %s >", node->nonTerm);
    if (node->linkToken != NULL)
        traverseTokenList(node->linkToken);
    printf("\n");

    if(node->child_0 != NULL)
        traverseTree(node->child_0);
    if(node->child_1 != NULL)
        traverseTree(node->child_1);
    if(node->child_2 != NULL)
        traverseTree(node->child_2);
    if(node->child_3 != NULL)
        traverseTree(node->child_3);


    level--;
}