#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <stdlib.h>
#include <getopt.h>
#include "scanner.h"
#include "parser.h"
#include "testTree.h"

FILE * file;
FILE * output;

int main (int argc, char **argv){

    FILE * fp;
    char infile[32];
    char outfile[32]= "out.asm\0";;
    memset(infile,0,32);
    char *ext = ".input1";
    char *extout = ".asm";

    if (argc == 1){
        fp = stdin;
        strcpy(infile, toString(stdin));
    }
    else {
        strcpy(infile, argv[1]);
        char *pos = strstr(infile, ext);

        if (pos == NULL) {}
        strcat(infile, ext);
        strcat(infile, "\0");
        fp = fopen(infile, "r");

        pos = strlen(infile) - strlen(ext);
        memset(outfile,0,32);
        strncpy(outfile, infile, pos);
        strcat(outfile,extout);
        strcat(infile, "\0");
    }
    output = fopen(outfile, "w");

    file = fp;
    Node * root = createNode( infile );
    parser(root);
   // traverseTree(root);
    fclose(fp);
    fclose(output);


    return 0;
}