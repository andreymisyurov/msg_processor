#include "stack.h"
#include <stdlib.h>
#include <stdio.h>

void init_stack(Stack* stack) {
    stack->top = NULL;
    stack->size = 0;
}

void push(Stack* stack, DataUnit* elem) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (!new_node) {
        printf("error: Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    new_node->unit = (DataUnit*)malloc(sizeof(DataUnit));
    if (!new_node->unit) {
        printf("error: Memory allocation failed.\n");
        free(new_node);
        exit(EXIT_FAILURE);
    }
    *new_node->unit = *elem;

    new_node->next = stack->top;
    stack->top = new_node;
    ++(stack->size);
}

// Allocate memory. Should clean up return value
DataUnit* pop(Stack* stack) {
    if (stack->size == 0) {
        printf("error: Stack underflow.\n");
        return NULL;
    }

    Node* node_to_pop = stack->top;
    DataUnit* unit = node_to_pop->unit;

    stack->top = node_to_pop->next;
    free(node_to_pop);

    stack->size--;
    return unit;
}

void free_stack(Stack* stack) {
    while (stack->size > 0) {
        DataUnit* unit = pop(stack);
        free(unit);
    }
}
