
#include <stdlib.h>
#include <stdio.h>
#include "Config.h"

//simula um banco de dados com suas operações, paginas e etc...

float totalOperations = TOTAL_OPERATIONS;
float totalWriteOperationsPercent = TOTAL_WRITE_OPERATIONS_PERCENT;

float totalReadOperations;
float totalWriteOperations;

float totalReadOperationsRemaining;
float totalWriteOperationsRemaining;

int read_count_remaining = 0;
int write_count_remaining = 0;

int cont = 0;
int burstState = 0;
char opType;
int i, pageId;
int pageIDModeBurst = 0;

int totalPages = NUMBER_OF_PAGES;


// Get Random Number
int random_in_range(unsigned int min, unsigned int max) {
	int base_random = rand(); /* in [0, RAND_MAX] */
	if (RAND_MAX == base_random)
		return random_in_range(min, max);
	/* now guaranteed to be in [0, RAND_MAX) */
	int range = max - min, remainder = RAND_MAX % range, bucket = RAND_MAX
			/ range;
	/* There are range buckets, plus one smaller interval
	 within remainder of RAND_MAX */
	if (base_random < RAND_MAX - remainder) {
		return min + base_random / bucket;
	} else {
		return random_in_range(min, max);
	}
}

unsigned int randr(unsigned int min, unsigned int max) {
	double scaled = (double) rand() / RAND_MAX;
	return (max - min + 1) * scaled + min;
}

int getRandomNumber(int minRange, int maxRange, int codRandomGenerator) {
	switch (codRandomGenerator) {
	case 1:
		return randr(minRange, maxRange);
		break;
	case 2:
		return random_in_range(minRange, maxRange);
		break;
	}
	return 0;
}

float getTotalOperations(float totalOperations,
		float totalWriteOperationsPercent, char operationType) {

	float totalWriteOperations = totalOperations
			* (totalWriteOperationsPercent / 100);
	float totalReadOperations = totalOperations - totalWriteOperations;

	if (operationType == 'W') {
		return totalWriteOperations;
	} else if (operationType == 'R') {
		return totalReadOperations;
	}
	return 0;

}

char getOperationType(int writeAffinity) {
	int num;
	char opType;
	num = rand() % 100;
	if (num <= writeAffinity) {
		opType = 'W';
	} else
		opType = 'R';

	return opType;
}

void initRemainingOperations() {
	totalWriteOperations = (int) getTotalOperations(totalOperations,
			totalWriteOperationsPercent, 'W');
	totalReadOperations = (int) getTotalOperations(totalOperations,
			totalWriteOperationsPercent, 'R');

	read_count_remaining = totalReadOperations;
	write_count_remaining = totalWriteOperations;
}

int res;
void printAndInsert(int* chosenPageId, char* chosenOpType, char opType, int pageId) {

	res = insere((int)pageId, opType);

	if(DEBUG_MODE == 1){
	if(res == 1){
		printf("HIT ");
	}else if (res == -1){
		printf("MISS ");
	}

	printf("[%c(%i)][Remaining %iW-%iR]", opType, pageId,(int) write_count_remaining, (int) read_count_remaining);

	imprime();

	printf("\n");
	}
	if (read_count_remaining == 0 || write_count_remaining == 0) {
		burstState = 0;
		cont = 0;
	}

	*chosenOpType = opType;
	*chosenPageId = pageId;
}

