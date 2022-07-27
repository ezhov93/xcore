#ifndef ECORE_CIRCULARBUFFER_H
#define ECORE_CIRCULARBUFFER_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

//#define CIRCULAR_BUFFER_CICLIC

typedef struct {
  char *data;
  size_t head;
  size_t tail;
  size_t elementSize;
  size_t max;
  bool full;
} CircularBuffer;

bool CircularBuffer_init(CircularBuffer *self, void *buffer, const size_t size,
                         const size_t elementSize);

static inline bool CircularBuffer_isFull(const CircularBuffer *self) {
  return self->full;
}

static inline bool CircularBuffer_isEmpty(const CircularBuffer *self) {
  return ((CircularBuffer_isFull(self) == false) && (self->head == self->tail));
}

bool CircularBuffer_push(CircularBuffer *self, const void *data);

bool CircularBuffer_pop(CircularBuffer *self, void *data);

bool CircularBuffer_peek(CircularBuffer *self, void *data);

static inline void CircularBuffer_clear(CircularBuffer *self) {
  self->head = 0;
  self->tail = 0;
  self->full = false;
}
size_t CircularBuffer_size(const CircularBuffer *self);

static inline size_t CircularBuffer_capacity(const CircularBuffer *self) {
  return self->max;
}
static inline size_t CircularBuffer_space(const CircularBuffer *self) {
  return self->max - CircularBuffer_size(self);
}

#endif // ECORE_CIRCULARBUFFER_H
