#include "scanner.h"

int main(int argc, char* argv[]) {

    token_t *current_token = get_token();

    while(current_token->variant != end_prg_var) {
		print_token(current_token);
        free_token(current_token);
		current_token = get_token();
    }
    print_token(current_token);
	free_token(current_token);

    return 0;
}
