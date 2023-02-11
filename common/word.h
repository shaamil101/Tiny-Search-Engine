/* 
 * word.h - header file for CS50 'word' module
 * 
 * Word simply provides a function to convert the to lowercase and normalize it
 * 
 * Shaamil Karim Shaw Alem, 2023
 */

#ifndef __WORD_H
#define __WORD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <unistd.h>

/**************** functions ****************/

/**************** openDirectory ****************/
/* Makes the given string all lowercase */
void normalizeWord(char* s);

#endif
