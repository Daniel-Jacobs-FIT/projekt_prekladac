#ifndef PSA_STACK_H
#define PSA_STACK_H

#include <stdlib.h>
#include <stdbool.h>
#include "scanner.h"
#include "utils.h"

/*
Implementace stacku, jako dynamickeho pole
Pamet je alokovana pres *start 
*top slouzi pouze jako ukazatel na vrchol zasobniku
Na zacatku start = top, ukazuji na ukoncovaci element ";"
*/
typedef struct {
	token_t **arr;
    int top;
} stack_t;
/*
Velikost pole lze ziskat ze vzorce: size = (top - start)/sizeof(token_var) + 1
Neni tedy nutne ji ukladat do struktury
*/

stack_t *psa_stack_init();

bool psa_stack_is_empty(stack_t *);

token_t *psa_stack_get_nth(stack_t *, int);

token_t *psa_stack_top_term(stack_t *);

int psa_stack_split_top(stack_t* );

int psa_stack_push(stack_t *, token_t *);

int psa_stack_pop(stack_t *);

void psa_stack_dispose(stack_t *);

#endif
