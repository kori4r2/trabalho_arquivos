#include "indiceArvoreB.h"

struct paginaB{
	unsigned char n;
	INDEXELEMENT **nos;
	int *ponteiros;
};

struct arvoreB{
	char *filename;
	FILE *fp;
	int size;
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
			int raiz = sizeof(int);
			fwrite(&raiz, sizeof(int), 1, tree->fp);
			PAGINAB *page = criaPaginaB();
			if(page == NULL){
				free(tree);
				fclose(tree->fp);
				return NULL;
			}
			escrevePaginaB(tree->fp, page);
			tree->size = 1;
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
		if(tree->fp != NULL){
			//fprintf(stderr, "abreIndice(): O arquivo de indice ja esta aberto\n");
		}else{
			tree->fp = fopen(tree->filename, "r+");
			if(tree->fp == NULL)
				fprintf(stderr, "abreIndice(): Erro ao abrir o indice\n");
		}
	}else fprintf(stderr, "abreIndice(): Parametro invalido passado\n");
}

void fechaIndice(ARVOREB *tree){
	if(tree != NULL){
		if(tree->fp == NULL){
			//fprintf(stderr, "fechaIndice(): O arquivo de indice nao esta aberto\n");
		}else{
			fclose(tree->fp);
			tree->fp = NULL;
		}
	}else fprintf(stderr, "fechaIndice(): Parametro invalido passado\n");
}

void apagaArvoreB(ARVOREB **tree){
	if(tree != NULL && *tree != NULL){
		if((*tree)->filename != NULL)
			free((*tree)->filename);
		fechaIndice(*tree);
		free(*tree);
		*tree = NULL;
	}else fprintf(stderr, "apagaArvoreB(): Parametro invalido passado\n");
}

int getRaiz(ARVOREB *tree){
	if(tree != NULL){
		long int raiz = -1;
		if(tree->fp == NULL){
			// Caso o indice estiver fechado
			abreIndice(tree);
			// Obtem o endereco da raiz
			fread(&raiz, sizeof(int), 1, tree->fp);
			// Move o ponteiro do arquivo para la
			fseek(tree->fp, filePos(raiz), SEEK_SET);
		}else{
			// Caso o indice ja esteja aberto
			long int curPos = ftell(tree->fp);
			fseek(tree->fp, 0, SEEK_SET);
			// Obtem a raiz
			fread(&raiz, sizeof(int), 1, tree->fp);
			// E volta para a posicao anterior
			fseek(tree->fp, curPos, SEEK_SET);
		}
		return raiz;
	}else fprintf(stderr, "getRaiz(): Parametro invalido passado\n");
	return -1;
}

void setRaiz(ARVOREB *tree, int newRoot){
	if(tree != NULL && newRoot >= sizeof(int)){
		if(tree->fp == NULL){
			// Se o indice estava fechado, abre e atualiza
			abreIndice(tree);
			fwrite(&newRoot, sizeof(int), 1, tree->fp);
		}else{
			// Se ja estava aberto, armazena a posicao atual antes de atualizar
			long int curPos = ftell(tree->fp);
			fseek(tree->fp, 0, SEEK_SET);
			fwrite(&newRoot, sizeof(int), 1, tree->fp);
			// Volta para a posicao anterior apos atualizar
			fseek(tree->fp, curPos, SEEK_SET);
		}
	}else fprintf(stderr, "setRaiz(): Parametro invalido passado \n");
}

long int buscaArvoreB(ARVOREB *tree, unsigned int id){
	if(tree != NULL){
		int i, aux;
		long int next;
		PAGINAB *page = criaPaginaB();

		next = filePos(getRaiz(tree));
		while(next > -1){
			// Enquanto nao tentar descer a partir de um no folha
			// obtem a proxima pagina a ser analisada
			fseek(tree->fp, next, SEEK_SET);
			next = 0;
			lePaginaB(tree->fp, page);
			// Percorre todos o elementos da pagina enquanto for necessario
			for(i = 0; (i < page->n) && (next == 0); i++){
				// Compara o id buscado com o elemento atual
				aux = checkElement(page->nos[i], id);
				// Se for igual, retorna o offset no arquivo de dados
				if(aux == 0){
					next = getOffset(page->nos[i]);
					apagaPaginaB(&page);
					return next;
				// Se for menor, obtem o ponteiro da pagina a ser buscada
				}else if(aux < 0){
					next = filePos(page->ponteiros[i]);
				}
			}
			// Se tiver chegado ate o fim da pagina sem encontrar e sem detectar
			// qual a proxima pagina a ser buscada, obtem o ponteiro correspondente
			if(next == 0)
				next = filePos(page->ponteiros[page->n]);
		}
		// Caso nao tenha encontrado, libera a memoria alocada e retorna -1
		apagaPaginaB(&page);
	}else fprintf(stderr, "buscaArvoreB(): Parametro invalido passado\n");
	return -1;
}

