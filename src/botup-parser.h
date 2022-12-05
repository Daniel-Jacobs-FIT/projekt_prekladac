#ifndef BOTUP_PARSER_H
#define BOTUP_PARSER_H

#include "scanner.h"
#include "psa_stack.h"
#include "symbtable.h"
#include "utils.h"

typedef enum {gr, ls, eq, ER} prec_table_t;
#define PREC_TABLE_VARS '>', '<', '=', 'X'

extern const prec_table_t ass_table[NUM_OF_TOKEN_VARS][NUM_OF_TOKEN_VARS];
extern const prec_table_t cond_table[NUM_OF_TOKEN_VARS][NUM_OF_TOKEN_VARS];

void parse_switch(token_t *, char[3]);
void float_parse(token_t *);
void string_parse(token_t *);

bst_node_t *bottom_up_parser(token_t *, bst_node_t **, bool, bool, const prec_table_t[NUM_OF_TOKEN_VARS][NUM_OF_TOKEN_VARS]);

char *get_rand_var_name(bst_node_t **);

#endif
