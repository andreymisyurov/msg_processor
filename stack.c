#include "stack.h"
#include <stdlib.h>
#include <stdio.h>

static Stack* stack_instance = NULL;

Stack* get_stack_instance(void) {
    if (stack_instance == NULL) {
        stack_instance = (Stack*)malloc(sizeof(Stack));
        if (!stack_instance) {
            printf("error: Memory allocation failed for stack.\n");
            exit(EXIT_FAILURE);
        }
        stack_instance->top = NULL;
        stack_instance->size = 0;
    }
    return stack_instance;
}

void push(DataUnit* elem) {
    Stack* stack = get_stack_instance();
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
DataUnit* pop(void) {
    Stack* stack = get_stack_instance();
    if (stack->size == 0) {
        printf("warn: Stack underflow.\n");
        return NULL;
    }

    Node* node_to_pop = stack->top;
    DataUnit* unit = node_to_pop->unit;

    stack->top = node_to_pop->next;
    free(node_to_pop);

    --(stack->size);
    return unit;
}

void free_stack(void) {
    Stack* stack = get_stack_instance();
    while (stack->size > 0) {
        DataUnit* unit = pop();
        free(unit);
    }
}
