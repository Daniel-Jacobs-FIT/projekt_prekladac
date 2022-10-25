#include "scanner.h"

const char *OPEN_SIGN = "<?php";
const char *TOKEN_VAR_NAMES[] = {ALL_TOKEN_VARS}; //TODO

/* Funkce pro overeni, ze standartni vstup na zacatku obsahuje znaky SIGN
 * sign = znacka ke kontrole 
 * */
bool string_check(char *sign) {
    int sign_len = strlen(sign);
    ++sign_len; //inkrementace, aby se terminalni znak \0 vesel do bufferu

    char buffer[sign_len];

    fgets(buffer, sign_len, stdin);
    
    if(!strcmp(sign, buffer)) {
        return true; 
    } else {
        return false;
    }
}

/* Funkce implementujici automat ridici lexikalni analyzu
 * input = znak nacteny ze vstupu
 * token = token, do ktereho se ma zapsat lexem
 * */
scanner_state_t fsm_step(char input, token_t *token) {
    static scanner_state_t fsm_state = default_s;

    switch(fsm_state) {
        case default_s :
            break;
        case div_oper_s :
            break;
        case com_oneline_s : 
            break; 
        case com_block_s : 
            break;
        case com_block_end_s :
            break;
        case num_oper_s :
            break;
        case oper_conc_s :
            break;
        case not_eq1_s :
            break;
        case not_eq2_s :
            break;
        case not_eq3_s :
            break;
        case eq_or_assign_s :
            break;
        case eq2_s :
            break;
        case eq3_s :
            break;
        case grt_s :
            break;
        case grt_eq_s :
            break;
        case less_s :
            break;
        case less_eq_s :
            break;
        case open_rnd_s :
            break;
        case cls_rnd_s :
            break;
        case open_curl_s :
            break;
        case cls_curl_s :
            break;
        case semicol_s :
            break;
        case string_lit_end_s :
            break;
        case string_lit_s :
            break;
        case esc_char_s :
            break;
        case esc_seq_s :
            break;
        case hex1_s :
            break;
        case hex2_s :
            break;
        case oct1_s :
            break;
        case oct2_s :
            break;
        case integ_s :
            break;
        case float_dot_s :
            break;
        case float_dot_num_s :
            break;
        case float_e_s :
            break;
        case float_e_sign_s :
            break;
        case float_e_num :
            break;
        case id_or_end_s :
            break;
        case identif_s :
            break;
        case end_sign_s :
            break;
        case end_prg_s :
            break;
        //TODO lze zjednodusit -> sloucit stavy, obsluha EOF, obsluha chyby, ...
    }  
    return fsm_state;
}

/* Funkce pro parser, na pozadani cte vstup, dokud neprecte dost informaci
 * pro vytvoreni tokenu, ktery vraci
 * */
token_t get_token() {
    char input_char = getc(stdin);
    static int line_counter = 0;

    if(input_char == '\n') ++line_counter; 

    token_t current_token = create_token(NULL, none, 0);

    while(current_token.variant == none) {
        fsm_step(input_char, &current_token);
    }

    current_token.line_num = line_counter;
    return current_token;
}

/* Funkce pro praci s tokeny*/

/* Funkce pro vytvoreni tokenu - argumenty funkce budou jednotlive polozky tokenu
 * Pro zvyseni citelnosti kodu pri tvorbe tokenu
 * content = obsah tokenu (nactene symboly)
 * token_var = varianta tokenu
 * linu_num = cislo radku, na kterem byl token nacteny
 * */
token_t create_token(char* content, token_var variant, int line_num) {
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
    printf("Token:\"%s\" Variant:%s Line:%d\n", token->content, TOKEN_VAR_NAMES[token->variant], token->line_num); 
}

/* Funkce pro uvolneni pameti na heapu zabirane tokenem
 * token = ukazatel na token, ktery se ma  uvolnit
 * */
void free_token(token_t *token) {
    free(token->content);
    token->content = NULL;
    token->variant = none;
    token->line_num = 0;
}
