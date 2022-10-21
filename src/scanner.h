#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#ifndef SCANNER_H_
#define SCANNER_H_ 1

/* Enum pro urceni stavu automatu ridiciho lexikalni analyzu
 * */
typedef enum {default_s, div_oper_s, com_oneline} scanner_state; //TODO

/* Enum pro urceni varianty tokenu (je to operator '/' nebo identifikator nebo ...)
 * */
typedef enum {div_oper} token_var; //TODO

/* Struktura predstavujici token
 * content = obsah tokenu (nactene symboly)
 * token_var = varianta tokenu, je to enum deklarovany vyse
 * line_num = cislo radku, na kterem byl token nacteny
 * */
typedef struct token_struct{
    char *content;
    token_var variant;
    int line_num;
} token_t;

bool sign_check(FILE *stream, const char *sign);

void fill_token(token_t* token, char* content, token_var variant, int line_num);

void print_token(token_t *token);

void free_token(token_t *token);

#endif
