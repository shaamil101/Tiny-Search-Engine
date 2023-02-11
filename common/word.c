#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

/**************** normalizeWord ****************/
/* Converts word to lower case */
void normalizeWord(char* s)
{
  // Convering to lower case
  for (int n = 0; n < strlen(s); n++) {
    s[n] = tolower(s[n]);
  }
}