#include "processor.h"
#include <stdio.h>

void add_padding(DataUnit* data_unit) {
    int padding_bytes = 4 - (data_unit->msg.length % 4);
    for(; 0 != data_unit->msg.length % 4 ; ++(data_unit->msg.length)) {
        data_unit->msg.payload[data_unit->msg.length] = 0x00;
    }
}
