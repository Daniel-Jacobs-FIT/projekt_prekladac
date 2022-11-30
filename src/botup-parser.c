#include "botup-parser.h"

const prec_table_t ass_table[NUM_OF_TOKEN_VARS][NUM_OF_TOKEN_VARS] = {
//       +   -   .   *   /   >   >=  <   <=  === !== id  f_e f   i   s   nil exp (   )   ;   {
/* + */ {gr, gr, gr, ls, ls, gr, gr, gr, gr, gr, gr, ls, ls, ls, ls, ER, ls, ls, ls, gr, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* - */ {gr, gr, gr, ls, ls, gr, gr, gr, gr, gr, gr, ls, ls, ls, ls, ER, ls, ls, ls, gr, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* . */ {gr, gr, gr, ls, ls, gr, gr, gr, gr, gr, gr, ls, ER, ER, ER, ls, ls, ls, ls, gr, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* * */ {gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, ls, ls, ls, ls, ER, ls, ls, ls, gr, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* / */ {gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, ls, ls, ls, ls, ER, ls, ls, ls, gr, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* > */ {ls, ls, ls, ls, ls, gr, gr, gr, gr, gr, gr, ls, ls, ls, ls, ls, ls, ls, ls, gr, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* >=*/ {ls, ls, ls, ls, ls, gr, gr, gr, gr, gr, gr, ls, ls, ls, ls, ls, ls, ls, ls, gr, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* < */ {ls, ls, ls, ls, ls, gr, gr, gr, gr, gr, gr, ls, ls, ls, ls, ls, ls, ls, ls, gr, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* <=*/ {ls, ls, ls, ls, ls, gr, gr, gr, gr, gr, gr, ls, ls, ls, ls, ls, ls, ls, ls, gr, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/*===*/ {ls, ls, ls, ls, ls, ls, ls, ls, ls, gr, gr, ls, ls, ls, ls, ls, ls, ls, ls, gr, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/*!==*/ {ls, ls, ls, ls, ls, ls, ls, ls, ls, gr, gr, ls, ls, ls, ls, ls, ls, ls, ls, gr, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* id*/ {gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, ER, ER, ER, ER, ER, ER, ER, ls, gr, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/*f_e*/ {gr, gr, ER, gr, gr, gr, gr, gr, gr, gr, gr, ER, ER, ER, ER, ER, ER, ER, ls, gr, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* f */ {gr, gr, ER, gr, gr, gr, gr, gr, gr, gr, gr, ER, ER, ER, ER, ER, ER, ER, ls, gr, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* i */ {gr, gr, ER, gr, gr, gr, gr, gr, gr, gr, gr, ER, ER, ER, ER, ER, ER, ER, ls, gr, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* s */ {ER, ER, gr, ER, ER, gr, gr, gr, gr, gr, gr, ER, ER, ER, ER, ER, ER, ER, ls, gr, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/*nil*/ {gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, ER, ER, ER, ER, ER, ER, ER, ls, gr, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/*exp*/ {gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, ER, ER, ER, ER, ER, ER, ER, ls, gr, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* ( */ {ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, eq, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* ) */ {gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, ER, ER, ER, ER, ER, ER, ER, ER, gr, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* ; */ {ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* { */ {ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
        {ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
        {ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
        {ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
        {ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
        {ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
        {ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
        {ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
        {ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
        {ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
        {ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
};

const prec_table_t cond_table[NUM_OF_TOKEN_VARS][NUM_OF_TOKEN_VARS] = {
//       +   -   .   *   /   >   >=  <   <=  === !== id  f_e f   i   s   nil exp (   )   ;   {
/* + */ {gr, gr, gr, ls, ls, gr, gr, gr, gr, gr, gr, ls, ls, ls, ls, ER, ls, ls, ls, gr, ER, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* - */ {gr, gr, gr, ls, ls, gr, gr, gr, gr, gr, gr, ls, ls, ls, ls, ER, ls, ls, ls, gr, ER, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* . */ {gr, gr, gr, ls, ls, gr, gr, gr, gr, gr, gr, ls, ER, ER, ER, ls, ls, ls, ls, gr, ER, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* * */ {gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, ls, ls, ls, ls, ER, ls, ls, ls, gr, ER, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* / */ {gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, ls, ls, ls, ls, ER, ls, ls, ls, gr, ER, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* > */ {ls, ls, ls, ls, ls, gr, gr, gr, gr, gr, gr, ls, ls, ls, ls, ls, ls, ls, ls, gr, ER, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* >=*/ {ls, ls, ls, ls, ls, gr, gr, gr, gr, gr, gr, ls, ls, ls, ls, ls, ls, ls, ls, gr, ER, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* < */ {ls, ls, ls, ls, ls, gr, gr, gr, gr, gr, gr, ls, ls, ls, ls, ls, ls, ls, ls, gr, ER, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* <=*/ {ls, ls, ls, ls, ls, gr, gr, gr, gr, gr, gr, ls, ls, ls, ls, ls, ls, ls, ls, gr, ER, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/*===*/ {ls, ls, ls, ls, ls, ls, ls, ls, ls, gr, gr, ls, ls, ls, ls, ls, ls, ls, ls, gr, ER, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/*!==*/ {ls, ls, ls, ls, ls, ls, ls, ls, ls, gr, gr, ls, ls, ls, ls, ls, ls, ls, ls, gr, ER, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* id*/ {gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, ER, ER, ER, ER, ER, ER, ER, ls, gr, ER, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/*f_e*/ {gr, gr, ER, gr, gr, gr, gr, gr, gr, gr, gr, ER, ER, ER, ER, ER, ER, ER, ls, gr, ER, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* f */ {gr, gr, ER, gr, gr, gr, gr, gr, gr, gr, gr, ER, ER, ER, ER, ER, ER, ER, ls, gr, ER, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* i */ {gr, gr, ER, gr, gr, gr, gr, gr, gr, gr, gr, ER, ER, ER, ER, ER, ER, ER, ls, gr, ER, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* s */ {ER, ER, gr, ER, ER, gr, gr, gr, gr, gr, gr, ER, ER, ER, ER, ER, ER, ER, ls, gr, ER, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/*nil*/ {gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, ER, ER, ER, ER, ER, ER, ER, ls, gr, ER, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/*exp*/ {gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, ER, ER, ER, ER, ER, ER, ER, ls, gr, ER, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* ( */ {ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, eq, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* ) */ {gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, ER, ER, ER, ER, ER, ER, ER, ER, gr, ER, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* ; */ {ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* { */ {ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
        {ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
        {ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
        {ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
        {ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
        {ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
        {ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
        {ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
        {ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
        {ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
        {ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
};

int inf_char_conv(char *str, int input)
{
	if(str == NULL)
	{
		char *content = (char *)malloc(2);
		if(content == NULL)
		{
			fprintf(stderr, "failed to allocate memory\n");
			return -1;
		}
		content[0] = (char)input;
		content[1] = '\0';
		str = content;
	}
	else
	{
		int str_size = strlen(str);
		char *content = (char *)realloc((void *)str, str_size+2);
		if(content == NULL)
		{
			fprintf(stderr, "failed to allocate memory\n");
			return -1;
		}
		content[str_size] = (char)input;
		content[str_size+1] = '\0';
		str = content;
	}
	return 0;
}

void float_parse(token_t *token)
{
	double num = atof(token->content);
	printf("float@%a\n", num);
}

//token with a string content to a specific type if string that is the put back into the string content
void string_parse(token_t *token)
{
	char *new_str = NULL;
	//I will need the size of the new string so I can allocate it, but maybe I can just use inf_char_input
	//and when I get a char that has a value less than 33 then I have to escape it, so Ill go into a sub function where it will be escaped
	//other special chars include 035, 092
	for(int i = 0; token->content[i] != '\0'; i++)
	{
		if(token->content[i] < 33 ||token->content[i] == 35 ||token->content[i] == 92)
		{
			int digit_count = 1, digit = (int)token->content[i];
			while((digit /= 10) >= 1)
			{
				digit_count++;
			}
			//use snprintf
			//+1 for the '\0' char and +1 for the '\' char
			char *helper_str = (char *)malloc(digit_count+2);
			snprintf(helper_str, digit_count+2, "\\%d", (int)token->content[i]);
			for(int j = 0; helper_str[j] != '\0'; j++)
			{
				if(inf_char_conv(new_str, helper_str[j]) == -1)
				{
					return;
				}
			}
			free(helper_str);
		}
		else
		{
			if(inf_char_conv(new_str, token->content[i]) == -1)
			{
				return;
			}
		}
	}
	free(token->content);
	token->content = new_str;
	printf("string@%s\n", token->content);
}
//current input se meni na zaklade toho, pokud jsme v podmince nebo v prirazeni a na zacatku
#define GET_CURRENT_INPUT \
        if(is_start == true && from_top_down != NULL) {\
            current_input = from_top_down;\
            is_start = false;\
        } else {\
            current_input = get_token();\
        }

void bottom_up_parser(token_t *from_top_down, bst_node_t **symb_table,
                        const prec_table_t precedence_table[NUM_OF_TOKEN_VARS][NUM_OF_TOKEN_VARS]) {
    stack_t *stack = psa_stack_init();
    token_t *current_input;

    token_t *top_of_stack;
    token_t *second_from_top;
    token_t *third_from_top;

    token_var end_token_variant;
    bool is_start = true;
    prec_table_t table_cell;

    if(from_top_down != NULL) { //bottom up parser zpracovava vyraz v prirazeni
        token_t *end_token = create_token(NULL, semicol_var, 0);
        psa_stack_push(stack, end_token);

        end_token_variant = semicol_var; 

    } else { //bottom up parser zpracovava podminku
        token_t *end_token = create_token(NULL, open_curl_var, 0);
        psa_stack_push(stack, end_token);

        /* krome ukoncujiciho znaku musime na zasobnik dat i oteviraci zavorku, aby se zpracovalo ( EXPR ) */
        token_t *initial_token_on_stack0 = create_token(NULL, less_prec_var, 0);
        psa_stack_push(stack, initial_token_on_stack0);
        token_t *initial_token_on_stack1 = create_token(NULL, open_rnd_var, 0);
        psa_stack_push(stack, initial_token_on_stack1);

        end_token_variant = open_curl_var; 
    }

    GET_CURRENT_INPUT
    do {
        top_of_stack = psa_stack_get_top(stack);

        table_cell = precedence_table[top_of_stack->variant][current_input->variant];

        switch(table_cell) {
            case eq:
                psa_stack_push(stack, current_input);
                GET_CURRENT_INPUT
                break;
            case ls:
                ; //kompilator si stezoval
                token_t *less_prec_token = create_token(NULL, less_prec_var, 0);
                psa_stack_push(stack, less_prec_token);
                psa_stack_push(stack, current_input);
                GET_CURRENT_INPUT
                break;
            case gr:
                //ziskani nejvrchnejsich tri znaku, aby bylo mozne zjistit shodu s nejakym pravidlem
                top_of_stack = psa_stack_get_top(stack);
                second_from_top = psa_stack_get_nth(stack, 1);
                third_from_top = psa_stack_get_nth(stack, 2);
                if(top_of_stack->variant == expression_var) {   //mozna shoda s operandovymi pravidly: E -> E op E

                } else if (top_of_stack->variant == cls_rnd_var) {  //mozna shoda se zavorkovym pravidlem
                    //kontrola shody se zavorkovym pravidlem
                    if(second_from_top->variant == expression_var && third_from_top->variant == open_rnd_var) {

                    }
                    
                    
                } else { //mozna shoda s prirazovacimi pravidly
                    token_var rule_variant = none;
                    switch(top_of_stack->variant) {
                        case identif_variable_var:
                            rule_variant = top_of_stack->variant;
                            break;
                        case float_e_num_var:
                            rule_variant = top_of_stack->variant;
                            break;
                        case float_dot_num_var:
                            rule_variant = top_of_stack->variant;
                            break;
                        case integ_var:
                            rule_variant = top_of_stack->variant;
                            break;
                        case string_lit_end_var:
                            rule_variant = top_of_stack->variant;
                            break;
                        case null_var:
                            rule_variant = top_of_stack->variant;
                            break;
                        default:
                    }
                    if(rule_variant != none) {
                        
                    } else {
                        //nejak zpracuj chybu
                    }
                }

                break;
            case ER: //chyba!!!!
                //return?
                break;
        }

        top_of_stack = psa_stack_get_top(stack);
        second_from_top = psa_stack_get_nth(stack, 1);

    } while (current_input->variant != end_token_variant || top_of_stack->variant != expression_var || second_from_top->variant != end_token_variant);
}

/*
Funkce pro generovani nahodneho jmena, zatim neobsazeneho v tabulce symbolu, pomocne promenne pro vyrazy.
Nahodne, abychom snizili sanci degenerace stromu, jimz je
reprezentovana tabulka symbolu.
Vraci dynamicky alokovany string "_expressionDDD", kde D je cislo 0-9
*/
char *get_rand_var_name(bst_node_t **symb_table) {
    char beginning[] = "_expression";
    char *rand_name;
    do {
        int suffix = abs(rand() % 1000);
        char suffix_as_str[4];
        snprintf(suffix_as_str, 4 * sizeof(char), "%d", suffix);

        rand_name = (char *)calloc(strlen(beginning) + 4, sizeof(char));
        strcpy(rand_name, beginning);
        strcat(rand_name, suffix_as_str);
    } while (bst_search(*symb_table, rand_name) != NULL);
    //kontrola zda-li neni uz v tabulce symbolu nejaka promenna se stejnym jmenem

    return rand_name;
}
