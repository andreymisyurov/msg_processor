#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "stack.h"
#include <stdbool.h>

typedef union {
    uint32_t mask_value;  // 32-битная маска
    uint8_t mask_bytes[4]; // 4 байта маски
} MaskUnion;

void add_padding(DataUnit* data_unit);
uint32_t calculate_crc32(const uint8_t *data, unsigned length);
bool cmp_crc32(DataUnit* data_unit);
void apply_mask_to_tetrads(DataUnit* data_unit);

#endif