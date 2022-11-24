#include "psa_stack.h"

//TODO doplnit token_var, abychom ho mohli pouzit

/* 
Inicializuje zasobnik, naalokuje misto, prida ukoncovaci element ";" a nastavi pointery
Pri selhani alokace pameti vrati -1
*/
int psa_stack_init() {

}

/* 
Zjisti, jestli je zasobnik prazdny (start = top)
*/
bool psa_stack_is_empty() {

}

/* 
Vrati hodnotu z vrcholu zasobniku
*/
token_var psa_stack_get_top() {

}

/* 
Vrati hodnotu elementu tesne pod vrcholem zasobnku
Bude potreba v urcite situaci u precedencni analyzy
*/
token_var psa_stack_get_second() {

}

/* 
Provede realokaci (zvetseni zasovnku o 1) a vlozi hodnotu elementu na vrchol zasobniku
Pri selhani realokace pameti vrati -1
*/
int psa_stack_push() { 

}

/* 
Provede realokaci (zmenseni(!) zasovnku o 1) a vlozi hodnotu elementu na vrchol zasobniku
Pri selhani realokace pameti vrati -1
*/
void psa_stack_pop() {

}

/*
Provede uvolneni celeho zasobniku a uvede jej do stavu pred inicializaci
*/
void psa_stack_dispose() {

}
