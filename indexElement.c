#include "indexElement.h"

struct idxElement{
	unsigned int id;
	long int offset;
};

INDEXELEMENT *createIdxElement(unsigned int id, long int offset){
	INDEXELEMENT *newElement = (INDEXELEMENT*)malloc(sizeof(INDEXELEMENT));
	if(newElement != NULL){
		newElement->id = id;
		newElement->offset = offset;
	}else fprintf(stderr, "createIdxElement(): Erro de alocacao de memoria\n");
	return newElement;
}

void readIdxElement(INDEXELEMENT *idxEl, FILE *fp){
	if(idxEl != NULL && fp != NULL){
		fread(&idxEl->id, sizeof(unsigned int), 1, fp);
		fread(&idxEl->offset, sizeof(long int), 1, fp);
	}else fprintf(stderr, "readIdxElement(): Parametro invalido passado\n");
}

void writeIdxElement(INDEXELEMENT *idxEl, FILE *fp){
	if(idxEl != NULL && fp != NULL){
		fwrite(&idxEl->id, sizeof(unsigned int), 1, fp);
		fwrite(&idxEl->offset, sizeof(long int), 1, fp);
	} else fprintf(stderr, "writeIdxElement(): Parametro invalido passado\n");
}

int checkElement(INDEXELEMENT *idxEl, unsigned int id){
	if(idxEl != NULL){
		return (id - idxEl->id);
	}
	fprintf(stderr, "checkElement(): Parametro invalido passado\n");
	return 0;
}

unsigned int getId(INDEXELEMENT *idxEl){
	if(idxEl != NULL){
		return idxEl->id;
	}
	fprintf(stderr, "getId(): Parametro invalido passado\n");
	return 0;
}

long int getOffset(INDEXELEMENT *idxEl){
	if(idxEl != NULL){
		return idxEl->offset;
	}
	fprintf(stderr, "getOffset(): Parametro invalido passado\n");
	return -1;
}

void deleteIdxElement(INDEXELEMENT **idxEl){
	if(idxEl != NULL && *idxEl != NULL){
		free(*idxEl);
		*idxEl = NULL;
	}else fprintf(stderr, "deleteIdxElement(): Parametro invalido passado\n");
}
