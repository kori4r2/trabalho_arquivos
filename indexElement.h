#ifndef _INDEX_ELEMENT_H_
#define _INDEX_ELEMENT_H_

#include <stdio.h>
#include <stdlib.h>

#define TAMANHO_IDXELEMENT (sizeof(unsigned int) + sizeof(long int))

typedef unsigned char bool;
#define true 1;
#define false 0;

typedef struct idxElement INDEXELEMENT;

INDEXELEMENT *createIdxElement(unsigned int, long int);
INDEXELEMENT *readIdxElement(FILE*);
void writeIdxElement(INDEXELEMENT*, FILE*);
int checkElement(INDEXELEMENT*, unsigned int);
unsigned int getId(INDEXELEMENT*);
long int getOffset(INDEXELEMENT*);
void deleteIdxElement(INDEXELEMENT**);

#endif
