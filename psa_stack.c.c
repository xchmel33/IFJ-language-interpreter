#include <stdio.h>
#include <stdlib.h>

#include "error.h"
#include "psa_stack.h"



Psa_stack *s_init()
{
    Psa_stack *new_stack = malloc(sizeof(Psa_stack));

    if (new_stack == NULL)
    {
        return NULL;
    }

    new_stack->active = NULL;
    new_stack->top = NULL;

    return new_stack;
}

void s_destroy(Psa_stack *stack)
{
    Stack_item *prev;
    while (stack->top != NULL)
    {
        prev = (Stack_item *) stack->top->lptr;
        free(stack->top->lptr);
        stack->top = prev;
    }
    free(stack);
}

int s_push (Psa_stack *stack, enum operator_s operator_ss)
{
    Stack_item *new_i = malloc(sizeof(Stack_item));
    if (new_i == NULL)
    {
        return ERR_PARSER;
    }

    new_i->operator_ss = operator_ss;
    new_i->lptr = (struct Stack_item *) stack->top;
    new_i->rptr = NULL;

    if (stack->top != NULL)
    {
        stack->top->rptr = (struct Stack_item *) new_i;
    }
    stack->top = new_i;
    return ERR_OK;
}
void s_pop (Psa_stack *stack)
{
    if (stack->top != NULL)
    {
        Stack_item *tmp = stack->top;
        stack->top = (Stack_item *) tmp->lptr;
        free(tmp);
    }
}








int main() {
    printf("Hello, World!\n");
    Psa_stack *stack = s_init();
    operator_s k = 's';
    s_push(stack,k);

    return 0;
}
