#include <stdio.h>
#include <stdlib.h>
FILE *file;

struct LFU {
	struct Page* tail;
	struct Page* head;
	int size;
};

struct Page {
	int isDirty;
	int idPage;
	int freq;
	struct Page* next;
	struct Page* prev;
	struct LFU* lfu;
	struct LFU* lista;
	char* content;
};
struct Page* buscaSimples(struct LFU* lfu, int id) {
	struct Page* x = lfu->head;
	while (x != NULL) {
		if (x->idPage == id) {
			return x;
		}
		x = x->next;
	}
	return NULL;
}
void startFILE() {

	file = fopen("file.txt", "r+");
}

void read_file(struct Page* x, int BYTES) {

	char * xs = (char*) malloc(BYTES);
	fseek(file, (x->idPage / 100000) * BYTES, SEEK_SET);
	fread(xs, sizeof(char), BYTES, file);
	x->content = xs;

}
void write_file(struct Page* x, int BYTES) {
	fseek(file, (x->idPage / 100000) * BYTES, SEEK_SET);
	fwrite(x->content, sizeof(char), BYTES, file);
}

void aumentaFreq(struct LFU* lfu) {
	struct Page* aux = lfu->head;

	while (aux != NULL) {
		aux->freq++;
		aux = aux->next;
	}
}

void insereLFU(struct LFU*lfu, struct Page* page) {
	struct Page* pageExiste = buscaSimples(lfu, page);
	struct Page* x = lfu->head;

	if (page != NULL) {
		page->prev = NULL;
		page->next = NULL;
		page->freq = 1;

		if (lfu->head != NULL) {
			if (pageExiste == NULL) {
				page->next = lfu->head;
				lfu->head->prev = page;
				lfu->head = page;
			} else {

				if (page == lfu->head) {
					lfu->head->freq = 1;
				} else if (page == lfu->tail) {
					lfu->tail = page->prev;
					lfu->head->prev = lfu->tail->next;
					page->next = lfu->head;
					lfu->head = lfu->tail->next;
					lfu->tail->next = NULL;
					page->prev = NULL;
					page->freq = 1;
				} else if (page != lfu->head && page != lfu->tail) {
					//remover no meio da lista e inserir no inicio com freq=1
					while (x != NULL) {
						if (x->next == page) {
							x->next = page->next;
							page->next;

							page->next = page->prev = NULL;
						}
						x = x->next;
					}
				}
			}

		} else {
			lfu->head = page;
			lfu->tail = page;
			page->freq++;
			lfu->size++;
		}
		page->lista = lfu;
	}
}
struct Page * removeLRU(struct LRU * lru, struct Page* page) {
	if (page != NULL) {
		if (lfu->tail == lfu->head) {
			lfu->tail = lfu->head = NULL;
		} else if (page == lfu->tail) {
			lfu->tail = page->prev;
			lfu->tail->next = NULL;
		}
		lfu->size--;
		page->lista = NULL;
		page->next = NULL;
		page->prev = NULL;
	}
	return page;
}

