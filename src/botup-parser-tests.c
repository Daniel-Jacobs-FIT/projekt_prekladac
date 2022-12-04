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
    char frame_name[] = "GF";
	token_t *token1 = NULL;
	token_t *token2 = NULL;
	token_t *token3 = NULL;

	//float tests
	token1 = create_token_from_lit("151515.123456789", float_var, 15);
	token2 = create_token_from_lit("1.1254698e+3", float_var, 16);
	token3 = create_token_from_lit("1523654.02E-4", float_var, 17);
	
	parse_switch(token1, frame_name);
	parse_switch(token2, frame_name);
	parse_switch(token3, frame_name);

	FREE_THREE_TOKENS;
	//string tests
	token1 = create_token_from_lit("$HELLO_WORLD!", string_lit_end_var, 18);
	token2 = create_token_from_lit("HELLO\nWorld", string_lit_end_var, 19);
	token3 = create_token_from_lit("INS ANE\tTA B\\S\tA#N#D\nN EW\tL\\I\\N ES\n", string_lit_end_var, 20);
	
	parse_switch(token1, frame_name);
	parse_switch(token2, frame_name);
	parse_switch(token3, frame_name);
	
	FREE_THREE_TOKENS;
	
	token1 = create_token_from_lit("null", null_var, 18);
	token2 = create_token_from_lit("123456789987654321", integ_var, 19);
	token3 = create_token_from_lit("-123456", integ_var, 20);

	parse_switch(token1, frame_name);
	parse_switch(token2, frame_name);
	parse_switch(token3, frame_name);
	
	FREE_THREE_TOKENS;
}

char *DEFINED_VAR_KEYS[] = {"$my_number", "$my_word", "$secret_float"};
char DEFINED_VAR_TYPES[] = {'i', 's', 'f'};
int NUM_OF_DEFINED_VARS = 3;

int main() {

    EXIT_CODE = 0;

    bst_node_t *table; 
    bst_init(&table);   //prazdna tabulka symbolu

    //nastrkani nejakych promennych do tabulky symbolu
    char *alloced_key;
    char *alloced_type;
    for(int i = 0; i < NUM_OF_DEFINED_VARS; i++) {
        alloced_key = (char *)malloc((14 + 1) * sizeof(char));
        alloced_type =(char *)malloc((1 + 1) * sizeof(char));
        strcpy(alloced_key, DEFINED_VAR_KEYS[i]);
        alloced_type[0] = DEFINED_VAR_TYPES[i];
        
        bst_insert(&table, alloced_key, var_id, alloced_type);
    }

    token_t *first_token = get_token();
    do {
        bottom_up_parser(first_token, &table, false, true , ass_table);
        first_token = get_token();
    } while (first_token->variant != end_prg_var && first_token->variant != err_var);
    fprintf(stdout, "Exit code = %d\n", EXIT_CODE);
    return EXIT_CODE;
}
