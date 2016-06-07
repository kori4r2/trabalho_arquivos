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
	}
	return newElement;
}

void readIdxElement(INDEXELEMENT *idxEl, FILE *fp){
	if(idxEl != NULL && fp != NULL){
		fread(&idxEl->id, sizeof(unsigned int), 1, fp);
		fread(&idxEl->offset, sizeof(long int), 1, fp);
	}
}

void writeIdxElement(INDEXELEMENT *idxEl, FILE *fp){
	if(idxEl != NULL && fp != NULL){
		fwrite(&idxEl->id, sizeof(unsigned int), 1, fp);
		fwrite(&idxEl->offset, sizeof(long int), 1, fp);
	}
}

int checkElement(INDEXELEMENT *idxEl, unsigned int id){
	if(idxEl != NULL){
		return (id - idxEl->id);
	}
	fprintf(stderr, "checkElement(): invalid parameters\n");
	return 0;
}

long int getOffset(INDEXELEMENT *idxEl){
	if(idxEl != NULL){
		return idxEl->offset;
	}
	return -1;
}

void deleteElement(INDEXELEMENT **idxEl){
	if(idxEl != NULL && *idxEl != NULL){
		free(*idxEl);
		*idxEl = NULL;
	}
}
