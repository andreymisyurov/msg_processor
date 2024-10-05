#include "processor.h"
#include <assert.h>
#include <stdio.h>
#include <zlib.h>

bool cmp_crc32(DataUnit* data_unit) {
    uint32_t crc_zlib = crc32(0L, Z_NULL, 0); // Инициализация
    crc_zlib = crc32(crc_zlib,
                    (const Bytef *)get_stack_instance()->top->unit->msg.payload,
                    get_stack_instance()->top->unit->msg.length);
    printf("Custom CRC-32: %08X\n", crc_zlib);
    if (data_unit->msg.crc32 != crc_zlib) {
        return -1;
    }
    return 0;
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
        crc ^= data[i];
        for (int j = 0; j < 8; ++j) {
            if (crc & 1) {
                crc = (crc >> 1) ^ 0x04C11DB7;
            } else {
                crc >>= 1;
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
}
