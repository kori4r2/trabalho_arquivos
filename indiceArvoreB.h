#ifndef _INDICE_B_H_
#define _INDICE_B_H_

#define ORDEM_PAGINA 7

#include <stdio.h>
#include <stdlib.h>
#include "indexElement.h"

typedef struct arvoreB ARVOREB;
typedef struct paginaB PAGINAB;


ARVOREB *criaArvoreB(const char*);
void abreIndice(ARVOREB*);
void fechaIndice(ARVOREB*);
long int getRaiz(ARVOREB*);
void setRaiz(ARVOREB*, long int);

PAGINAB *criaPaginaB();
void lePaginaB(FILE*, PAGINAB*);
void escrevePaginaB(FILE*, PAGINAB*);
void apagaPaginaB(PAGINAB**);

#endif