int generateOperations(int* chosenPageId, char* chosenOpType, char flagBurst) {

	totalWriteOperations = (int) getTotalOperations(totalOperations,
			totalWriteOperationsPercent, 'W');
	totalReadOperations = (int) getTotalOperations(totalOperations,
			totalWriteOperationsPercent, 'R');

	if (burstState == 1) {
		pageIDModeBurst++;
		pageId = pageIDModeBurst;
	} else {
		pageId = getRandomNumber(0, totalPages, 2);
	}

	opType = getOperationType(totalWriteOperationsPercent);

	if (flagBurst == 'T' && cont != 0) {
		if (TYPE_BURST == 'W') {
			if (((write_count_remaining < OPERATIONS_BURST_LENGHT)
					&& burstState == 0)) {


				if (read_count_remaining == 0) {
					opType = 'W';
				} else if (write_count_remaining == 0) {
					opType = 'R';
				}
				if (opType == 'R') {
					read_count_remaining--;
				} else if (opType == 'W') {
					write_count_remaining--;
				}
				printAndInsert(chosenPageId, chosenOpType, opType, pageId);
			} else {
				write_count_remaining--;
				printAndInsert(chosenPageId, chosenOpType, TYPE_BURST, pageId);
			}
		} else if (TYPE_BURST == 'R') {
			if (((read_count_remaining < OPERATIONS_BURST_LENGHT)
					&& burstState == 0)) {

							if (read_count_remaining == 0) {
					opType = 'W';
				} else if (write_count_remaining == 0) {
					opType = 'R';
				}
				if (opType == 'R') {
					read_count_remaining--;
				} else if (opType == 'W') {
					write_count_remaining--;
				}
				printAndInsert(chosenPageId, chosenOpType, opType, pageId);
			} else {
				read_count_remaining--;
				printAndInsert(chosenPageId, chosenOpType, TYPE_BURST, pageId);
			}
		}
	} else {
		if (read_count_remaining == 0) {
			opType = 'W';
		} else if (write_count_remaining == 0) {
			opType = 'R';
		}

		if (opType == 'R') {
			read_count_remaining--;
		} else if (opType == 'W') {
			write_count_remaining--;
		}
		printAndInsert(chosenPageId, chosenOpType, opType, pageId);
	}
	return EXIT_SUCCESS;

struct OP{
	char opTYPE;
	int pageID;
	struct OP *next;
};

struct OPlist{
	struct OP *head;
	struct OP *tail;
};


void insertOP( int opTYPE, int pageID, struct OPlist* lista){
	struct OP* op = (struct OP*)malloc(sizeof(struct OP));
	op->pageID = pageID;
	op->opTYPE = opTYPE;

	if(lista->head == NULL){
		lista->head = lista->tail = op;
	}else{
		lista->tail->next = op;
		lista->tail = op;
	}
}

void printOP( struct OP *p)
{

	while(p != NULL){
      printf( "%d\n", p->pageID);
      p = p->next;
	}

};
struct OPlist*  loadData(char *nome_do_arquivo){
	FILE * file;
	file = fopen(nome_do_arquivo,"r");
	if(file == NULL){
		printf("File not found");
	}
	char op2;
	int pageID;
	struct OPlist* list = (struct OPlist*)malloc(sizeof(struct OPlist));

	while(fscanf(file,"%c;%d\n",&op2,&pageID) > 0){

		insertOP(op2,pageID,list);
	}

	fclose(file);
	return list;
}



char burstTest(float i, float operations_burst_start) {

	float valor = i / operations_burst_start;
	float partefracionaria = valor - (int) valor;

	if (((int) i % (int) operations_burst_start == 0 && partefracionaria == 0)
			&& i != 0) {
		if (burstState != 1) {
			cont = -1;
		}
		return 'T';
	} else
		return 'F';
}

void execute(char *fileName,int capacity) {

	inicializa(bytes, capacity);
	if(DATA_FILES == 0){
	int i;
	char* randOP = malloc(sizeof(char));
	int* randGID = malloc(sizeof(int));

	//srand( (int)(time(NULL)));

	initRemainingOperations();



	for (i = 1; i <= TOTAL_OPERATIONS; i++) {
		if(DEBUG_MODE == 1){
		printf("[OP %i]\t[BURST: %i]\t", i, burstState);

		}

		char resultTest = burstTest(i, OPERATIONS_BURST_START);
		if ((resultTest == 'T' && cont != 0) || cont != 0) {

			if (resultTest == 'T' && cont == -1) {

				if ((TYPE_BURST == 'R' && read_count_remaining == 0)
						|| (TYPE_BURST == 'R' && write_count_remaining == 0)) {
					cont = 0;
				} else if ((TYPE_BURST == 'W' && read_count_remaining == 0)
						|| (TYPE_BURST == 'W' && write_count_remaining == 0)) {
					cont = 0;
				} else {
					cont = OPERATIONS_BURST_LENGHT;
					burstState = 1;
					pageIDModeBurst = getRandomNumber(0, totalPages, 2);
				}
			}

			generateOperations(randGID, randOP, 'T');

			if (burstState == 1) {
				cont--;
			}

			if (burstState == 1 && cont == 0) {
				burstState = 0;
				pageIDModeBurst = -1;
			}
		} else {
			generateOperations(randGID, randOP, 'F');
		}
	}

	}else{
		struct OP *lo = loadData(fileName)->head;

		while(lo != NULL){
			if(DEBUG_MODE == 1){
			printf("[%c(%d)]",lo->opTYPE, lo->pageID);

			}

			insere((int) lo->pageID, lo->opTYPE);
			if(DEBUG_MODE == 1){
				imprime();
				printf("\n");
			}
			lo = lo->next;
		}

	}
		//_print_report();

}}
