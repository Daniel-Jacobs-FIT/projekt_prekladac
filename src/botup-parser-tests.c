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

int main() {
    printf("Table for assignments:\n");
    print_table(ass_table);
    printf("Table for conditions:\n");
    print_table(cond_table);
    return 0;
}
