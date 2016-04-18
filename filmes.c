#include "filmes.h"

#define TAM_FIXO (sizeof(unsigned int) + (2 * sizeof(int)))

struct filme{
	unsigned int id;
	int ano;
	int dur;
	char *titulo;
	char *descr;
	char *pais;
	char *genero;
};

struct catalogo{
	int n;
	char *filename;
};

FILME *criarFilme(int ano, int dur, char *titulo, char *descr, char *pais, char *genero){
	if(ano > 0 && dur > 0 && titulo != NULL && descr != NULL && pais != NULL && genero != NULL){
		FILME *novoFilme = (FILME*)malloc(sizeof(FILME));

		if(novoFilme != NULL){
			novoFilme->id = 0;
			novoFilme->ano = ano;
			novoFilme->dur = dur;
			novoFilme->titulo = titulo;
			novoFilme->descr = descr;
			novoFilme->pais = pais;
			novoFilme->genero = genero;
		}else fprintf(stderr, "criarFilme: erro na alocacao de memoria\n");

		return novoFilme;
	}else fprintf(stderr, "criarFilme: parametro invalido passado\n");
	return NULL;
}

void *apagarFilme(FILME **filme){
	if(filme != NULL && *filme != NULL){
		if((*filme)->titulo != NULL)
			free((*filme)->titulo);
		if((*filme)->descr != NULL)
			free((*filme)->descr);
		if((*filme)->pais != NULL)
			free((*filme)->pais);
		if((*filme)->genero != NULL)
			free((*filme)->genero);
		free(*filme);
		*filme = NULL;
	}else fprintf(stderr, "apagarFilme: parametro invalido passado\n");
}

CATALOGO *criarCatalogo(char *nomeArquivo){
	if(nomeArquivo != NULL){
		CATALOGO *novoCatalogo = (CATALOGO*)malloc(sizeof(CATALOGO));
		if(novoCatalogo != NULL){
			novoCatalogo->n = 0;
			novoCatalogo->filename = nomeArquivo;

			FILE *fp = fopen(novoCatalogo->filename, "w+");
			if(fp == NULL){
				apagarCatalogo(&novoCatalogo);
				fprintf(stderr, "criarCatalogo: erro ao criar o arquivo\n");
			}else
				fclose(fp);

			return novoCatalogo;
		}else fprintf(stderr, "criarCatalogo: erro na alocacao de memoria\n");
		return novoCatalogo;
	}else fprintf(stderr, "criarCatalogo: parametro invalido passado\n");
}

void apagarCatalogo(CATALOGO **catalogo){
	if(catalogo != NULL && *catalogo != NULL){
		if((*catalogo)->filename != NULL)
			free((*catalogo)->filename);
		free(*catalogo);
		*catalogo = NULL;
	}else fprintf(stderr, "apagarCatalogo: parametro invalido passado");
}
