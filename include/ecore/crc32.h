/*
 * @file   crc32.h
 * @author Mikhail Ezhov <ezhov93@gmail.com>
 * @brief  CRC class.
 * Copyright 2022 Ezhov Mihail ezhov93@gmail.com.
 */

#ifndef CRC32_H
#define CRC32_H

#include <stdbool.h>

typedef struct {
  unsigned int crc;
} Crc32;

static inline void Crc32_init(Crc32 *const self) { self->crc = ~0L; }
static inline unsigned int Crc32_value(const Crc32 *self) { return ~self->crc; }
static inline void Crc32_reset(Crc32 *const self) { self->crc = ~0L; }
static inline bool Crc32_check(const Crc32 *self, unsigned int crc) {
  return (bool)(Crc32_value(self) == crc);
}
void Crc32_add(Crc32 *const self, const unsigned char data);
void Crc32_addData(Crc32 *const self, const void *data, int size);

#endif // CRC32_H
