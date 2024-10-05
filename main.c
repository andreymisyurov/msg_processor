#include "file_parser.h"
#include "line_handler.h"
#include "processor.h"
#include "stack.h"

#include <stdio.h>

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

int main() {
    if (parse_file("data_in.txt", handle_line) != 0) {
        printf("error: processing\n");
        return -1;
    }

    add_padding(get_stack_instance()->top->unit);

    print_top_elem();

    free_stack();

    return 0;
}
