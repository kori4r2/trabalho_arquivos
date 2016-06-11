#include "indiceArvoreB.h"

struct paginaB{
	unsigned char n;
	INDEXELEMENT **nos;
	long int *ponteiros;
};

struct arvoreB{
	char *filename;
	FILE *fp;
};

ARVOREB *criaArvoreB(const char *filename){
	if(filename != NULL){
		ARVOREB *tree = (ARVOREB*)malloc(sizeof(ARVOREB));
		if(tree != NULL){
			tree->filename = myStrdup(filename);
			// Cria um novo arquivo de indice
			tree->fp = fopen(filename, "w");
			if(tree->fp == NULL){
				fprintf(stderr, "criaArvoreB(): Foi impossivel criar o arquivo\n");
				free(tree);
				return NULL;
			}
			// Cria uma pagina vazia e armazena essa pagina como a raiz da arvore
			long int raiz = sizeof(long int);
			fwrite(&raiz, sizeof(long int), 1, tree->fp);
			PAGINAB *page = criaPaginaB();
			if(page == NULL){
				free(tree);
				fclose(tree->fp);
				return NULL;
			}
			escrevePaginaB(tree->fp, page);
			// Apaga memoria alocada e retorna
			apagaPaginaB(&page);
			fclose(tree->fp);
			tree->fp = NULL;
		}else fprintf(stderr, "criaArvoreB(): Erro de alocacao de memoria\n");
		return tree;
	}
	fprintf(stderr, "criaArvoreB(): Parametro invalido passado\n");
	return NULL;
}

void abreIndice(ARVOREB *tree){
	if(tree != NULL){
		if(tree->fp != NULL)
			fprintf(stderr, "abreIndice(): O arquivo de indice ja esta aberto\n");
		else{
			tree->fp = fopen(tree->filename, "r+");
			if(tree->fp == NULL)
				fprintf(stderr, "abreIndice(): Erro ao abrir o indice\n");
		}
	}else fprintf(stderr, "abreIndice(): Parametro invalido passado\n");
}

void fechaIndice(ARVOREB *tree){
	if(tree != NULL){
		if(tree->fp == NULL)
			fprintf(stderr, "fechaIndice(): O arquivo de indice nao esta aberto\n");
		else{
			fclose(tree->fp);
			tree->fp = NULL;
		}
	}else fprintf(stderr, "fechaIndice(): Parametro invalido passado\n");
}

long int getRaiz(ARVOREB *tree){
	if(tree != NULL){
		if(tree->fp == NULL)
			abreIndice(tree);
		else
			fseek(tree->fp, 0, SEEK_SET);
		long int raiz = -1;
		// Obtem o endereco da raiz
		fread(&raiz, sizeof(long int), 1, tree->fp);
		// Move o ponteiro do arquivo para la
		fseek(tree->fp, raiz, SEEK_SET);
		return raiz;
	}else fprintf(stderr, "getRaiz(): Parametro invalido passado\n");
	return -1;
}

void setRaiz(ARVOREB *tree, long int newRoot){
	if(tree != NULL && newRoot >= sizeof(long int)){
		if(tree->fp == NULL)
			abreIndice(tree);
		else
			fseek(tree->fp, 0, SEEK_SET);
		fwrite(&newRoot, sizeof(long int), 1, tree->fp);
	}else fprintf(stderr, "atualizaRaiz(): Parametro invalido passado \n");
}

//-----------------------------------------------------------------------------------------------------

PAGINAB *criaPaginaB(){
	PAGINAB *page = (PAGINAB*)malloc(sizeof(PAGINAB));
	if(page != NULL){
		int i;
		page->n = 0;
		page->nos = (INDEXELEMENT**)malloc(sizeof(INDEXELEMENT*) * (ORDEM_PAGINA - 1));
		// Verificacao de erro
		if(page->nos == NULL){
			free(page);
			fprintf(stderr, "criaPaginaB(): Erro de alocacao de memoria\n");
			return NULL;
		}
		// Zera todos os valores do vetor
		for(i = 0; i < ORDEM_PAGINA - 1; i++)
			page->nos[i] = NULL;
		page->ponteiros = (long int*)malloc(sizeof(long int) * ORDEM_PAGINA);
		// Verificacao de erro
		if(page->ponteiros == NULL){
			free(page->nos);
			free(page);
			fprintf(stderr, "criaPaginaB(): Erro de alocacao de memoria\n");
			return NULL;
		}
		// Zera todos os valores do vetor
		for(i = 0; i < ORDEM_PAGINA; i++)
			page->ponteiros[i] = -1;
	}else fprintf(stderr, "criaPaginaB(): Erro de alocacao de memoria\n");
	return page;
}

void lePaginaB(FILE *fp, PAGINAB *page){
	if(fp != NULL && page != NULL){
		int i;
		fread(&page->n, sizeof(unsigned char), 1, fp);
		for(i = 0; i < ORDEM_PAGINA - 1; i++)
			readIdxElement(page->nos[i], fp);
		for(i = 0; i < ORDEM_PAGINA; i++)
			fread(&page->ponteiros[i], sizeof(long int), 1, fp);
	}else fprintf(stderr, "lePaginaB(): Parametro invalido passado\n");
}

void escrevePaginaB(FILE *fp, PAGINAB *page){
	if(fp != NULL && page != NULL){
		int i;
		fwrite(&page->n, sizeof(unsigned char), 1, fp);
		for(i = 0; i < ORDEM_PAGINA - 1; i++)
			writeIdxElement(page->nos[i], fp);
		for(i = 0; i < ORDEM_PAGINA; i++)
			fwrite(&page->ponteiros[i], sizeof(long int), 1, fp);
	}else fprintf(stderr, "escrevePaginaB(): Parametro invalido passado\n");
}

void apagaPaginaB(PAGINAB **page){
	if(page != NULL && *page != NULL){
		if((*page)->nos != NULL)
			free((*page)->nos);
		if((*page)->ponteiros != NULL)
			free((*page)->ponteiros);
		free(*page);
		*page = NULL;
	}else fprintf(stderr, "apagaPaginaB(): Parametro invalido passado\n");
}
