// Alisson Mateus de Oliveira Magalhães - nUSP 8066287
// Ítalo Tobler Silva - nUSP 8551910

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
void insereFilme(CATALOGO*, FILME**);
void imprimeCatalogo(CATALOGO*);
void procuraFilme(CATALOGO*, unsigned int);

#endif
