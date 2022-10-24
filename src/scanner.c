#include "scanner.h"

const char *open_sign = "<?php";
const char *token_var_names[] = {"none", "div_oper"}; //TODO

/* Funkce pro overeni, ze soubor STREAM na zacatku obsahuje znaky SIGN
 * stream = odtud nacita znaky
 * sign = znacka ke kontrole 
 * */
bool sign_check(FILE *stream, const char *sign) {
    int sign_len = strlen(sign);
    ++sign_len; //inkrementace, aby se terminalni znak \0 vesel do bufferu

    char buffer[sign_len];

    fgets(buffer, sign_len, stream);
    
    if(!strcmp(sign, buffer)) {
        return true; 
    } else {
        return false;
    }
}

/* Funkce pro praci s tokeny*/

/* Funkce pro vytvoreni tokenu - argumenty funkce budou jednotlive polozky tokenu
 * Pro zvyseni citelnosti kodu pri tvorbe tokenu
 * content = obsah tokenu (nactene symboly)
 * token_var = varianta tokenu
 * linu_num = cislo radku, na kterem byl token nacteny
 * */
token_t new_token(char* content, token_var variant, int line_num) {
    token_t token;
    token.content = content;
    token.variant = variant;
    token.line_num = line_num;
    return token;
}

/* Funkce pro vypsani tokenu, pro debugovaci ucely
 * token = ukazatel na token, ktery se ma vypsat
 * */
void print_token(token_t *token) {
    printf("Token:\"%s\" Variant:%s Line:%d\n", token->content, token_var_names[token->variant], token->line_num); 
}

/* Funkce pro uvolneni pameti na heapu zabirane tokenem
 * token = ukazatel na token, ktery se ma  uvolnit
 * */
void free_token(token_t *token) {
    free(token->content);
    token->content = NULL;
}
