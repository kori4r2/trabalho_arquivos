#ifndef _INDICE_B_H_
#define _INDICE_B_H_

#include <stdio.h>
#include <stdlib.h>
#include "myStrings.h"
#include "indexElement.h"

#define ORDEM_PAGINA 7
// Fica definido o tamanho em bytes da pagina
#define TAMANHO_PAGINA (sizeof(unsigned char) + ((ORDEM_PAGINA-1) * TAMANHO_IDXELEMENT) + (ORDEM_PAGINA * sizeof(int)))
// E fica definida uma funcao para obtem o offset de uma pagina a partir do seu rrn caso seja um rrn valido
#define filePos(rrn) ((rrn > -1)? (sizeof(int) + (rrn * TAMANHO_PAGINA)) : (-1))

typedef struct arvoreB ARVOREB;
typedef struct paginaB PAGINAB;


ARVOREB *criaArvoreB(const char*);
int getRaiz(ARVOREB*);
void setRaiz(ARVOREB*, int);
int insereArvoreB(ARVOREB*, INDEXELEMENT*, int);
void apagaArvoreB(ARVOREB**);

PAGINAB *criaPaginaB();
void lePaginaB(FILE*, PAGINAB*);
void escrevePaginaB(FILE*, PAGINAB*);
void apagaPaginaB(PAGINAB**);

#endif
