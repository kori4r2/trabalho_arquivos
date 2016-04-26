// Alisson Mateus de Oliveira Magalhães - nUSP 8066287
// Ítalo Tobler Silva - nUSP 8551910

#include "myStrings.h"

// Aloca a memoria necessaria para criar uma copia da string passada como argumento
char *myStrdup(const char *string){
	if(string == NULL) return NULL;
	char *duplicate = (char*)malloc(sizeof(char) * (strlen(string) + 1));
	if(duplicate == NULL) return NULL;
	strcpy(duplicate, string);
	return duplicate;
}

// Le uma palavra da stream de dados passada. Começa a ler no primeiro caractere alfanumerico.
// As condicoes de parada sao espaço, quebra de linha e fim de arquivo.
char *getWord(FILE *stream){
	if(stream == NULL) return NULL;
	char input;
	char *word = NULL;
	int size = 0;

	do{
		input = fgetc(stream);
	}while(input != EOF && !isalnum(input));
	if(input == EOF){
		return NULL;
	}
	do{
		word = (char*)realloc(word, sizeof(char) * (size+1));
		word[size++] = input;
		input = fgetc(stream);
	}while(input != ' ' && input != '\n' && input != EOF);
	word = (char*)realloc(word, sizeof(char) * (size+1));
	word[size] = '\0';
	return word;
}

// Funciona da mesma maneira que getWord(), mas nao tem o espaco como condicao de parada
char *myGetLine(FILE *stream){
	if(stream == NULL) return NULL;
	char input;
	char *string = NULL;
	int size = 0;

	do{
		input = fgetc(stream);
	}while(input != EOF && !isalnum(input));
	if(input == EOF){
		return NULL;
	}
	do{
		string = (char*)realloc(string, sizeof(char) * (size+1));
		string[size++] = input;
		input = fgetc(stream);
	}while(input != '\n' && input != EOF);
	string= (char*)realloc(string, sizeof(char) * (size+1));
	string[size] = '\0';
	return string;
}

// Funciona da mesma maneira que myGetLine, mas possui um tamanho maximo e aloca a string com esse tamanho obrigatoriamente
char *readLimitedString(FILE *stream, int maxSize){
	if(stream == NULL || maxSize < 1) return NULL;
	char *string = (char*)malloc(sizeof(char) * maxSize);
	memset(string, 0, sizeof(char) * maxSize);
	if(string == NULL){
		fprintf(stderr, "readLimitedString(): error allocating memory\n");
		return NULL;
	}
	char input;
	int size = 0;

	do{
		input = fgetc(stream);
	}while(input != EOF && !isalnum(input));
	if(input == EOF){
		return NULL;
	}
	do{
		if(size < maxSize-1) string[size++] = input;
		input = fgetc(stream);
	}while(input != '\n' && input != EOF);
	string[size] = '\0';
	return string;
}
