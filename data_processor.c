#include <assert.h>
#include <stdio.h>

#include "data_processor.h"

bool cmp_crc32(const DataUnit* data_unit) {
    return data_unit->msg.crc32 != calculate_crc32(get_stack_instance()->top->unit->msg.payload,
                                                get_stack_instance()->top->unit->msg.length);
}

void add_padding(DataUnit* data_unit) {
    int padding_bytes = 4 - (data_unit->msg.length % 4);
    for(; 0 != data_unit->msg.length % 4 ; ++(data_unit->msg.length)) {
        data_unit->msg.payload[data_unit->msg.length] = 0x00;
    }
}

uint32_t calculate_crc32(const uint8_t *data, unsigned length) {
    uint32_t crc = 0xFFFFFFFF;
    for (size_t i = 0; i < length; ++i) {
        crc ^= ((uint32_t)data[i]) << 24;
        for (int j = 0; j < 8; ++j) {
            if (crc & 0x80000000) {
                crc = (crc << 1) ^ POLYNOMIAL;
            } else {
                crc <<= 1;
            }
        }
    }
    return ~crc;
}


void apply_mask_to_tetrads(DataUnit* data_unit) {
    MaskUnion mask;
    mask.mask_value = data_unit->mask;
    assert(data_unit->msg.length % 4 == 0);
    unsigned num_tetrads = data_unit->msg.length / 4;

    for (unsigned t = 0; t < num_tetrads; ++t) {
        if (t % 2 == 0) {
            for (unsigned b = 0; b < 4; ++b) {
                data_unit->msg.payload[t * 4 + b] ^= mask.mask_bytes[b];
            }
        }
    }

    data_unit->msg.crc32 = calculate_crc32(data_unit->msg.payload, data_unit->msg.length);
}
