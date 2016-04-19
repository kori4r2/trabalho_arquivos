#ifndef _FILMES_H_
#define _FILMES_H_

#include <ctype.h>
#include "myStrings.h"

typedef struct filme FILME;
typedef struct catalogo CATALOGO;

FILME *criaFilme(int, int, char*, char*, char*, char*);
void apagaFilme(FILME**);
CATALOGO *criaCatalogo(char*);
void apagaCatalogo(CATALOGO**);
void inserirFilme(CATALOGO*, FILME*);
void imprimeCatalogo(CATALOGO*);
void procuraFilme(CATALOGO*, unsigned int);

#endif
