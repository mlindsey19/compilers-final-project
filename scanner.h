//
// Created by mlind on 3/5/2019.
//

#ifndef SCANNER_SCANNER_H
#define SCANNER_SCANNER_H
#include <stdio.h>

#include "token.h"

Token scanner();
enum CharacterRank{
    digit,
    letter,
    operator,
    comment,
    notInAlphabet,
    whitespace,
    endOfFile,
    newLine
};

#endif //SCANNER_SCANNER_H