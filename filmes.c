// Alisson Mateus de Oliveira Magalhães - nUSP 8066287
// Ítalo Tobler Silva - nUSP 8551910
// Pedro Henrique Siqueira de Oliveira - nUSP 8922006

#include "filmes.h"

#define FIM_CAMPO '|'
#define FIM_REGISTRO '^'

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
	int *idList;
	char *filename;
};

// Funcoes de filme-------------------------------------------------------------------------------------------

FILME *criaFilme(int ano, int dur, const char *titulo, const char *descr, const char *pais, const char *genero){
	// Checa parametros
	if(ano > 0 && dur > 0 && titulo != NULL && descr != NULL && pais != NULL && genero != NULL){
		// Aloca memoria
		FILME *novoFilme = (FILME*)malloc(sizeof(FILME));
		// Armazena informacoes
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
	// Le os campos te tamanho fixo
	fread(&filme->id, sizeof(unsigned int), 1, fp);
	if(feof(fp)) return 0;
	fread(&filme->ano, sizeof(int), 1, fp);
	fread(&filme->dur, sizeof(int), 1, fp);
	// Le e armazena um campo variavel contendo uma string
	i = 0;
	do{
		filme->titulo = (char*)realloc(filme->titulo, sizeof(char) * (i+1));
		fread(filme->titulo+i, sizeof(char), 1, fp);
	}while(filme->titulo[i++] != FIM_CAMPO);
	filme->titulo[i-1] = '\0';
	// Repete o procedimento para os outros 3 campos
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
	// Le um caractere e compara com o caractere indicador de fim de registro
	fread(&fimRegistro, sizeof(char), 1, fp);
	// Se for diferente, indica que houve erro na organizacao do arquivo
	if(fimRegistro != FIM_REGISTRO)
		fprintf(stderr, "leFilme: erro na organizacao do arquivo(%c, not %c)\n", fimRegistro, FIM_REGISTRO);

	return 1;
}

void escreveFilme(FILE *fp, FILME *filme){
	int i;
	char fimCampo = FIM_CAMPO, fimRegistro = FIM_REGISTRO;
	// Escreve no arquivo todos os campos do filme
	fwrite(&filme->id, sizeof(unsigned int), 1, fp);
	fwrite(&filme->ano, sizeof(int), 1, fp);
	fwrite(&filme->dur, sizeof(int), 1, fp);
	// No caso das strings substitui o '\0' pelo caractere indicador de fim de campo
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
	// Escreve o caractere indicador de fim de registro
	fwrite(&fimRegistro, sizeof(char), 1, fp);
}

void esvaziaFilme(FILME *filme){
	if(filme != NULL){
		// Transforma todos os campos em 0 ou NULL, liberando memoria alocada
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
		printf("id       : %03d\n", filme->id);
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
		int i;
		CATALOGO *novoCatalogo = (CATALOGO*)malloc(sizeof(CATALOGO));
		if(novoCatalogo != NULL){
			novoCatalogo->n = 0;
			// O vetor idList armazena o id dos filmes que serao inseridos
			novoCatalogo->idList = (int*)malloc(sizeof(int) * 100);
			if(novoCatalogo->idList == NULL){
				apagaCatalogo(&novoCatalogo);
				fprintf(stderr, "criaCatalogo: erro de alocacao de memoria\n");
			}
			// Os valores do vetor sao inicializados
			for(i = 0; i < 100; i++)
				novoCatalogo->idList[i] = (i+1);
			// E embaralhados
			shuffle(novoCatalogo->idList, 100);
			novoCatalogo->filename = myStrdup(nomeArquivo);
			// Assim que salva o nome de arquivo, tenta criar um novo arquivo, apagando qualquer
			// arquivo anterior de mesmo nome
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
	// Checa se os parametros sao validos e tenta abrir o arquivo de dados
	if(catalogo != NULL && filme != NULL && *filme != NULL){
		FILE *fp = fopen(catalogo->filename, "ab");
		if(fp == NULL)
			fprintf(stderr, "insereFilme: erro ao abrir o arquivo\n");
		else{
			// Determina o id do filme sendo salvo e
			// incrementa o contador do catalogo
			(*filme)->id = catalogo->idList[(catalogo->n++)%100];
			// Escreve as informacoes no arquivo e apaga o filme
			escreveFilme(fp, *filme);
			apagaFilme(filme);
			fclose(fp);
		}
	}else fprintf(stderr, "insereFilme: parametro invalido passado\n");
}

void imprimeCatalogo(CATALOGO *catalogo){
	if(catalogo != NULL){
		// Cria um filme vazio
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
			// Le todos os filmes salvos no arquivo
			while(leFilme(fp, filme) != 0){
				// Imprime as informacoes e esvazia o filme novamente
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
		// offset armazena a posicao do filme buscado
		long int offset = 0;
		int curId, aux;
		char caractere;
		// Cria um filme vazio
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
			// Le o id de todos os filmes com id diferente do desejado no arquivo
			while((fread(&curId, sizeof(unsigned int), 1, fp)) > 0 && curId != id){
				// Tendo lido um filme diferente do procurado pula os proximos dois
				// campos de tamanho fixo
				fread(&aux, sizeof(int), 1, fp);
				fread(&aux, sizeof(int), 1, fp);
				// Incrementa offset
				offset += sizeof(unsigned int) + (2 * sizeof(int));
				// Le todos os caracteres ate o fim de campo ser encontrado
				// para pular o campo de tamanho variavel para os 4 campos
				do{
					fread(&caractere, sizeof(char), 1, fp);
					// Incrementa o offset enquanto le
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
				// Le um ultimo caractere que deve ser o indicador de fim de registro
				fread(&caractere, sizeof(char), 1, fp);
				offset++;
				// Caso nao seja, indica o erro na organizacao do arquivo
				if(caractere != FIM_REGISTRO){
					fprintf(stderr, "procuraFilme: erro na organizacao do arquivo (%c, not %c)\n", caractere, FIM_REGISTRO);
					apagaFilme(&filme);
					fclose(fp);
					return;
				}
			}
			// Se a busca terminar e o filme for encontrado
			if(!feof(fp) && curId == id){
				// Imprime o filme desejado
				fseek(fp, offset, SEEK_SET);
				leFilme(fp, filme);
				imprimeFilme(filme);
			}else{
				// Se o filme nao for econtrado, avisa o usuario
				printf("Filme nao encontrado\n");
			}
			fclose(fp);
		}
		apagaFilme(&filme);
	}else fprintf(stderr, "procuraFilme: parametro invalido passado\n");
}

void shuffle(int* aux_shuffle, int numFilmes){
	int i, j, t;
	// Troca cada posicao do vetor com uma posicao aleatoria
	for (i = 0; i < numFilmes - 1; i++) {
		j = (rand() % (numFilmes-1));
		t = aux_shuffle[j];
		aux_shuffle[j] = aux_shuffle[i];
		aux_shuffle[i] = t;
	}
}
