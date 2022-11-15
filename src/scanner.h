#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#ifndef SCANNER_H_
#define SCANNER_H_

/* Enum pro urceni stavu automatu ridiciho lexikalni analyzu
 * */
typedef enum {default_s, div_oper_s, com_oneline_s,\
                com_block_s, com_block_end_s, num_oper_s, num_oper_adv_s,\
                oper_conc_s, not_eq1_s, not_eq2_s, not_eq3_s,\
                eq_or_assign_s, eq2_s, eq3_s, grt_s, grt_eq_s,\
                less_s, less_eq_s, open_rnd_s, cls_rnd_s,\
                open_curl_s, cls_curl_s, semicol_s,\
                string_lit_end_s, string_lit_s, esc_char_s,\
                esc_seq_s, hex1_s, hex2_s, oct1_s, oct2_s,\
                integ_s, float_dot_s, float_dot_num_s,\
                float_e_s, float_e_sign_s, float_e_num_s,\
                id_or_end_s, identif_s, end_sign_s, end_prg_s, expect_eof_s\
    } scanner_state_t;

/* Enum pro urceni varianty tokenu (je to operator '/' nebo identifikator nebo ...)
 * */
typedef enum {none, div_oper, num_oper_var, num_oper_adv_var, oper_conc_var,\
                eq_or_assign_var, eq_var, not_eq_var,\
                grt_var, grt_eq_var, less_var, less_eq_var,\
                open_rnd_var, cls_rnd_var, open_curl_var,\
                cls_curl_var, semicol_var, string_lit_end_var,\
                integ_var, float_dot_num_var, float_e_num_var,\
                identif_var, end_prg_var, err_var\
} token_var; //TODO - mozne zjednoduseni, scanneru je lhostejna varianta reprezentace floatu, ...
/* Makro pro print_token, mapovani enumu na jejich jmena pro lepsi citelnost pri debuggovani
 * */
#define ALL_TOKEN_VARS \
    "none", "div_oper", "num_oper_var", "num_oper_adv_var", "oper_conc_var", \
    "eq_or_assign_var", "eq_var", "not_eq_var", \
    "grt_var", "grt_eq_var", "less_var", "less_eq_var", \
    "open_rnd_var", "cls_rnd_var", "open_curl_var", \
    "cls_curl_var", "semicol_var", "string_lit_end_var", \
    "integ_var", "float_dot_num_var", "float_e_num_var", \
    "identif_var", "end_prg_var", "err_var"

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

bool string_check(char *sign);

scanner_state_t fsm_step(int input, token_t *token);

token_t get_token();

token_t create_token(char* content, token_var variant, int line_num);

void print_token(token_t *token);

void free_token(token_t *token);

#endif
