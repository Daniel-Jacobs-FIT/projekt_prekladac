#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

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
