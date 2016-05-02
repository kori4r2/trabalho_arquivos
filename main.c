// Alisson Mateus de Oliveira Magalhães - nUSP 8066287
// Ítalo Tobler Silva - nUSP 8551910
// Pedro Henrique Siqueira de Oliveira - nUSP 8922006

// defines identificam o sistema operacional
#ifdef _WIN32
	#define WINDOWS 1
	#define LINUX 0
#elif __linux__
	#define WINDOWS 0
	#define LINUX 1
#elif __unix__
	#define WINDOWS 0
	#define LINUX 1
#else
	#define WINDOWS 0
	#define LINUX 0
#endif

#include <stdio.h>
#include <stdlib.h>
#include "filmes.h"
#include <time.h>


void shuffle(int*, int);
void geracaoAleatoria100Filmes(CATALOGO*);

int main(int argc, char *argv[]){
	int op, codigoFilme;
	char *filename;

	// Gera a seed para a função rand()
	srand(time(NULL));
	//Cria Catalogo
	filename = myStrdup("filmes.data");
	CATALOGO *c = criaCatalogo(filename);
	printf("catalogo criado\n");

	geracaoAleatoria100Filmes(c);

	if(WINDOWS) system("cls"); //windows
	else if(LINUX) system("clear"); //linux
	do{
		printf("\t\tSEJA BEM VINDO AO NOSSO ACERVO VIRTUAL DE FILMES!!\t\t\n\n");
		printf("O que deseja fazer?\n");
		printf("1 - Listar todo o acervo de filmes\n");
		printf("2 - Buscar um filme especifico\n");
		printf("3 - Sair do programa\n");
		printf("Opcao Selecionada: ");
		scanf("%d", &op);

		if(op == 1){
			if(WINDOWS) system("cls"); //windows
			else if(LINUX) system("clear"); //linux
			printf("Lista de todos os filmes do nosso acervo:\n\n");
			imprimeCatalogo(c);
			printf("\n\nCatalogo Impresso!!");
			printf("\n\n\n");
		}else if(op == 2){
			if(WINDOWS) system("cls"); //windows
			else if(LINUX) system("clear"); //linux

			printf("Digite o codigo do filme: ");
			scanf("%d", &codigoFilme);
			procuraFilme(c, codigoFilme);
			printf("\n\n\n");
		}else{
			printf("Operacao invalida!\n");
		}
	} while(op != 3);

	if(WINDOWS) system("cls"); //windows
	else if(LINUX) system("clear"); //linux

	printf("Obrigado por nos visitar! Volte sempre!\n\n\n");

	apagaCatalogo(&c);

	return 0;
}

void geracaoAleatoria100Filmes(CATALOGO* c) {
	char *titulo, *pais, *descr, *genero;
	char *linha_entrada;
	char **filmes = (char**)malloc(sizeof(char*) * 100);
	int ano, dur, num_filmes, i;
	FILME* filme;
	FILE* entrada;

	num_filmes = 0;
	if((entrada = fopen("100filmes.txt", "r"))) {
		while((linha_entrada = myGetLine(entrada)) != NULL) {
			filmes[num_filmes++] = linha_entrada;
		}
	}else{
		fprintf(stderr, "geracaoAleatoria100Filmes: erro ao abrir o arquivo\n");
		exit(1);
	}

	int aux_shuffle[num_filmes];
	for(i = 0; i < num_filmes; i++)
		aux_shuffle[i] = i;
	shuffle(aux_shuffle, num_filmes);

	for(i = 0; i < num_filmes; i++) {
		ano = atoi(strtok(filmes[aux_shuffle[i]], "|"));
		dur = atoi(strtok(NULL, "|"));
		titulo = strtok(NULL, "|");
		descr = strtok(NULL, "|");
		pais = strtok(NULL, "|");
		genero = strtok(NULL, "|");
		filme = criaFilme(ano, dur, titulo, descr, pais, genero);
		insereFilme(c, &filme);
	}

	for(i = 0; i < num_filmes; i++) {
		free(filmes[i]);
	}
	free(filmes);
	fclose(entrada);
}

void shuffle(int* aux_shuffle, int numFilmes) {
	int i, j, t;
	for (i = 0; i < numFilmes - 1; i++) {
		j = (rand() % (numFilmes-1));
		t = aux_shuffle[j];
		aux_shuffle[j] = aux_shuffle[i];
		aux_shuffle[i] = t;
	}
}
