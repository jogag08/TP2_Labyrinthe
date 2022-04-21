#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "labo.h"

#define OPTICK_ENABLE_GPU (0)
#include "optick.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

Stack stack_init(size_t max_size) {
	Stack s;
	s.max = max_size;
	s.top = -1;
	s.elements = (void**)allocate(sizeof(void*) * s.max);
	return s;
}

void stack_push(Stack* s, void* element) {
	if (s->max != s->top + 1) {
		s->top++;
		s->elements[s->top] = element;
	}
}

void* stack_pop(Stack* s) {
	if (s->top > -1) {
		void* elem = s->elements[s->top];
		s->elements[s->top] = NULL;
		--s->top;
		return elem;
	}
	return NULL;
}
void queue_init(Queue* q) {
	q->next = q->prev = NULL;
	q->count = 0;
}
void queue_push(Queue* q, void* data) {
	QNode* n = (QNode*)allocate(sizeof(Node));
	n->data = data;
	n->prev = n->next = NULL;
	if (q->prev == NULL) {
		q->prev = q->next = n;
	}
	else {
		QNode* temp = q->next;
		q->next = temp->prev = n;
		n->next = temp;
	}
}
void* queue_pop(Queue* q) {
	if (q->prev != NULL) {
		QNode* temp = q->prev;
		if(temp->prev != NULL)
			temp->prev->next = NULL;
		q->prev = temp->prev;
		q->count++;
		return temp->data;
	}
}

#define HEAP_SIZE (UINT32_MAX * UINT32_MAX) * 9999999999
static uint8_t* heap = NULL;
static size_t heap_top = 0;
void* allocate(size_t size) {
	size_t old_top = heap_top;
	heap_top += size;
	assert(heap_top <= HEAP_SIZE);
	return &heap[old_top];
}

int main(int argc, char** argv) {
	heap = (uint8_t*)malloc(HEAP_SIZE);
	assert(heap != NULL);
	OPTICK_APP("ConsoleApp");

	int width, height, channels, nodesQty;
	unsigned char* img = stbi_load("perfect2k.png", &width, &height, &channels, 0);
	nodesQty = width * height;
	if (img == NULL)
	{
		printf("Error in loading the image\n");
		exit(1);
	}
	printf("Loaded image with a width of %dpx, a height of %dpx and %d channels Nodes : %d\n", width, height, channels, nodesQty);

	stbi_write_png("perfect2k.png", width, height, channels, img, width * channels);
	stbi_write_jpg("perfect2k.jpg", width, height, channels, img, 100);

	AdjMatrix* graph = create_graph(nodesQty);
	
	return 0;
}
