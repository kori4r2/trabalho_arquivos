// Alisson Mateus de Oliveira Magalhães - nUSP 8066287
// Ítalo Tobler Silva - nUSP 8551910

#ifndef _MY_STRINGS_H_
#define _MY_STRINGS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*------------------------------------------------------------------------------------------------------------
   myStrdup()
        Funcao que aloca na memoria heap uma string contendo copia da string passada como parametro e retorna
	seu ponteiro.
        - Parametros
          const char* : string a ser copiada;
        - Retorno
          char* : string criada; NULL - caso erro de alocacao ou parametro invalido;
*/
char *myStrdup(const char*);
/*------------------------------------------------------------------------------------------------------------
   getWord()
        Funcao que le uma palavra da stream de dados passada e retorna uma string alocada em memoria heap
	com o resultado da leitura. As condicoes de parada sao espaco, quebra de linha ou fim de arquivo.
        - Parametros
          FILE* : stream de dados a ser lida;
        - Retorno
          char* : string criada; NULL - caso erro de alocacao ou parametro invalido;
*/
char *getWord(FILE*);
/*------------------------------------------------------------------------------------------------------------
   readLimitedString()
        Funcao que le uma linha da stream de dados passada mas armazena um numero maximo de caracteres em
	uma string alocada em memoria heap. As condicoes de parada sao quebra de linha e fim de arquivo.
        - Parametros
          FILE* : stream de dados a ser lida;
	  int : tamanho maximo da string (contando com o caractere indicador de fim: '\0');
        - Retorno
          char* : string criada; NULL - caso erro de alocacao ou parametro invalido;
*/
char *readLimitedString(FILE*, int);
/*------------------------------------------------------------------------------------------------------------
   myGetLine()
        Funcao que le uma linha da stream de dados passada e armazena tudo o que foi lido em uma string alocada
	em memoria heap. As condicoes de parada sao quebra de linha e fim de arquivo;
        - Parametros
          FILE* : stream de dados a ser lida;
        - Retorno
          char* : string criada; NULL - caso erro de alocacao ou parametro invalido;
*/
char *myGetLine(FILE*);
/*------------------------------------------------------------------------------------------------------------
   printLongString()
        Funcao que imprime uma string muito grande de modo que ela tenha um numero maximo de caracteres
	impressos por linha para facilitar visualizacao e melhorar formatacao do texto. Quando uma linha
	terminar em uma palavra muito grande o limite estabelecido pode ser ultrapassado.
        - Parametros
          const char* : string a ser impressa;
	  int : limite (aproximado) de caracteres a ser impresso por linha;
        - Retorno
          void
*/
void printLongString(const char*, int);

#endif
