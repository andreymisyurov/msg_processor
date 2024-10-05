#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "stack.h"
#include <stdbool.h>

#define POLYNOMIAL 0x04C11DB7

typedef union {
    uint32_t mask_value;
    uint8_t mask_bytes[4];
} MaskUnion;

uint32_t calculate_crc32(const uint8_t *data, unsigned length);
bool cmp_crc32(const DataUnit* data_unit);
void add_padding(DataUnit* data_unit);
void apply_mask_to_tetrads(DataUnit* data_unit);

#endif