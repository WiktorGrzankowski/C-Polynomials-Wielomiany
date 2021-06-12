/** @file
        Część druga dużego zadania z IPP - kalkulator

@authors Wiktor Grzankowski <wg429211@mimuw.edu.pl>
@copyright Uniwersytet Warszawski
@date 2021
*/

#ifndef CALCDEFINITIONS_H
#define CALCDEFINITIONS_H

#define _GNU_SOURCE ///< makro potrzebne dla funkcji getline()

#include "poly.h"
#include "stack.h"

#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

#define IS_SOMETHING 73 ///< kod ASCII litery 'I'
#define ZERO 90 ///< kod ASCII litery 'Z'
#define CLONE 67 ///< kod ASCII litery 'C
#define ADD_OR_AT 65 ///< kod ASCII litery 'A'
#define MUL 77 ///< kod ASCII litery 'M'
#define NEG 78 ///< kod ASCII litery 'N'
#define SUB 83 ///< kod ASCII litery 'S"
#define DEG_SOMETHING 68 ///< kod ASCII litery 'D'
#define PRINT_OR_POP 80 ///< kod ASCII litery 'P'

#define OPENING_BRACKET 40 ///< kod ASCII znaku '('
#define CLOSING_BRACKET 41 ///< kod ASCII znaku ')'
#define PLUS 43 ///< kod ASCII znaku '+'
#define COMMA 44 ///< kod ASCII znaku ','
#define MINUS 45 ///< kod ASCII znaku '-'
#define SPACE 32 ///< kod ASCII znaku ' '

#define COMMENT 35 ///< kod ASCII znaku '#'
#define NEWLINE 10 ///< kod ASCII znaku nowej wiersza '\n'

#endif /* CALCDEFINITIONS_H */