void swap(INDEXELEMENT **vector, int i, int j){
	INDEXELEMENT *aux = vector[i];
	vector[i] = vector[j];
	vector[j] = aux;
}

// No inicio das 3 funcoes abaixo a pagina com tamanho acima do normal esta armazenada no
// final do arquivo (rrn = tree->size) e caso a funcao gere overflow, a nova pagina com tamanho
// acima do normal deve tambem ficar no final do arquivo(rrn = tree->size)
void splitRaiz(ARVOREB *tree){
	tree->size++;
	tree->size++;
}

bool split2to3(ARVOREB *tree, int fullPageRRN){
	tree->size++;
	// Retorna false caso ainda seja preciso avaliar
	return false;
}

bool reDistribute(ARVOREB *tree, int fullPageRRN){
	// Retorna false caso ainda seja preciso avaliar
	return false;
}

int insereArvoreB(ARVOREB *tree, INDEXELEMENT *newElement, int rrn){
	// depth armazena qual o nivel atual na arvore
	static int depth = -1;
	depth++;
	// Obtem-se a localizacao do no atual baseado no seu rrn
	long int location;
	if(depth == 0){
		rrn = getRaiz(tree);
		location = filePos(rrn);
	}else
		location = filePos(rrn);

	// Se foi tentado verificar um rrn igual a -1, retorna -1 e volta ao nivel anterior
	if(location == -1){
		depth--;
		return -1;
	}

	// Caso contrario, le a pagina salva no rrn passado
	fseek(tree->fp, location, SEEK_SET);
	PAGINAB *page = criaPaginaB();
	lePaginaB(tree->fp, page);

	int i, j, result, aux, childrrn;
	bool found;
	result = -1;
	found = false;
	// Checa todos os nos da pagina atual para descobrir onde no ovo elemento deve ser
	// inserido
	for(i = 0; (i < page->n) && !found; i++){
		aux = checkElement(page->nos[i], getId(newElement));
		if(aux == 0){
			// Caso o elemento ja exista, exibe mensagem de erro e retorna -2
			fprintf(stderr, "insereArvoreB(): Esse elemento ja existe\n");
			apagaPaginaB(&page);
			deleteIdxElement(&newElement);
			depth--;
			return -2;
		}else if(aux < 0){
			// Ao encontrar a posicao onde deve ser salvo, armazena o ponteiro correspondente
			found = true;
			childrrn = page->ponteiros[i];
		}
	}
	// Ao final do for, a variavel i armazena a posicao do ponteiro onde a funcao deve descer
	if(!found)
		childrrn = page->ponteiros[i];

	// Se chama recursivamente para avaliar a posicao encontrada
	result = insereArvoreB(tree, newElement, childrrn);
	// Caso o no atual seja uma folha, o retorno sera -1
	if(result == -1){
		// A inserçao ocorre aqui
		// Caso haja espaço no no atual
		if(page->n < ORDEM_PAGINA){
			// Insere o novo elemento na pagina
			j = page->n-1;
			page->nos[page->n++] = newElement;
			while((checkElement(page->nos[j], getId(newElement)) < 0) && j >= 0){
				swap(page->nos, j, j+1);
				j--;
			}
			// Atualiza a pagina alterada no arquivo
			fseek(tree->fp, filePos(rrn), SEEK_SET);
			escrevePaginaB(tree->fp, page);
			apagaPaginaB(&page);
			depth--;
			return 0;
		}else{
		// Caso nao haja espaco no no atual
			//Insere o novo elemento
			page->n++;
			page->nos = (INDEXELEMENT**)realloc(page->nos, sizeof(INDEXELEMENT*) * page->n);
			page->nos[page->n-1] = newElement;
			page->ponteiros = (int*)realloc(page->ponteiros, sizeof(int) * (page->n + 1));
			page->ponteiros[page->n] = -1;
			// Ordena os elementos
			j = page->n-1;
			while((checkElement(page->nos[j], getId(newElement)) < 0) && j >= 0){
				swap(page->nos, j, j+1);
				j--;
			}
			// E escreve a pagina com tamanho anormal no final do arquivo
			fseek(tree->fp, filePos(tree->size), SEEK_SET);
			escrevePaginaB(tree->fp, page);
			if(depth == 0){
				// se for a raiz, realiza o split 1-to-2 e retorna 0
				splitRaiz(tree);
				apagaPaginaB(&page);
				depth--;
				return 0;
			}else{
				// Indica que o overflow deve ser analisado para redistribuição ou split
				apagaPaginaB(&page);
				depth--;
				return -3;
			}
		}
	}else if(result == -3){
		// overflow deve ser avaliado
		// Se possivel, realiza a redistribuição
		if(reDistribute(tree, i)){
			// Caso tenha sido resolvido com a redistribuicao, libera memoria e retorna 0
			depth--;
			apagaPaginaB(&page);
			return 0;
		}else{
			// Se nao for possivel, faz o split 2-to-3
			if(split2to3(tree, i)){
				// Caso o split nao gere overflow, libera memoria e retorna 0
				depth--;
				apagaPaginaB(&page);
				return 0;
			}else{
				//Caso gere overflow e nao for raiz, libera memoria e retorna -3
				if(depth > 0){
					depth--;
					apagaPaginaB(&page);
					return -3;
				}else{
				//Se for a raiz, realiza o split 1-to-2 e retorna 0
					splitRaiz(tree);
					depth--;
					apagaPaginaB(&page);
					return 0;
				}
			}
		}
	}else if(result == -2 || result == 0){
		// deu ruim ou deu bom
		depth--;
		apagaPaginaB(&page);
		return result;
	}else{
		fprintf(stderr, "insereArvoreB(): Valor de retorno invalido detectado\n");
		depth--;
		apagaPaginaB(&page);
		return -2;
	}
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
		page->ponteiros = (int*)malloc(sizeof(int) * ORDEM_PAGINA);
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

// Funcao auxiliar para acomodar o caso em que uma pagina com overflow foi escrita no arquivo
int max(int a, int b){
	return (a > b)? a : b;
}

void lePaginaB(FILE *fp, PAGINAB *page){
	if(fp != NULL && page != NULL){
		int i;
		fread(&page->n, sizeof(unsigned char), 1, fp);
		if(page->n > (ORDEM_PAGINA - 1)){
			page->ponteiros = (int*)realloc(page->ponteiros, sizeof(int) * page->n);
			page->nos = (INDEXELEMENT**)realloc(page->nos, sizeof(INDEXELEMENT*) * (page->n + 1));
		}
		for(i = 0; i < max(ORDEM_PAGINA - 1, page->n); i++)
			readIdxElement(page->nos[i], fp);
		for(i = 0; i < max(ORDEM_PAGINA, page->n + 1); i++)
			fread(&page->ponteiros[i], sizeof(int), 1, fp);
	}else fprintf(stderr, "lePaginaB(): Parametro invalido passado\n");
}

void escrevePaginaB(FILE *fp, PAGINAB *page){
	if(fp != NULL && page != NULL){
		int i;
		fwrite(&page->n, sizeof(unsigned char), 1, fp);
		for(i = 0; i < max(ORDEM_PAGINA - 1, page->n); i++)
			writeIdxElement(page->nos[i], fp);
		for(i = 0; i < max(ORDEM_PAGINA, page->n + 1); i++)
			fwrite(&page->ponteiros[i], sizeof(int), 1, fp);
	}else fprintf(stderr, "escrevePaginaB(): Parametro invalido passado\n");
}

void apagaPaginaB(PAGINAB **page){
	if(page != NULL && *page != NULL){
		int i;
		if((*page)->nos != NULL){
			for(i = 0; i < (*page)->n; i++)
				deleteIdxElement(&((*page)->nos[i]));
		}
		if((*page)->ponteiros != NULL)
			free((*page)->ponteiros);
		free(*page);
		*page = NULL;
	}else fprintf(stderr, "apagaPaginaB(): Parametro invalido passado\n");
}
