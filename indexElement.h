#ifndef _INDEX_ELEMENT_H_
#define _INDEX_ELEMENT_H_

#include <stdio.h>
#include <stdlib.h>

typedef unsigned char bool;
#define true 1;
#define false 0;

typedef struct idxElement INDEXELEMENT;

INDEXELEMENT *createIdxElement(unsigned int, long int);
void readIdxElement(INDEXELEMENT*, FILE*);
void writeIdxElement(INDEXELEMENT*, FILE*);
int checkElement(INDEXELEMENT*, unsigned int);
long int getOffset(INDEXELEMENT*);
void deleteIdxElement(INDEXELEMENT**);

#endif
