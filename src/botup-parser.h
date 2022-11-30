#ifndef BOTUP_PARSER_H
#define BOTUP_PARSER_H

#include "scanner.h"
#include "psa_stack.h"
#include "symbtable.h"

typedef enum {gr, ls, eq, ER} prec_table_t;
#define PREC_TABLE_VARS '>', '<', '=', 'X'

extern const prec_table_t ass_table[NUM_OF_TOKEN_VARS][NUM_OF_TOKEN_VARS];
extern const prec_table_t cond_table[NUM_OF_TOKEN_VARS][NUM_OF_TOKEN_VARS];


void bottom_up_parser(token_t *from_top_down, bst_node_t **symb_table, const prec_table_t precedence_table[NUM_OF_TOKEN_VARS][NUM_OF_TOKEN_VARS]);

char *get_rand_var_name(bst_node_t **symb_table);

#endif
