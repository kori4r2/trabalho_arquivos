// Alisson Mateus de Oliveira Magalhães - nUSP 8066287
// Ítalo Tobler Silva - nUSP 8551910
// Pedro Henrique Siqueira de Oliveira - nUSP 8922006

#ifndef _FILMES_H_
#define _FILMES_H_

#include <ctype.h>
#include "myStrings.h"

typedef struct filme FILME;
typedef struct catalogo CATALOGO;

/*------------------------------------------------------------------------------------------------------------
   criaFilme()
        Funcia que aloca a memoria e cria um filme com as informacoes passadas como parametro
        - Parametros
          int : ano do filme;
          int : duracao em minutos do filme;
	  const char* : titulo do filme;
	  const char* : descricao do filme;
	  const char* : pais onde o filme foi gravado;
	  const char* : genero do filme;
        - Retorno
          FILME* : ponteiro para o filme criado; NULL - caso erro de alocacao ou parametro invalido;
*/
FILME *criaFilme(int, int, const char*, const char*, const char*, const char*);
/*------------------------------------------------------------------------------------------------------------
   apagaFilme()
        Funcao que apaga um filme alocado em memoria heap, setando NULL no seu ponteiro.
        - Parametros
          FILME** : endereco do ponteiro para o filme criado; NULL - caso parametro invalido;
        - Retorno
          void
*/
void apagaFilme(FILME**);
/*------------------------------------------------------------------------------------------------------------
   criaCatalogo()
        Funcao que aloca a memoria e cria um catalogo de filmes. Ao criar o catalogo, cria-se um arquivo para
	armazenar os dados utilizando o nome passado. Caso ja exista um arquivo com o nome passado, ele sera
	sobreescrito.
        - Parametros
	  const char* : string contendo o nome do arquivo a ser criado
        - Retorno
          CATALOGO* : ponteiro para o catalogo criado; NULL - caso erro de alocacao ou parametro invalido;
*/
CATALOGO *criaCatalogo(const char*);
/*------------------------------------------------------------------------------------------------------------
   apagaCatalogo()
        Funcao que apaga um catalogo alocado em memoria heap, setando NULL no seu ponteiro.
        - Parametros
	  CATALOGO** : endereco do ponteiro para o catalogo criado; NULL - caso parametro invalido;
        - Retorno
          void
*/
void apagaCatalogo(CATALOGO**);
/*------------------------------------------------------------------------------------------------------------
   insereFilme()
        Funcao que insere um filme no catalogo de filmes criado(a insercao ocorre no arquivo de acordo com a 
	organizacao pre-definida). Apos a insercao ser realizada, o filme passado sera apagado.
        - Parametros
          CATALOGO* : ponteiro para o catalogo criado;
          FILME** : endereco do ponteiro para o filme criado;
        - Retorno
          void
*/
void insereFilme(CATALOGO*, FILME**);
/*------------------------------------------------------------------------------------------------------------
   imprimeCatalogo()
        Funcao que imprime as informacoes de todos os filmes inseridos no catalogo.
        - Parametros
          CATALOGO* : ponteiro para o catalogo criado;
        - Retorno
          void
*/
void imprimeCatalogo(CATALOGO*);
/*------------------------------------------------------------------------------------------------------------
   procuraFilme()
        Funcao que busca um filme contendo o id passado e imprime as informacoes desse filme na tela.
        - Parametros
          CATALOGO* : ponteiro para o catalogo criado;
          unsigned int : id do filme sendo buscado;
        - Retorno
          void
*/
void procuraFilme(CATALOGO*, unsigned int);

#endif
