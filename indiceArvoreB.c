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
		}else fprintf(stderr, "criaArvoreB(): Erro de alocacao de memoria\n");
		return tree;
	}
	fprintf(stderr, "criaArvoreB(): Parametro invalido passado\n");
	return NULL;
}

void apagaArvoreB(ARVOREB **tree){
	if(tree != NULL && *tree != NULL){
		if((*tree)->filename != NULL)
			free((*tree)->filename);
		if((*tree)->fp != NULL)
			fclose((*tree)->fp);
		free(*tree);
		*tree = NULL;
	}else fprintf(stderr, "apagaArvoreB(): Parametro invalido passado\n");
}

int getRaiz(ARVOREB *tree){
	if(tree != NULL){
		long int raiz = -1;
		if(tree->fp == NULL){
			// Caso o indice estiver fechado
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
	int i, newRootRRN, pageRRN, oldRootRRN;

	oldRootRRN = getRaiz(tree);
	PAGINAB *oldRoot = criaPaginaB();
	fseek(tree->fp, filePos(tree->size), SEEK_SET);
	lePaginaB(tree->fp, oldRoot);

	newRootRRN = tree->size++;
	pageRRN = tree->size++;
	PAGINAB *newRoot = criaPaginaB();
	PAGINAB *page = criaPaginaB();

	// Copia a segunda metade da raiz antiga para a nova pagina
	for(i = 0; i < (ORDEM_PAGINA-1) / 2; i++){
		page->nos[i] = oldRoot->nos[((ORDEM_PAGINA-1) / 2) + 1 + i];
		oldRoot->nos[((ORDEM_PAGINA-1) / 2) + 1 + i] = NULL;
		page->ponteiros[i] = oldRoot->ponteiros[((ORDEM_PAGINA-1) / 2) + 1 + i];
		oldRoot->ponteiros[((ORDEM_PAGINA-1) / 2) + 1 + i] = -1;
	}
	page->ponteiros[i] = oldRoot->ponteiros[((ORDEM_PAGINA-1) / 2) + 1 + i];
	oldRoot->ponteiros[((ORDEM_PAGINA-1) / 2) + 1 + i] = -1;
	// Promove o elemento intermendiario
	newRoot->nos[0] = oldRoot->nos[(ORDEM_PAGINA-1) / 2];
	oldRoot->nos[(ORDEM_PAGINA-1) / 2] = NULL;
	// Atualiza os ponteiros da nova raiz
	newRoot->ponteiros[0] = oldRootRRN;
	newRoot->ponteiros[1] = pageRRN;
	// Atualiza os tamanhos das paginas
	newRoot->n = 1;
	oldRoot->n = (ORDEM_PAGINA-1) / 2;
	page->n = (ORDEM_PAGINA-1) / 2;
	// Atualiza a raiz
	setRaiz(tree, newRootRRN);
	// Escreve as paginas em suas respectivas posicoes
	fseek(tree->fp, filePos(oldRootRRN), SEEK_SET);
	escrevePaginaB(tree->fp, oldRoot);
	fseek(tree->fp, filePos(pageRRN), SEEK_SET);
	escrevePaginaB(tree->fp, page);
	fseek(tree->fp, filePos(newRootRRN), SEEK_SET);
	escrevePaginaB(tree->fp, newRoot);

	// Libera a memoria alocada
	apagaPaginaB(&oldRoot);
	apagaPaginaB(&page);
	apagaPaginaB(&newRoot);
}

bool split2to3(ARVOREB *tree, PAGINAB *parent, int parentRRN, int fullPageRRN){
	int i, newRRN = tree->size;
	INDEXELEMENT *promotedElement;
	PAGINAB *page = criaPaginaB();
	PAGINAB *newPage = criaPaginaB();
	PAGINAB *fullPage = criaPaginaB();
	fseek(tree->fp, filePos(tree->size), SEEK_SET);
	lePaginaB(tree->fp, fullPage);
	tree->size++;
	if(fullPageRRN > 0){
		// Se possivel, realiza o split com a pagina a sua esquerda
		fseek(tree->fp, filePos(parent->ponteiros[fullPageRRN - 1]), SEEK_SET);
		lePaginaB(tree->fp, page);
		// Copia os elementos da pagina irma para a nova pagina, com excecao do que sera promovido
		for(i = 0; i < (((ORDEM_PAGINA-1) / 3) - 1); i++){
			newPage->nos[i] = page->nos[(2 * (ORDEM_PAGINA-1) / 3) + 1 + i];
			page->nos[(2 * (ORDEM_PAGINA-1) / 3) + 1 + i] = NULL;
			newPage->ponteiros[i] = page->ponteiros[(2 * (ORDEM_PAGINA-1) / 3) + 1 + i];
			page->ponteiros[(2 * (ORDEM_PAGINA-1) / 3) + 1 + i] = -1;
		}
		newPage->ponteiros[i] = page->ponteiros[(2 * (ORDEM_PAGINA-1) / 3) + 1 + i];
		page->ponteiros[(2 * (ORDEM_PAGINA-1) / 3) + 1 + i] = -1;
		// Copia o elemento do pai ,que sera substituido pelo elemento promovido da irma, para a nova pagina
		newPage->nos[((ORDEM_PAGINA-1) / 3) - 1] = parent->nos[fullPageRRN - 1];
		// Promove o elemento da pagina irma
		parent->nos[fullPageRRN - 1] = page->nos[2 * (ORDEM_PAGINA-1) / 3];
		page->nos[2 * (ORDEM_PAGINA-1) / 3] = NULL;
		// Copia os elementos da pagina cheia para a pagina irma
		for(i = 0; i < ((ORDEM_PAGINA-1) / 3); i++){
			newPage->nos[((ORDEM_PAGINA-1) / 3) + i] = fullPage->nos[i];
			newPage->ponteiros[((ORDEM_PAGINA-1) / 3) + i] = fullPage->ponteiros[i];
		}
		newPage->ponteiros[((ORDEM_PAGINA-1) / 3) + i] = fullPage->ponteiros[i];
		// Armazena qual o elemento que deve ser promovido
		promotedElement = fullPage->nos[i];
		fullPage->nos[i] = NULL;
		// Move os elementos que permanecerao na pagina para o inicio da mesma
		for(i = 0; i < (2 * (ORDEM_PAGINA-1) / 3); i++){
			fullPage->nos[i] = fullPage->nos[((ORDEM_PAGINA+1) / 3) + 1 + i];
			fullPage->nos[((ORDEM_PAGINA+1) / 3) + 1 + i] = NULL;
			fullPage->ponteiros[i] = fullPage->ponteiros[((ORDEM_PAGINA+1) / 3) + 1 + i];
			fullPage->ponteiros[((ORDEM_PAGINA+1) / 3) + 1 + i] = -1;
		}
		fullPage->ponteiros[i] = fullPage->ponteiros[((ORDEM_PAGINA+1) / 3) + 1 + i];
		fullPage->ponteiros[((ORDEM_PAGINA+1) / 3) + 1 + i] = -1;
		// Volta a pagina ao tamanho normal
		fullPage->nos = (INDEXELEMENT**)realloc(fullPage->nos, sizeof(INDEXELEMENT*) * (ORDEM_PAGINA-1));
		fullPage->ponteiros = (int*)realloc(fullPage->ponteiros, sizeof(int) * ORDEM_PAGINA);
		// Atualiza o tamanho das 3 paginas
		fullPage->n = 2 * (ORDEM_PAGINA-1) / 3;
		page->n = 2 * (ORDEM_PAGINA-1) / 3;
		newPage->n = 2 * (ORDEM_PAGINA-1) / 3;
		// Escreve as paginas alteradas e criadas
		fseek(tree->fp, filePos(parent->ponteiros[fullPageRRN - 1]), SEEK_SET);
		escrevePaginaB(tree->fp, page);
		fseek(tree->fp, filePos(parent->ponteiros[fullPageRRN]), SEEK_SET);
		escrevePaginaB(tree->fp, fullPage);
		fseek(tree->fp, filePos(newRRN), SEEK_SET);
		escrevePaginaB(tree->fp, newPage);
		// Realiza a promocao do elemento salvo (pode causar overflow)
		if(parent->n == ORDEM_PAGINA-1){
			// Se necessario, aumenta o tamanho dos vetores
			parent->nos = (INDEXELEMENT**)realloc(parent->nos, sizeof(INDEXELEMENT*) * ORDEM_PAGINA);
			parent->ponteiros = (int*)realloc(parent->ponteiros, sizeof(int) * (ORDEM_PAGINA+1));
		}
		// Movimenta elementos e ponteiros para a direita para ter espaço para o novo elemento
		parent->ponteiros[parent->n+1] = parent->ponteiros[parent->n];
		for(i = parent->n; i > fullPageRRN; i--){
			parent->nos[i-1] = parent->nos[i];
			parent->ponteiros[i-1] = parent->ponteiros[i];
		}
		// Adiciona o novo elemento e o novo ponteiro
		parent->nos[fullPageRRN] = promotedElement;
		parent->ponteiros[fullPageRRN] = newRRN;
		parent->n++;
		// Libera memoria alocada
		apagaPaginaB(&page);
		apagaPaginaB(&fullPage);
		apagaPaginaB(&newPage);
		// Se causou overflow escreve a pagina anomala no fim do arquivo e retorna false
		if(parent->n > (ORDEM_PAGINA-1)){
			fseek(tree->fp, filePos(tree->size), SEEK_SET);
			escrevePaginaB(tree->fp, parent);
			return false;
		// Caso contrario retorna true
		}else{
			return true;
		}
	}else{
		// Caso contrario, faz o split com a irma a direita
		fseek(tree->fp, filePos(parent->ponteiros[fullPageRRN + 1]), SEEK_SET);
		lePaginaB(tree->fp, page);

		// Copia os elementos da pagina cheia para a nova pagina, com excecao do elemento que sera promovido
		for(i = 0; i < ((ORDEM_PAGINA-1) / 3); i++){
			newPage->nos[i] = fullPage->nos[(2 * (ORDEM_PAGINA-1) / 3) + 1 + i];
			fullPage->nos[(2 * (ORDEM_PAGINA-1) / 3) + 1 + i] = NULL;
			newPage->ponteiros[i] = fullPage->ponteiros[(2 * (ORDEM_PAGINA-1) / 3) + 1 + i];
			fullPage->ponteiros[(2 * (ORDEM_PAGINA-1) / 3) + 1 + i] = -1;
		}
		newPage->ponteiros[i] = fullPage->ponteiros[(2 * (ORDEM_PAGINA-1) / 3) + 1 + i];
		fullPage->ponteiros[(2 * (ORDEM_PAGINA-1) / 3) + 1 + i] = -1;
		// Copia o elemento da pagina pai que sera substituida pelo elemento promovido
		newPage->nos[((ORDEM_PAGINA-1) / 3)] = parent->nos[fullPageRRN];
		// Promove o elemento da pagina cheia
		parent->nos[fullPageRRN] = fullPage->nos[2 * (ORDEM_PAGINA-1) / 3];
		fullPage->nos[2 * (ORDEM_PAGINA-1) / 3] = NULL;
		// Ajusta o tamanho da pagina cheia
		fullPage->nos = (INDEXELEMENT**)realloc(fullPage->nos, sizeof(INDEXELEMENT*) * (ORDEM_PAGINA-1));
		fullPage->ponteiros = (int*)realloc(fullPage->ponteiros, sizeof(int) * ORDEM_PAGINA);
		// Copia os elementos da pagina irma para a nova pagina, com excecao do elemento promovido
		for(i = 0; i < ((ORDEM_PAGINA-1) / 3) - 1; i++){
			newPage->nos[((ORDEM_PAGINA-1) / 3) + 1 + i] = page->nos[i];
			newPage->ponteiros[((ORDEM_PAGINA-1) / 3) + 1 + i] = page->ponteiros[i];
		}
		newPage->ponteiros[((ORDEM_PAGINA-1) / 3) + 1 + i] = page->ponteiros[i];
		// Armazena o elemento a ser promovido
		promotedElement = page->nos[((ORDEM_PAGINA-1) / 3) - 1];
		// Move todos os elementos da pagina irma para a esquerda
		for(i = 0; i < 2 * (ORDEM_PAGINA-1) / 3; i++){
			page->nos[i] = page->nos[((ORDEM_PAGINA-1) / 3) + i];
			page->nos[((ORDEM_PAGINA-1) / 3) + i] = NULL;
			page->ponteiros[i] = page->ponteiros[((ORDEM_PAGINA-1) / 3) + i];
			page->ponteiros[((ORDEM_PAGINA-1) / 3) + i] = -1;
		}
		page->ponteiros[i] = page->ponteiros[((ORDEM_PAGINA-1) / 3) + i];
		page->ponteiros[((ORDEM_PAGINA-1) / 3) + i] = -1;

		// Atualiza o tamanho das 3 paginas
		fullPage->n = 2 * (ORDEM_PAGINA-1) / 3;
		page->n = 2 * (ORDEM_PAGINA-1) / 3;
		newPage->n = 2 * (ORDEM_PAGINA-1) / 3;
		// Escreve as paginas alteradas e criadas
		fseek(tree->fp, filePos(parent->ponteiros[fullPageRRN - 1]), SEEK_SET);
		escrevePaginaB(tree->fp, page);
		fseek(tree->fp, filePos(parent->ponteiros[fullPageRRN]), SEEK_SET);
		escrevePaginaB(tree->fp, fullPage);
		fseek(tree->fp, filePos(newRRN), SEEK_SET);
		escrevePaginaB(tree->fp, newPage);
		// Realiza a promocao do elemento salvo (pode causar overflow)
		if(parent->n == ORDEM_PAGINA-1){
			// Se necessario, aumenta o tamanho dos vetores
			parent->nos = (INDEXELEMENT**)realloc(parent->nos, sizeof(INDEXELEMENT*) * ORDEM_PAGINA);
			parent->ponteiros = (int*)realloc(parent->ponteiros, sizeof(int) * (ORDEM_PAGINA+1));
		}

		// Movimenta elementos e ponteiros para a direita para ter espaço para o novo elemento
		parent->ponteiros[parent->n+1] = parent->ponteiros[parent->n];
		for(i = parent->n; i > fullPageRRN+1; i--){
			parent->nos[i-1] = parent->nos[i];
			parent->ponteiros[i-1] = parent->ponteiros[i];
		}
		// Adiciona o novo elemento e o novo ponteiro
		parent->nos[fullPageRRN+1] = promotedElement;
		parent->ponteiros[fullPageRRN+1] = newRRN;
		parent->n++;
		// Libera memoria alocada
		apagaPaginaB(&page);
		apagaPaginaB(&fullPage);
		apagaPaginaB(&newPage);
		// Se causou overflow escreve a pagina anomala no fim do arquivo e retorna false
		if(parent->n > (ORDEM_PAGINA-1)){
			fseek(tree->fp, filePos(tree->size), SEEK_SET);
			escrevePaginaB(tree->fp, parent);
			return false;
		// Caso contrario retorna true
		}else{
			return true;
		}
	}
	// Retorna false caso ainda seja preciso avaliar
	return false;
}

bool reDistribute(ARVOREB *tree, PAGINAB *parent, int parentRRN, int fullPageRRN){
	int i;
	PAGINAB *page = criaPaginaB();
	PAGINAB *fullPage = criaPaginaB();
	fseek(tree->fp, filePos(tree->size), SEEK_SET);
	lePaginaB(tree->fp, fullPage);
	// Avalia se e possivel fazer a redistribuicao usando o no vizinho a esquerda
	if(fullPageRRN > 0){
		fseek(tree->fp, filePos(parent->ponteiros[fullPageRRN - 1]), SEEK_SET);
		lePaginaB(tree->fp, page);
		if(page->n < ORDEM_PAGINA  -1){
			// Realiza a redistribuicao
			page->nos[page->n++] = parent->nos[fullPageRRN-1];
			page->ponteiros[page->n] = fullPage->ponteiros[0];
			parent->nos[fullPageRRN-1] = fullPage->nos[0];
			for(i = 0; i < ORDEM_PAGINA; i++){
				fullPage->nos[i] = fullPage->nos[i+1];
				fullPage->ponteiros[i] = fullPage->ponteiros[i+1];
			}
			fullPage->ponteiros[i] = fullPage->ponteiros[i+1];
				// Ajusta o tamanho da pagina anormal
			fullPage->nos = (INDEXELEMENT**)realloc(fullPage->nos, sizeof(INDEXELEMENT*) * (ORDEM_PAGINA - 1));
			fullPage->ponteiros = (int*)realloc(fullPage->ponteiros, sizeof(int) * ORDEM_PAGINA);
			// Reescreve as paginas alteradas
			fseek(tree->fp, filePos(parentRRN), SEEK_SET);
			escrevePaginaB(tree->fp, parent);
			fseek(tree->fp, filePos(parent->ponteiros[fullPageRRN]), SEEK_SET);
			escrevePaginaB(tree->fp, fullPage);
			fseek(tree->fp, filePos(parent->ponteiros[fullPageRRN-1]), SEEK_SET);
			escrevePaginaB(tree->fp, page);
			// Libera a memoria alocada e retorna true;
			apagaPaginaB(&page);
			apagaPaginaB(&fullPage);
			return true;
		}
	}
	// Avalia se e possivel fazer a redistribuicao usando o no vizinho a direita
	if(fullPageRRN < ORDEM_PAGINA - 1){
		fseek(tree->fp, filePos(parent->ponteiros[fullPageRRN+1]), SEEK_SET);
		lePaginaB(tree->fp, page);
		if(page->n < ORDEM_PAGINA - 1){
			// Realiza a redistribuicao
			page->ponteiros[page->n] = page->ponteiros[page->n-1];
			for(i = page->n - 1; i > 0; i--){
				page->nos[i] = page->nos[i-1];
				page->ponteiros[i] = page->ponteiros[i-1];
			}
			page->nos[0] = parent->nos[fullPageRRN-1];
			page->ponteiros[0] = fullPage->ponteiros[fullPage->n--];
			parent->nos[fullPageRRN-1] = fullPage->nos[fullPage->n];
			// Ajusta o tamanho da pagina anormal
			fullPage->nos = (INDEXELEMENT**)realloc(fullPage->nos, sizeof(INDEXELEMENT*) * (ORDEM_PAGINA - 1));
			fullPage->ponteiros = (int*)realloc(fullPage->ponteiros, sizeof(int) * ORDEM_PAGINA);
			// Reescreve as paginas alteradas
			fseek(tree->fp, filePos(parentRRN), SEEK_SET);
			escrevePaginaB(tree->fp, parent);
			fseek(tree->fp, filePos(parent->ponteiros[fullPageRRN]), SEEK_SET);
			escrevePaginaB(tree->fp, fullPage);
			fseek(tree->fp, filePos(parent->ponteiros[fullPageRRN+1]), SEEK_SET);
			escrevePaginaB(tree->fp, page);
			// Libera a memoria alocada e retorna true;
			apagaPaginaB(&page);
			apagaPaginaB(&fullPage);
			return true;
		}
	}
	apagaPaginaB(&page);
	apagaPaginaB(&fullPage);
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
		if(page->n < ORDEM_PAGINA-1){
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
		if(reDistribute(tree, page, rrn, i)){
			// Caso tenha sido resolvido com a redistribuicao, libera memoria e retorna 0
			depth--;
			apagaPaginaB(&page);
			return 0;
		}else{
			// Se nao for possivel, faz o split 2-to-3
			if(split2to3(tree, page, rrn, i)){
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
			page->nos[i] = readIdxElement(fp);
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
			free((*page)->nos);
		}
		if((*page)->ponteiros != NULL)
			free((*page)->ponteiros);
		free(*page);
		*page = NULL;
	}else fprintf(stderr, "apagaPaginaB(): Parametro invalido passado\n");
}
