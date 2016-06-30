#include "LFU.h";

int miss = 0, hit = 0, capacidade = 5, estado;
int writeCounter = 0, readCounter = 0;
struct LFU* lfu;
int bytes;

struct Page* criaPagina(int id) {
	struct Page* novaPagina = (struct Page*) malloc(sizeof(struct Page));
	novaPagina->idPage = id;
	novaPagina->freq = 0;
	novaPagina->isDirty = 0;
	read_file(novaPagina, bytes);
	return novaPagina;
}
void operacao(struct Page* page, char operation) {
	if (operation == 'W') {
		page->isDirty = 1;
		writeCounter++;
	} else {
		readCounter++;
	}
}
struct Page* flush(struct Page*x) {
	if (x != NULL) {
		if (x->isDirty == 1) {
			write_file(x, bytes);
		}
		free(x->content);
		x->content = NULL;
	}
	return x;
}

int insere(int id, char oper) {
	struct Page* page = buscaSimples(lfu, id);

	if (page != NULL) {
		operacao(page, id);
		hit++;
		aumentaFreq(lfu);
		estado = 1;
		insereLFU(lfu, page);
	} else {
		operacao(page, id);
		miss++;
		estado = -1;
		page = criaPagina(id);

		if (lfu->size == capacidade) {
			removeLFU(lfu, page);
			free(flush(page));
			page = NULL;

		}
	}
	return estado;
}

void imprime() {
	struct Page* page = lfu->head;
	while (page != NULL) {
		printf("[%d]", page->idPage);
		page = page->next;
	}
}
void inicializa(int bytesT, int capacidadeT) {
	startFILE();
	capacidade = capacidadeT;
	bytes = bytesT;
	lfu = (struct LFU*) malloc(sizeof(struct LFU));
	lfu->head = NULL;
	lfu->tail = NULL;
	lfu->size = 0;

}

