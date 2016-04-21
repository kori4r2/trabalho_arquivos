#include <stdio.h>
#include <stdlib.h>
#include "filmes.h"

int main(int argc, char *argv[]){

	char *titulo, *pais, *descr, *genero, *filename;
	int ano, dur;
	FILME *filme;
	// So pra compilar, ainda vo fazer o teste
	filename = myStrdup("filmes.data");
	CATALOGO *c = criaCatalogo(filename);
	printf("catalogo criado\n");
	titulo = myStrdup("filme numero 1");
	descr = myStrdup("primeira descricao");
	genero = myStrdup("acao");
	pais = myStrdup("US");
	ano = 2005;
	dur = 120;
	filme = criaFilme(ano, dur, titulo, descr, pais, genero);
	insereFilme(c, &filme);
	titulo = myStrdup("filme numero 2");
	descr = myStrdup("segunda descricao");
	genero = myStrdup("aventura");
	pais = myStrdup("India");
	ano = 2004;
	dur = 130;
	filme = criaFilme(ano, dur, titulo, descr, pais, genero);
	insereFilme(c, &filme);
	imprimeCatalogo(c);
	printf("catalogo impresso\n");
	printf("\n");
	procuraFilme(c, 2);
	printf("fim da execução\n");
	apagaCatalogo(&c);

	return 0;
}
