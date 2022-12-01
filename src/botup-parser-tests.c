#include "botup-parser.h"

#define FREE_THREE_TOKENS\
	free_token(token1);\
	free_token(token2);\
	free_token(token3);\



void print_table(const prec_table_t table[NUM_OF_TOKEN_VARS][NUM_OF_TOKEN_VARS]) {

    char table_rep[] = {PREC_TABLE_VARS};

    for(int i = 0; i < NUM_OF_TOKEN_VARS; i++) {
        printf("---");
    }
    printf("\n");

    for(int i = 0; i < NUM_OF_TOKEN_VARS; i++) {
        for(int j = 0; j < NUM_OF_TOKEN_VARS; j++) {
            printf(" %c ", table_rep[table[i][j]]);
        }
        printf("|\n");
    }
    
    for(int i = 0; i < NUM_OF_TOKEN_VARS; i++) {
        printf("---");
    }
    printf("\n");

    printf("Only equal: %c\n", table_rep[table[open_rnd_var][cls_rnd_var]]);
    return;
}

void test_rand_names() {
    bst_node_t *symbol_table;
    bst_init(&symbol_table);

    char *rand_name1 = get_rand_var_name(&symbol_table);
    char *rand_name2 = get_rand_var_name(&symbol_table);
    char *rand_name3 = get_rand_var_name(&symbol_table);
    char *rand_name4 = get_rand_var_name(&symbol_table);

    printf("Random variable names:\n%s\n%s\n%s\n%s\n",
        rand_name1,
        rand_name2,
        rand_name3,
        rand_name4);

    free(rand_name1);
    free(rand_name2);
    free(rand_name3);
    free(rand_name4);
    bst_dispose(&symbol_table);
    return;
}

void test_parse_switch() {
	token_t *token1 = NULL;
	token_t *token2 = NULL;
	token_t *token3 = NULL;

	//float tests
	token1 = create_token_from_lit("151515.123456789", float_dot_num_var, 15);
	token2 = create_token_from_lit("1.1254698e+3", float_e_num_var, 16);
	token3 = create_token_from_lit("1523654.02E-4", float_e_num_var, 17);
	
	parse_switch(token1);
	parse_switch(token2);
	parse_switch(token3);

	FREE_THREE_TOKENS;
	//string tests
	token1 = create_token_from_lit("$HELLO_WORLD!", string_lit_end_var, 18);
	token2 = create_token_from_lit("HELLO\nWorld", string_lit_end_var, 19);
	token3 = create_token_from_lit("INS ANE\tTA B\\S\tA#N#D\nN EW\tL\\I\\N ES\n", string_lit_end_var, 20);
	
	parse_switch(token1);
	parse_switch(token2);
	parse_switch(token3);
	
	FREE_THREE_TOKENS;
	
	token1 = create_token_from_lit("null", null_var, 18);
	token2 = create_token_from_lit("123456789987654321", integ_var, 19);
	token3 = create_token_from_lit("-123456", integ_var, 20);

	parse_switch(token1);
	parse_switch(token2);
	parse_switch(token3);
	
	FREE_THREE_TOKENS;
}

int main() {
    token_t *first_token = get_token();
    bst_node_t *table;  //prvni token se vzdy preda shora
    bst_init(&table);   //prazdna tabulka symbolu
    bottom_up_parser(first_token, &table, false, true, ass_table);

    return 0;
}
