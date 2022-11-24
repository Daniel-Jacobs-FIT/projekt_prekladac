#ifndef PSA_STACK_H
#define PSA_STACK_H

#include <stdlib.h>
#include <stdbool.h>
#include "scanner.h" //potrebne pro ziskani token_var data typu

/*
Implementace stacku, jako dynamickeho pole
Pamet je alokovana pres *start 
*top slouzi pouze jako ukazatel na vrchol zasobniku
Na zacatku start = top, ukazuji na ukoncovaci element ";"
*/
typedef struct {
    token_var *start; 
    token_var *top;
} stack_t;

/*
Velikost pole lze ziskat ze vzorce: size = (top - start)/sizeof(token_var) + 1
Neni tedy nutne ji ukladat do struktury
*/

#endif
