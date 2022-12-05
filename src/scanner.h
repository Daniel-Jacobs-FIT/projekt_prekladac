#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "utils.h"

#ifndef SCANNER_H_
#define SCANNER_H_

/* Enum pro urceni stavu automatu ridiciho lexikalni analyzu
 * */
typedef enum {default_s, div_oper_s, com_oneline_s,\
                com_block_s, com_block_end_s, add_oper_s, sub_oper_s, mul_oper_s,\
                oper_conc_s, not_eq1_s, not_eq2_s, not_eq3_s,\
                eq_or_assign_s, eq2_s, eq3_s, grt_s, grt_eq_s,\
                less_s, less_eq_s, open_rnd_s, cls_rnd_s,\
                open_curl_s, cls_curl_s, semicol_s,\
                string_lit_end_s, string_lit_s, esc_char_s,\
                esc_seq_s, hex1_s, oct1_s,\
                integ_s, float_dot_s, float_dot_num_s,\
                float_e_s, float_e_sign_s, float_e_num_s,\
                id_or_end_s, identif_s, end_sign_s,\
				end_prg_s, expect_eof_s, comma_s, colon_s\
    } scanner_state_t;

/* Enum pro urceni varianty tokenu (je to operator '/' nebo identifikator nebo ...)
 * */
//keywords: else, float, ?float, function, if, int, ?int, null, return, string, ?string, void, while
typedef enum {add_oper_var, sub_oper_var,  oper_conc_var,
                mul_oper_var, div_oper_var,
                grt_var, less_var, less_eq_var, grt_eq_var,
                eq_var, not_eq_var,
                identif_variable_var,
                float_var, integ_var, string_lit_end_var, null_var, //vsechny konstanty v BTP
                expression_var,            //varianta pouzita pouze v BOTUP parseru
                open_rnd_var, cls_rnd_var, //zavorky ve vyrazech
                semicol_var, open_curl_var, //ukoncovaci znaky
                less_prec_var,             //specialni varianta v BOTUP parseru pro analyzu
                /* ^varianty pouzite nejak platne v precedencni tabulce, 23 jich je */
                cls_curl_var, none, assign_var,
                identif_function_var, identif_keyword_var,
				end_prg_var, err_var, comma_var, colon_var
} token_var; //TODO - mozne zjednoduseni, scanneru je lhostejna varianta reprezentace floatu, ...
#define NUM_OF_TOKEN_VARS 31
/* Makro pro print_token, mapovani enumu na jejich jmena pro lepsi citelnost pri debuggovani
 * */
#define ALL_TOKEN_VARS "add_oper_var", "sub_oper_var", "oper_conc_var",\
                "mul_oper_var", "div_oper_var",\
                "grt_var", "less_var", "less_eq_var", "grt_eq_var",\
                "eq_var", "not_eq_var",\
                "identif_variable_var",\
                "float_var", "integ_var", "string_lit_end_var", "null_var",\
                "expression_var",\
                "open_rnd_var", "cls_rnd_var",\
                "semicol_var", "open_curl_var", "less_prec_var",\
                "cls_curl_var", "none", "assign_var",\
                "identif_function_var", "identif_keyword_var",\
				"end_prg_var", "err_var", "comma_var", "colon_var"

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

token_t *get_token();

token_t *create_token(char* content, token_var variant, int line_num);

token_t *create_token_from_lit(char *, token_var, int);

void print_token(token_t *token);

void free_token(token_t *token);


#endif
