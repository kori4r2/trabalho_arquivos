// Alisson Mateus de Oliveira Magalhães - nUSP 8066287
// Ítalo Tobler Silva - nUSP 8551910
// Pedro Henrique Siqueira de Oliveira - nUSP 8922006

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

FILME *criaFilme(int ano, int dur, const char *titulo, const char *descr, const char *pais, const char *genero){
	if(ano > 0 && dur > 0 && titulo != NULL && descr != NULL && pais != NULL && genero != NULL){
		FILME *novoFilme = (FILME*)malloc(sizeof(FILME));

		if(novoFilme != NULL){
			novoFilme->id = 0;
			novoFilme->ano = ano;
			novoFilme->dur = dur;
			novoFilme->titulo = myStrdup(titulo);
			novoFilme->descr = myStrdup(descr);
			novoFilme->pais = myStrdup(pais);
			novoFilme->genero = myStrdup(genero);
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
		fread(filme->titulo+i, sizeof(char), 1, fp);
	}while(filme->titulo[i++] != FIM_CAMPO);
	filme->titulo[i-1] = '\0';
	i = 0;
	do{
		filme->descr = (char*)realloc(filme->descr, sizeof(char) * (i+1));
		fread(filme->descr+i, sizeof(char), 1, fp);
	}while(filme->descr[i++] != FIM_CAMPO);
	filme->descr[i-1] = '\0';
	i = 0;
	do{
		filme->pais = (char*)realloc(filme->pais, sizeof(char) * (i+1));
		fread(filme->pais+i, sizeof(char), 1, fp);
	}while(filme->pais[i++] != FIM_CAMPO);
	filme->pais[i-1] = '\0';
	i = 0;
	do{
		filme->genero = (char*)realloc(filme->genero, sizeof(char) * (i+1));
		fread(filme->genero+i, sizeof(char), 1, fp);
	}while(filme->genero[i++] != FIM_CAMPO);
	filme->genero[i-1] = '\0';

	fread(&fimRegistro, sizeof(char), 1, fp);
	if(fimRegistro != FIM_REGISTRO)
		fprintf(stderr, "leFilme: erro na organizacao do arquivo(%c, not %c)\n", fimRegistro, FIM_REGISTRO);

	return 1;
}

void escreveFilme(FILE *fp, FILME *filme){
	int i;
	char fimCampo = FIM_CAMPO, fimRegistro = FIM_REGISTRO;
	fwrite(&filme->id, sizeof(unsigned int), 1, fp);
	fwrite(&filme->ano, sizeof(int), 1, fp);
	fwrite(&filme->dur, sizeof(int), 1, fp);
	for(i = 0; filme->titulo[i] != '\0'; i++)
		fwrite(filme->titulo+i, sizeof(char), 1, fp);
	fwrite(&fimCampo, sizeof(char), 1, fp);
	for(i = 0; filme->descr[i] != '\0'; i++)
		fwrite(filme->descr+i, sizeof(char), 1, fp);
	fwrite(&fimCampo, sizeof(char), 1, fp);
	for(i = 0; filme->pais[i] != '\0'; i++)
		fwrite(filme->pais+i, sizeof(char), 1, fp);
	fwrite(&fimCampo, sizeof(char), 1, fp);
	for(i = 0; filme->genero[i] != '\0'; i++)
		fwrite(filme->genero+i, sizeof(char), 1, fp);
	fwrite(&fimCampo, sizeof(char), 1, fp);
	fwrite(&fimRegistro, sizeof(char), 1, fp);
}

void esvaziaFilme(FILME *filme){
	if(filme != NULL){
		if(filme->titulo != NULL){
			free(filme->titulo);
			filme->titulo = NULL;
		}
		if(filme->descr != NULL){
			free(filme->descr);
			filme->descr = NULL;
		}
		if(filme->pais != NULL){
			free(filme->pais);
			filme->pais = NULL;
		}
		if(filme->genero != NULL){
			free(filme->genero);
			filme->genero = NULL;
		}
		filme->ano = 0;
		filme->dur = 0;
	}else fprintf(stderr, "esvaziaFilme: parametro invalido passado\n");
}

void imprimeFilme(FILME *filme){
	if(filme != NULL){
		printf("/-------------------------------------------------------------------------------\n");
		printf("id       : %d\n", filme->id);
		printf("Título   : %s\n", filme->titulo);
		printf("Descrição:\n   ");
		printLongString(filme->descr, 75);
		printf("\n");
		printf("Gênero   : %s\n", filme->genero);
		printf("Duração  : %d min\n", filme->dur);
		printf("Ano      : %d\n", filme->ano);
		printf("País     : %s\n", filme->pais);
		printf("-------------------------------------------------------------------------------/\n");
	}
}

void apagaFilme(FILME **filme){
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

CATALOGO *criaCatalogo(const char *nomeArquivo){
	if(nomeArquivo != NULL){
		CATALOGO *novoCatalogo = (CATALOGO*)malloc(sizeof(CATALOGO));
		if(novoCatalogo != NULL){
			novoCatalogo->n = 0;
			novoCatalogo->filename = myStrdup(nomeArquivo);

			FILE *fp = fopen(novoCatalogo->filename, "wb+");
			if(fp == NULL){
				apagaCatalogo(&novoCatalogo);
				fprintf(stderr, "criaCatalogo: erro ao criar o arquivo\n");
			}else
				fclose(fp);

			return novoCatalogo;
		}else fprintf(stderr, "criaCatalogo: erro na alocacao de memoria\n");
		return novoCatalogo;
	}
	fprintf(stderr, "criaCatalogo: parametro invalido passado\n");
	return NULL;
}

void apagaCatalogo(CATALOGO **catalogo){
	if(catalogo != NULL && *catalogo != NULL){
		if((*catalogo)->filename != NULL)
			free((*catalogo)->filename);
		free(*catalogo);
		*catalogo = NULL;
	}else fprintf(stderr, "apagaCatalogo: parametro invalido passado\n");
}

void insereFilme(CATALOGO *catalogo, FILME **filme){
	if(catalogo != NULL && filme != NULL && *filme != NULL){
		FILE *fp = fopen(catalogo->filename, "ab");
		if(fp == NULL)
			fprintf(stderr, "insereFilme: erro ao abrir o arquivo\n");
		else{
			catalogo->n++;
			(*filme)->id = catalogo->n;
			escreveFilme(fp, *filme);
			apagaFilme(filme);
			fclose(fp);
		}
	}else fprintf(stderr, "insereFilme: parametro invalido passado\n");
}

void imprimeCatalogo(CATALOGO *catalogo){
	if(catalogo != NULL){
		FILME *filme = (FILME*)malloc(sizeof(FILME));
		filme->titulo = NULL;
		filme->descr = NULL;
		filme->genero = NULL;
		filme->pais = NULL;
		filme->ano = 0;
		filme->dur = 0;
		FILE *fp = fopen(catalogo->filename, "rb");
		if(fp == NULL)
			fprintf(stderr, "imprimeCatalogo: erro ao abrir o arquivo\n");
		else{
			while(leFilme(fp, filme) != 0){
				imprimeFilme(filme);
				esvaziaFilme(filme);
			}
			fclose(fp);
		}
		apagaFilme(&filme);
	}else fprintf(stderr, "imprimeCatalogo: parametro invalido passado\n");
}

void procuraFilme(CATALOGO *catalogo, unsigned int id){
	if(catalogo != NULL && id > 0){
		long int offset = 0;
		int curId, aux;
		char caractere;
		FILME *filme = (FILME*)malloc(sizeof(FILME));
		filme->titulo = NULL;
		filme->descr = NULL;
		filme->genero = NULL;
		filme->pais = NULL;
		filme->ano = 0;
		filme->dur = 0;
		FILE *fp = fopen(catalogo->filename, "rb");
		if(fp == NULL)
			fprintf(stderr, "procuraFilme: erro ao abrir o arquivo\n");
		else{
			while((fread(&curId, sizeof(unsigned int), 1, fp)) > 0 && curId != id){
				fread(&aux, sizeof(int), 1, fp);
				fread(&aux, sizeof(int), 1, fp);
				offset += sizeof(unsigned int) + (2 * sizeof(int));
				do{
					fread(&caractere, sizeof(char), 1, fp);
					offset++;
				}while(caractere != FIM_CAMPO);
				do{
					fread(&caractere, sizeof(char), 1, fp);
					offset++;
				}while(caractere != FIM_CAMPO);
				do{
					fread(&caractere, sizeof(char), 1, fp);
					offset++;
				}while(caractere != FIM_CAMPO);
				do{
					fread(&caractere, sizeof(char), 1, fp);
					offset++;
				}while(caractere != FIM_CAMPO);
				fread(&caractere, sizeof(char), 1, fp);
				offset++;
				if(caractere != FIM_REGISTRO){
					fprintf(stderr, "procuraFilme: erro na organizacao do arquivo (%c, not %c)\n", caractere, FIM_REGISTRO);
					apagaFilme(&filme);
					fclose(fp);
					return;
				}
			}
			if(!feof(fp) && curId == id){
				fseek(fp, offset, SEEK_SET);
				leFilme(fp, filme);
				imprimeFilme(filme);
			}else{
				printf("Filme nao encontrado\n");
			}
			fclose(fp);
		}
		apagaFilme(&filme);
	}else fprintf(stderr, "procuraFilme: parametro invalido passado\n");
}
