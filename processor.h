#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "stack.h"
#include <stdbool.h>

void add_padding(DataUnit* data_unit);
uint32_t calculate_crc32(const uint8_t *data, unsigned length);
bool cmp_crc32(DataUnit* data_unit);
// void apply_mask_to_tetrads(DataUnit* data_unit);
// void write_results_to_file(DataUnit* data_unit, const char* filename);

#endif