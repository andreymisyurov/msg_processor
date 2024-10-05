#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data_processor.h"
#include "io.h"
#include "stack.h"
#include "logger.h"

int main() {
    set_log_file("log.txt");
    if (parse_file("data_in.txt", handle_msg) != 0) {
        log_message(LOG_LEVEL_ERROR, "error: Could not process data.");
        exit(EXIT_FAILURE);
    }
    DataUnit initial_data;
    memcpy(&initial_data, get_stack_instance()->top->unit, sizeof(DataUnit));

    if(cmp_crc32(get_stack_instance()->top->unit)) {
        log_message(LOG_LEVEL_ERROR, "error: CRC-32 mismatch! Possible data corruption.");
        exit(EXIT_FAILURE);
    }
    add_padding(get_stack_instance()->top->unit);
    apply_mask_to_tetrads(get_stack_instance()->top->unit);

    write_results_to_file(get_stack_instance()->top->unit, &initial_data, "data_out.txt");

    free_stack();
    close_log_file();
    return 0;
}
