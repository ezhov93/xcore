#include "ecore/circularbuffer.h"

#include <string.h>

static inline size_t _advanceHeadtailValue(size_t value, size_t max) {
  return (value + 1) % max;
}

static void _advanceHeadPointer(CircularBuffer *self) {
  if (CircularBuffer_isFull(self))
    self->tail = _advanceHeadtailValue(self->tail, self->max);
  self->head = _advanceHeadtailValue(self->head, self->max);
  self->full = (self->head == self->tail);
}

bool CircularBuffer_init(CircularBuffer *self, void *buffer, const size_t bytes,
                         size_t elementSize) {
  const size_t mod = bytes % elementSize;
  if (mod != 0) return false;
  self->data = buffer;
  self->elementSize = elementSize;
  self->max = bytes / elementSize;
  CircularBuffer_clear(self);
	return true;
}

size_t CircularBuffer_size(const CircularBuffer *self) {
  size_t size = self->max;
  if (CircularBuffer_isFull(self) == false) {
    if (self->head >= self->tail)
      size = (self->head - self->tail);
    else
      size = (self->max + self->head - self->tail);
  }
  return size;
}

bool CircularBuffer_push(CircularBuffer *self, const void *data) {
#ifdef CIRCULAR_BUFFER_CICLIC
  memcpy(self->data[self->head * self->elementSize], data, self->elementSize);
  _advanceHeadPointer(self);
#else
  if (CircularBuffer_isFull(self) == true) return false;
  memcpy(&self->data[self->head * self->elementSize], data, self->elementSize);
  _advanceHeadPointer(self);
#endif
  return true;
}

bool CircularBuffer_pop(CircularBuffer *self, void *data) {
  if (CircularBuffer_isEmpty(self) == true) return false;
  memcpy(data, &self->data[self->tail * self->elementSize], self->elementSize);
  self->tail = _advanceHeadtailValue(self->tail, self->max);
  self->full = false;
  return true;
}

bool CircularBuffer_peek(CircularBuffer *self, void *data) {
  if (CircularBuffer_isEmpty(self)) return false;
  const size_t pos = self->tail;
  memcpy(data, &self->data[pos * self->elementSize], self->elementSize);
  return true;
}