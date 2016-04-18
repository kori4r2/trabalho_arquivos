#include "filmes.h"

#define FIM_CAMPO '|'
#define FIM_REGISTRO '^'
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

// Funcoes de filme-------------------------------------------------------------------------------------------

FILME *criaFilme(int ano, int dur, char *titulo, char *descr, char *pais, char *genero){
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
		}else fprintf(stderr, "criaFilme: erro na alocacao de memoria\n");

		return novoFilme;
	}else fprintf(stderr, "criaFilme: parametro invalido passado\n");
	return NULL;
}

int leFilme(FILE *fp, FILME *filme){
	if(feof(fp)) return 0;

	int i;
	char fimRegistro;
	fread(&filme->id, sizeof(unsigned int), 1, fp);
	if(feof(fp)) return 0;
	fread(&filme->ano, sizeof(int), 1, fp);
	fread(&filme->dur, sizeof(int), 1, fp);

	i = 0;
	do{
		filme->titulo = (char*)realloc(filme->titulo, sizeof(char) * (i+1));
		fread(filme->titulo+i, sizeof(char), 1, fp)
	}while(titulo[i++] != FIM_CAMPO);
	titulo[i-1] = '\0';
	i = 0;
	do{
		filme->descr = (char*)realloc(filme->descr, sizeof(char) * (i+1));
		fread(filme->descr+i, sizeof(char), 1, fp)
	}while(descr[i++] != FIM_CAMPO);
	descr[i-1] = '\0';
	i = 0;
	do{
		filme->pais = (char*)realloc(filme->pais, sizeof(char) * (i+1));
		fread(filme->pais+i, sizeof(char), 1, fp)
	}while(pais[i++] != FIM_CAMPO);
	pais[i-1] = '\0';
	i = 0;
	do{
		filme->genero = (char*)realloc(filme->genero, sizeof(char) * (i+1));
		fread(filme->genero+i, sizeof(char), 1, fp)
	}while(genero[i++] != FIM_CAMPO);
	genero[i-1] = '\0';

	fread(&fimRegistro, sizeof(char), 1, fp);
	if(fimRegistro != FIM_REGISTRO);
		fprintf(stderr, "leFilme: erro na organizacao do arquivo\n");
	return 1;
}

void escreveFilme(FILE *fp, FILME *filme){
	int i;
	char fimCampo = FIM_CAMPO, fimRegistro = FIM_REGISTRO;
	fwrite(&filme->id, sizeof(unsigned int), 1, fp);
	fwrite(&filme->ano, sizeof(int), 1, fp);
	fwrite(&filme->dur, sizeof(int), 1, fp);
	for(i = 0; filme->titulo[i] != '\0'; i++)
		fwrite(filme->titulo+i, sizeof(char), 1, fp)
	fwrite(&fimCampo, sizeof(char), 1, fp);
	for(i = 0; filme->descr[i] != '\0'; i++)
		fwrite(filme->descr+i, sizeof(char), 1, fp)
	fwrite(&fimCampo, sizeof(char), 1, fp);
	for(i = 0; filme->pais[i] != '\0'; i++)
		fwrite(filme->pais+i, sizeof(char), 1, fp)
	fwrite(&fimCampo, sizeof(char), 1, fp);
	for(i = 0; filme->genero[i] != '\0'; i++)
		fwrite(filme->genero+i, sizeof(char), 1, fp)
	fwrite(&fimCampo, sizeof(char), 1, fp);
	fwrite(&fimRegistro, sizeof(char), 1, fp);
}

void esvaziaFilme(FILME *filme){
	if(filme != NULL){
		if(filme->titulo != NULL)
			free(filme->titulo);
		if(filme->descr != NULL)
			free(filme->descr);
		if(filme->pais != NULL)
			free(filme->pais);
		if(filme->genero != NULL)
			free(filme->genero);
	}else fprintf(stderr, "esvaziaFilme: parametro invalido passado\n");
}

void imprimeFilme(Filme **filme){
}

void *apagaFilme(FILME **filme){
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
	}else fprintf(stderr, "apagaFilme: parametro invalido passado\n");
}

//Funcoes de catalogo--------------------------------------------------------------------------------

CATALOGO *criaCatalogo(char *nomeArquivo){
	if(nomeArquivo != NULL){
		CATALOGO *novoCatalogo = (CATALOGO*)malloc(sizeof(CATALOGO));
		if(novoCatalogo != NULL){
			novoCatalogo->n = 0;
			novoCatalogo->filename = nomeArquivo;

			FILE *fp = fopen(novoCatalogo->filename, "w+");
			if(fp == NULL){
				apagaCatalogo(&novoCatalogo);
				fprintf(stderr, "criaCatalogo: erro ao criar o arquivo\n");
			}else
				fclose(fp);

			return novoCatalogo;
		}else fprintf(stderr, "criaCatalogo: erro na alocacao de memoria\n");
		return novoCatalogo;
	}else fprintf(stderr, "criaCatalogo: parametro invalido passado\n");
}

void apagaCatalogo(CATALOGO **catalogo){
	if(catalogo != NULL && *catalogo != NULL){
		if((*catalogo)->filename != NULL)
			free((*catalogo)->filename);
		free(*catalogo);
		*catalogo = NULL;
	}else fprintf(stderr, "apagaCatalogo: parametro invalido passado\n");
}

void inserirFilme(CATALOGO *catalogo, FILME *filme){
	if(catalogo != NULL && filme != NULL){
		FILE *fp = fopen(catalogo->filename, "a");
		if(fp == NULL)
			fprintf(stderr, "inserirFilme: erro ao abrir o arquivo\n");
		else{
			catalogo->n++;
			filme->id = catalogo->n;
			escreveFilme(fp, filme);
			apagaFilme(&filme);
		}
	}else fprintf(stderr, "inserirFilme: parametro invalido passado\n");
}

void imprimeCatalogo(CATALOGO *catalogo){
	if(catalogo != NULL){
		FILME *filme = (FILME*)malloc(sizeof(FILME));
		FILE *fp = fopen(catalogo->filename, "r");
		if(fp == NULL)
			fprintf(stderr, "imprimeCatalogo: erro ao abrir o arquivo\n");
		else{
			while(leFilme(fp, filme)){
				imprimeFilme(filme);
				esvaziaFilme(filme);
			}
			fclose(fp);
		}
		apagaFilme(&filme);
	}else fprintf(stderr, "imprimeCatalogo: parametro invalido passado\n");
}

long int procuraFilme(CATALOGO *catalogo, unsigned int id){
	if(catalogo != NULL){
		long int offset = 0;
		FILME *filme = (FILME*)malloc(sizeof(FILME));
		FILE *fp = fopen(catalogo->filename, "r");
		if(fp == NULL)
			fprintf(stderr, "procuraFilme: erro ao abrir o arquivo\n");
		else{
			//to do
			fclose(fp);
		}
		apagaFilme(&filme);
	}else fprintf(stderr, "procuraFilme: parametro invalido passado\n");
}
