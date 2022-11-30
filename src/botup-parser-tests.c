#include "botup-parser.h"

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

int main() {
    test_rand_names();

    return 0;
}
