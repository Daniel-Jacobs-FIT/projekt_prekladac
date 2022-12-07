#ifndef BOTUP_PARSER_H
#define BOTUP_PARSER_H

#include "scanner.h"
#include "psa_stack.h"
#include "symbtable.h"
#include "utils.h"

typedef enum {gr, ls, eq, ER} prec_table_t;
#define PREC_TABLE_VARS '>', '<', '=', 'X'
#define NO_REDEF_TYPE_VAR "_no_redef_type_var"

//makro pro defvar v podminkach (zamezeni redefinicim)
#define BOTUP_DEFVAR_COND(TO_DEFVAR)\
    if(in_loop == false) {\
        fprintf(stdout, "DEFVAR %s@%s\n", frame_name, TO_DEFVAR);\
    } else {\
        char *no_redef_label = get_rand_name(symb_table, "_defvar_skip");\
        if(EXIT_CODE != 0) {\
            return;\
        }\
        bst_insert(symb_table, no_redef_label, label_id, NULL);\
        if(EXIT_CODE != 0) {\
            return;\
        }\
        fprintf(stdout, "TYPE %s@%s %s@%s\n", frame_name, NO_REDEF_TYPE_VAR,\
                                              frame_name, TO_DEFVAR);\
        fprintf(stdout, "JMPIFNEQ %s %s@%s string@\n", no_redef_label,\
            frame_name, NO_REDEF_TYPE_VAR);\
        fprintf(stdout, "DEFVAR %s@%s\n", frame_name, TO_DEFVAR);\
        fprintf(stdout, "LABEL %s\n", no_redef_label);\
    }

extern const prec_table_t ass_table[NUM_OF_TOKEN_VARS][NUM_OF_TOKEN_VARS];
extern const prec_table_t cond_table[NUM_OF_TOKEN_VARS][NUM_OF_TOKEN_VARS];

void parse_switch(token_t *, char[3]);
void float_parse(token_t *);
void string_parse(token_t *);

bst_node_t *bottom_up_parser(stack_t *, int *, bst_node_t **, bool, bool, bool, const prec_table_t[NUM_OF_TOKEN_VARS][NUM_OF_TOKEN_VARS]);

char *get_rand_name(bst_node_t **, char *);

token_t *copy_token(stack_t *, int *);

#endif
