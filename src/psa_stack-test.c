#include <string.h>
#include "psa_stack.h"
#include "scanner.h"

const char *TEST_TOKEN_VARS_NAMES[] = {ALL_TOKEN_VARS};

char tokens[][100] = {"123.652e524", "1.596E6", "0008.1e13", "some_key_from_symbol_table", "$Hello_World123", "$cyberPUNK2077", "$el33th4ck3r", "$______no______", "65", "5161465", "121", "1647988498", "00300", "000000000010"};

token_var variants[] = {float_var, float_var, float_var, expression_var, identif_variable_var, identif_variable_var, identif_variable_var, identif_variable_var, integ_var, integ_var, integ_var, integ_var, integ_var, integ_var};

#define TEST(STACK, DESCRIPTION)\
{\
    printf("%s\n", DESCRIPTION);\
    STACK = psa_stack_init();\
}

#define ENDTEST(STACK)\
{\
	printf("after:\n");\
	psa_print_stack(STACK);\
	psa_stack_dispose(STACK);\
	printf("------------------------------\n");\
}

/* int digit_count(int num) */
/* { */
/* 	int count = 0; */
/* 	while(num > 1); */
/* 	{ */
/* 		count++; */
/* 		num /= 10; */
/* 	} */
/* 	return count; */
/* } */

/* int max_index(int arr[], int arr_size) */
/* { */
/* 	int max = arr[0], index = 0; */
/*     for (int i = 0; i < arr_size; i++) */
/* 	{ */
/* 		if(max < arr[i]) */
/* 		{ */
/* 			max = arr[i]; */
/* 			index = i; */
/* 		} */
/* 	} */
/* 	return i; */
/* } */

void psa_print_part(token_t *addr, char *content, token_var variant, int line_num)
{
	/* int lenghts[] = {14, strlen(content), strlen(TEST_TOKEN_VARS_NAMES[variant]), 6 + digit_count(line_num)}; */
	/* int max_index = max_index(lenghts, 4); */
	/* int max_size = lenghts[max_index]; */
	
	/* //loop through array */
	/* for(int i = 0; i < 4; i++) */
	/* { */
	/* 	if(i == max_index) */
	/* 	{ */
	/* 		lenghts[i] = 0; */
	/* 	} */
	/* 	else */
	/* 	{ */
	/* 		lenghts[i] = (max_size - lenghts[i])/2; */
	/* 	} */
		
	/* } */

	printf("| %p |\n", (void*)addr);
	printf("| %s |\n", content);
	printf("| %s |\n", TEST_TOKEN_VARS_NAMES[variant]);
	printf("| line: %d |\n", line_num);
	printf("---------------\n");
}

void psa_print_stack(stack_t *stack)
{
	if(psa_stack_is_empty(stack))
	{
		printf("STACK IS EMPTY\n");
	}else
	{
		token_t *token = NULL;

		for(int i = 0; i <= stack->top; i++)
		{	
			token = psa_stack_get_nth(stack, i);
			psa_print_part(token, token->content, token->variant, token->line_num);
		}
	}
}

token_t *create_token_test(char *content, token_var variant, int line_num)
{
	token_t *token = (token_t *)malloc(sizeof(token_t));
	char *token_content = (char *)malloc(strlen(content)+1);
	strcpy(token_content, content);
	token->content = token_content;
	token->line_num = line_num;
	token->variant = variant;
	return token;
}

void print_before(stack_t *stack)
{
	printf("before:\n");
	psa_print_stack(stack);
}

int main(void)
{
    stack_t *stack = NULL;
	token_t *token = NULL;

	TEST(stack, "stack init:\n");
	ENDTEST(stack);

	TEST(stack, "insert one:");
	print_before(stack);
	token = create_token_test("Hello World", 5, 15);
	printf("elem: %s, %s, %d\n", token->content, TEST_TOKEN_VARS_NAMES[5], token->line_num);
	psa_stack_push(stack, token);
	ENDTEST(stack);

	TEST(stack, "insert many:");
	print_before(stack);
	for(int i = 0; i < 4; i++)
	{
		token = create_token_test(tokens[i], variants[i], i);
		printf("inserting elem [%s, %s, %d]\n", tokens[i], TEST_TOKEN_VARS_NAMES[variants[i]], i);
		psa_stack_push(stack, token);
	}
	ENDTEST(stack);

	TEST(stack, "get top:");
	for(int i = 0; i < 4; i++)
	{
		token = create_token_test(tokens[i], variants[i], i);
		psa_stack_push(stack, token);
	}
	token = psa_stack_top_term(stack);
	printf("gotten token: [%s, %s, %d]\n", token->content, TEST_TOKEN_VARS_NAMES[token->variant], token->line_num);
	ENDTEST(stack);

	TEST(stack, "split top, there is expression:");
	for(int i = 0; i < 4; i++)
	{
		token = create_token_test(tokens[i], variants[i], i);
		psa_stack_push(stack, token);
	}
    psa_stack_split_top(stack);
	printf("gotten token: [%s, %s, %d]\n", token->content, TEST_TOKEN_VARS_NAMES[token->variant], token->line_num);
	ENDTEST(stack);

	TEST(stack, "split top, there is no expression:");
	for(int i = 0; i < 3; i++)
	{
		token = create_token_test(tokens[i], variants[i], i);
		psa_stack_push(stack, token);
	}
    psa_stack_split_top(stack);
	printf("gotten token: [%s, %s, %d]\n", token->content, TEST_TOKEN_VARS_NAMES[token->variant], token->line_num);
	ENDTEST(stack);

	TEST(stack, "get nth (2 from top):");
	for(int i = 0; i < 4; i++)
	{
		token = create_token_test(tokens[i], variants[i], i);
		psa_stack_push(stack, token);
	}
	token = psa_stack_get_nth(stack, 2);
	printf("gotten token: [%s, %s, %d]\n", token->content, TEST_TOKEN_VARS_NAMES[token->variant], token->line_num);
	ENDTEST(stack);

	//get non existant, both ways and empty -1

	TEST(stack, "pop one:\n");
	for(int i = 0; i < 4; i++)
	{
		token = create_token_test(tokens[i], variants[i], i);
		psa_stack_push(stack, token);
	}
	printf("before:\n");
	psa_print_stack(stack);
	psa_stack_pop(stack);
	ENDTEST(stack);

	TEST(stack, "pop all:\n");
	for(int i = 0; i < 4; i++)
	{
		token = create_token_test(tokens[i], variants[i], i);
		psa_stack_push(stack, token);
	}
	printf("before:\n");
	psa_print_stack(stack);
	psa_stack_pop(stack);
	psa_stack_pop(stack);
	psa_stack_pop(stack);
	psa_stack_pop(stack);
	ENDTEST(stack);

	//dispose empty
	//dispose 1 elem
}
