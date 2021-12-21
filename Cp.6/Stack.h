#include "LinkList.h"

#define INIT_SIZE 10

typedef struct
{
    int size;
    int capacity;
    ListNode *top;
    LinkList *list;
} Stack;

Stack *init_stack();
void clean_stack(Stack *);
int size_stack(Stack *);
void *top_stack(Stack *);
void *pop_stack(Stack *);
void push_stack(Stack *, void *);
void print_stack(Stack *stack, PLN print_function);
void reset_stack(Stack *);

Stack *init_stack()
{
    Stack *stack = (Stack *)malloc(sizeof(Stack));
    stack->list = init_list();
    stack->size = 0;
    stack->capacity = 0;
    return stack;
}
void clean_stack(Stack *stack)
{
    clean_list(stack->list);
    free(stack);
}
int size_stack(Stack *stack)
{
    return stack->size;
}
int empty_stack(Stack *stack)
{
    return stack->size ? 0 : 1;
}
void *top_stack(Stack *stack)
{
    if (stack->size == 0)
    {
        printf("Error,stack has been empty!\n");
        return 0;
    }
    return (stack->top)->data;
}
void *pop_stack(Stack *stack)
{
    if (stack->size == 0)
    {
        printf("Error,stack has been empty!\n");
        return NULL;
    }
    (stack->size)--;
    void *rValue = stack->top->data;
    stack->top = stack->top->prev;
    return rValue;
}
void push_stack(Stack *stack, void *data)
{
    if (stack->size == stack->capacity)
    {
        stack->top = push_back(stack->list, data);
        (stack->capacity)++;
        (stack->size)++;
        return;
    }
    stack->top = stack->top->succ;
    stack->top->data = data;
    (stack->size)++;
}
void print_stack(Stack *stack, PLN print_function)
{
    ListNode *head = stack->top;
    for (int i = stack->size; i != 0; i--)
    {
        (*print_function)(head);
        head = head->prev;
    }
}
void reset_stack(Stack *stack)
{
    stack->size = 0;
    stack->top = stack->list->header;
}
