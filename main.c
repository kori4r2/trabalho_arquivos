// Alisson Mateus de Oliveira Magalhães - nUSP 8066287
// Ítalo Tobler Silva - nUSP 8551910

#include <stdio.h>
#include <stdlib.h>
#include "filmes.h"

int main(int argc, char *argv[]){

	char *titulo, *pais, *descr, *genero, *filename;
	int ano, dur, op;
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

	system("clear");
	do{
		printf("\t\tSEJA BEM VINDO AO NOSSO ACERVO VIRTUAL DE FILMES!!\t\t\n\n");
		printf("O que deseja fazer?\n");
		printf("1 - Listar todo o acervo de filmes\n");
		printf("2 - Buscar um filme específico\n");
		printf("3 - Sair do programa\n");
		printf("Opção Selecionada: ");
		scanf("%d", &op);
		
		if(op == 1){
			system("clear");			
			printf("Lista de todos os filmes do nosso acervo:\n\n");
			imprimeCatalogo(c);
			printf("\n\nCatálogo Impresso!!");	
			printf("\n\n\n");
		}

		if(op == 2){
			system("clear");			
			//printf("Digite o código do filme: ");
			//scanf("%d", &codigo_filme);
			//TODO
			procuraFilme(c, 2);
			printf("\n\n\n");
		}
		
	} while(op != 3);
	
	system("clear");
	printf("Obrigado por nos visitar! Volte sempre!\n\n\n");
	
	
	//printf("fim da execução\n");
	apagaCatalogo(&c);

	return 0;
}
