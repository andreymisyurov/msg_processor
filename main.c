#include "file_parser.h"
#include "line_handler.h"
#include "processor.h"
#include "stack.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_top_elem() {
    Stack* stack = get_stack_instance();
    printf("Message processed:\n");
    printf(" Type: %02X\n", stack->top->unit->msg.type);
    printf(" Length: %02X\n", stack->top->unit->msg.length);
    printf(" Payload: ");
    for (int i = 0; i < stack->top->unit->msg.length; i++)
        printf("%02X ", stack->top->unit->msg.payload[i]);
    printf("\n");
    printf(" CRC-32: %08X\n", stack->top->unit->msg.crc32);
    printf(" Mask: %08X\n", stack->top->unit->mask);
}

void print_data(const uint8_t* data, unsigned length) {
    printf("Data: ");
    for (unsigned i = 0; i < length; i++) {
        printf("%02X ", data[i]);
    }
    printf("\n");
}

int main() {
    if (parse_file("data_in.txt", handle_line) != 0) {
        printf("error: processing\n");
        return -1;
    }

    print_data(get_stack_instance()->top->unit->msg.payload, get_stack_instance()->top->unit->msg.length);
    printf("Custom CRC-32: %08X\n", calculate_crc32(get_stack_instance()->top->unit->msg.payload, get_stack_instance()->top->unit->msg.length));
    if(cmp_crc32(get_stack_instance()->top->unit)) {
        printf("error: CRC-32 mismatch! Possible data corruption.\n");
        exit(EXIT_FAILURE);
    }

    add_padding(get_stack_instance()->top->unit);

    apply_mask_to_tetrads(get_stack_instance()->top->unit);

    print_top_elem();

    print_data(get_stack_instance()->top->unit->msg.payload, get_stack_instance()->top->unit->msg.length);


    free_stack();
    return 0;
}
