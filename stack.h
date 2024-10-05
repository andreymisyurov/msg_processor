#ifndef STACK_H
#define STACK_H

#include <stdint.h>

typedef struct Message {
    uint8_t type;
    uint8_t length;
    uint8_t payload[254];
    uint32_t crc32;
} Message;

typedef struct DataUnit {
    Message msg;
    uint32_t mask;
} DataUnit;

typedef struct Node {
    DataUnit* unit;
    struct Node* next;
} Node;

typedef struct Stack {
    Node* top;
    int size;
} Stack;

Stack* get_stack_instance(void);
void push(DataUnit* element);
DataUnit* pop(void);
void free_stack(void);

#endif
