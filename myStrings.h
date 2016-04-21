// Alisson Mateus de Oliveira Magalhães - nUSP 8066287
// Ítalo Tobler Silva - nUSP 8551910

#ifndef _MY_STRINGS_H_
#define _MY_STRINGS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char *myStrdup(const char*);
char *getWord(FILE*);
char *readLimitedString(FILE*, int);
char *myGetLine(FILE*);

#endif
