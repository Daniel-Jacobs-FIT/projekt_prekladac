#include "botup-parser.h"

const prec_table_t ass_table[NUM_OF_TOKEN_VARS][NUM_OF_TOKEN_VARS] = {
//       +   -   .   *   /   >   >=  <   <=  === !== id  f   i   s   nil exp (   )   ;   {
/* + */ {gr, gr, gr, ls, ls, gr, gr, gr, gr, gr, gr, ls, ls, ls, ls, ls, ls, ls, gr, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* - */ {gr, gr, gr, ls, ls, gr, gr, gr, gr, gr, gr, ls, ls, ls, ls, ls, ls, ls, gr, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* . */ {gr, gr, gr, ls, ls, gr, gr, gr, gr, gr, gr, ls, ls, ls, ls, ls, ls, ls, gr, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* * */ {gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, ls, ls, ls, ls, ls, ls, ls, gr, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* / */ {gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, ls, ls, ls, ls, ls, ls, ls, gr, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* > */ {ls, ls, ls, ls, ls, gr, gr, gr, gr, gr, gr, ls, ls, ls, ls, ls, ls, ls, gr, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* >=*/ {ls, ls, ls, ls, ls, gr, gr, gr, gr, gr, gr, ls, ls, ls, ls, ls, ls, ls, gr, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* < */ {ls, ls, ls, ls, ls, gr, gr, gr, gr, gr, gr, ls, ls, ls, ls, ls, ls, ls, gr, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* <=*/ {ls, ls, ls, ls, ls, gr, gr, gr, gr, gr, gr, ls, ls, ls, ls, ls, ls, ls, gr, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/*===*/ {ls, ls, ls, ls, ls, ls, ls, ls, ls, gr, gr, ls, ls, ls, ls, ls, ls, ls, gr, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/*!==*/ {ls, ls, ls, ls, ls, ls, ls, ls, ls, gr, gr, ls, ls, ls, ls, ls, ls, ls, gr, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* id*/ {gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, ER, ER, ER, ER, ER, ER, ls, gr, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* f */ {gr, gr, ER, gr, gr, gr, gr, gr, gr, gr, gr, ER, ER, ER, ER, ER, ER, ls, gr, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* i */ {gr, gr, ER, gr, gr, gr, gr, gr, gr, gr, gr, ER, ER, ER, ER, ER, ER, ls, gr, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* s */ {ER, ER, gr, ER, ER, gr, gr, gr, gr, gr, gr, ER, ER, ER, ER, ER, ER, ls, gr, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/*nil*/ {gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, ER, ER, ER, ER, ER, ER, ls, gr, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/*exp*/ {gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, ER, ER, ER, ER, ER, ER, ls, gr, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* ( */ {ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, eq, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* ) */ {gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, ER, ER, ER, ER, ER, ER, ER, gr, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* ; */ {ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* { */ {ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
        {ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
        {ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
        {ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
        {ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
        {ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
        {ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
        {ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
        {ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
        {ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
        {ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
};

const prec_table_t cond_table[NUM_OF_TOKEN_VARS][NUM_OF_TOKEN_VARS] = {
//       +   -   .   *   /   >   >=  <   <=  === !== id  f   i   s   nil exp (   )   ;   {
/* + */ {gr, gr, gr, ls, ls, gr, gr, gr, gr, gr, gr, ls, ls, ls, ls, ls, ls, ls, gr, ER, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* - */ {gr, gr, gr, ls, ls, gr, gr, gr, gr, gr, gr, ls, ls, ls, ls, ls, ls, ls, gr, ER, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* . */ {gr, gr, gr, ls, ls, gr, gr, gr, gr, gr, gr, ls, ls, ls, ls, ls, ls, ls, gr, ER, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* * */ {gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, ls, ls, ls, ls, ls, ls, ls, gr, ER, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* / */ {gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, ls, ls, ls, ls, ls, ls, ls, gr, ER, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* > */ {ls, ls, ls, ls, ls, gr, gr, gr, gr, gr, gr, ls, ls, ls, ls, ls, ls, ls, gr, ER, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* >=*/ {ls, ls, ls, ls, ls, gr, gr, gr, gr, gr, gr, ls, ls, ls, ls, ls, ls, ls, gr, ER, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* < */ {ls, ls, ls, ls, ls, gr, gr, gr, gr, gr, gr, ls, ls, ls, ls, ls, ls, ls, gr, ER, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* <=*/ {ls, ls, ls, ls, ls, gr, gr, gr, gr, gr, gr, ls, ls, ls, ls, ls, ls, ls, gr, ER, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/*===*/ {ls, ls, ls, ls, ls, ls, ls, ls, ls, gr, gr, ls, ls, ls, ls, ls, ls, ls, gr, ER, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/*!==*/ {ls, ls, ls, ls, ls, ls, ls, ls, ls, gr, gr, ls, ls, ls, ls, ls, ls, ls, gr, ER, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* id*/ {gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, ER, ER, ER, ER, ER, ER, ls, gr, ER, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* f */ {gr, gr, ER, gr, gr, gr, gr, gr, gr, gr, gr, ER, ER, ER, ER, ER, ER, ls, gr, ER, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* i */ {gr, gr, ER, gr, gr, gr, gr, gr, gr, gr, gr, ER, ER, ER, ER, ER, ER, ls, gr, ER, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* s */ {ER, ER, gr, ER, ER, gr, gr, gr, gr, gr, gr, ER, ER, ER, ER, ER, ER, ls, gr, ER, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/*nil*/ {gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, ER, ER, ER, ER, ER, ER, ls, gr, ER, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/*exp*/ {gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, ER, ER, ER, ER, ER, ER, ls, gr, ER, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* ( */ {ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, eq, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* ) */ {gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, ER, ER, ER, ER, ER, ER, ER, gr, ER, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* ; */ {ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* { */ {ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
        {ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
        {ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
        {ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
        {ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
        {ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
        {ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
        {ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
        {ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
        {ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
        {ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
};

#define MEM_ERR_WR(PTR)\
	if(PTR == NULL)\
	{\
        fprintf(stderr, "Chyba alokace paměti!\n");\
        EXIT_CODE = 99;\
        return;\
	}

#define MEM_ERR\
	if(EXIT_CODE != 0)\
	{\
        EXIT_CODE = 99;\
        return;\
	}

void parse_switch(token_t *token, char frame_name[3])
{
	switch(token->variant)
	{
		case float_var:
			float_parse(token);
			break;
		case integ_var:
			printf("int@%s\n", token->content);
			break;
		case string_lit_end_var:
			string_parse(token);
			break;
		case null_var:
			printf("nil@nil\n");
			break;
        case identif_variable_var:
            printf("%s@%s\n", frame_name, token->content);
            break;
		default:
			fprintf(stderr, "Interní chyba, netisknutelná varianta tokenu\n");
			break;
	}
}

int inf_char_str_lit(char **str, int input)
{
	if((*str) == NULL)
	{
		char *content = (char *)malloc(2);
		if(content == NULL)
		{
            fprintf(stderr, "Chyba alokace paměti!\n");
            EXIT_CODE = 99;
            return -1;
		}
		content[0] = (char)input;
		content[1] = '\0';
		(*str) = content;
	}
	else
	{
		int str_size = strlen((*str));
		char *content = (char *)realloc((void *)(*str), str_size+2);
		if(content == NULL)
		{
            fprintf(stderr, "Chyba alokace paměti!\n");
            EXIT_CODE = 99;
            return -1;
		}
		content[str_size] = (char)input;
		content[str_size+1] = '\0';
		(*str) = content;
	}
	return 0;
}

void float_parse(token_t *token)
{
	double num = atof(token->content);
	printf("float@%a\n", num);
}

void string_parse(token_t *token)
{
	char *new_str = NULL;
	for(int i = 0; token->content[i] != '\0'; i++)
	{
		if(token->content[i] < 33 ||token->content[i] == 35 ||token->content[i] == 92)
		{
			int digit_count = 1, digit = (int)token->content[i];
			char extra_zero[3];
			while((digit /= 10) >= 1)
			{
				digit_count++;
			}
			if(digit_count == 1)
			{
				extra_zero[0] = '0';
				extra_zero[1] = '0';
				extra_zero[2] = '\0';
				//pridavame 2 nuly a proto +2
				digit_count+=2;
			}
			
			if(digit_count == 2)
			{
				extra_zero[0] = '0';
				extra_zero[1] = '\0';
				//pridavame 1 nulu a proto 1;
				digit_count += 1;
			}
			//+1 pro '\0' char a +1 pro '\' char
			char *helper_str = (char *)malloc(digit_count+2);
            if(helper_str == NULL) {
                EXIT_CODE = 99;
                
            }
			snprintf(helper_str, digit_count+2, "\\%s%d", extra_zero,(int)token->content[i]);
			for(int j = 0; helper_str[j] != '\0'; j++)
			{
				inf_char_str_lit(&new_str, helper_str[j]);
				if(EXIT_CODE != 0)
				{
					return;
				}
			}
			free(helper_str);
		}
		else
		{
			inf_char_str_lit(&new_str, token->content[i]);
            if(EXIT_CODE != 0)
            {
                return;
            }
		}
	}
	free(token->content);
	token->content = new_str;
	printf("string@%s\n", token->content);
}

/* Funkce pro nahrazeni to_pop polozek na stacku jednou polozkou expression
 * pro kterou alokuje retezec pro jmeno a zkopiruje obsah retezce expr_content,
 * (zamezeni double free), nastavi variantu na expression_var a cislo radku na line_num
 * V pripade chyby pameti nastavi EXIT_CODE na 99
 * */
void replace_and_push_exp(int to_pop, stack_t *stack, char *expr_content, int line_num) {
    char *new_expr_content = calloc(strlen(expr_content) + 1, sizeof(char)); 
    int local_line_num = line_num;
    if(EXIT_CODE != 0) {    //kontrola, ze calloc probehl v poradku
        return;
    }
    strncpy(new_expr_content, expr_content, strlen(expr_content) + 1);

    for(int i = 0; i < to_pop; i++) {
        psa_stack_pop(stack);
    }

    if(EXIT_CODE != 0) {    //kontrola, ze popy probehly v poradku
        return;
    }

    psa_stack_push(stack, create_token(new_expr_content, expression_var, local_line_num));
    return;
}

void parse_soft_ineq(bst_node_t *first_operand, bst_node_t *second_operand,
                                token_t *operator_token, char frame_name[3],
                                bst_node_t **symb_table, stack_t *stack, bool in_loop) {
    char first_data_type = first_operand->data_type[0];
    char second_data_type = second_operand->data_type[0];
    char opcode[3] = "";

    if(operator_token->variant == grt_eq_var) {
        strcpy(opcode, "GT");
    } else {
        strcpy(opcode, "LT");
    }

    /* Jsou potreba tri nove promenne, neexistuje instrukce pro operaci <= nebo >=
     * Nasimulujeme E <= E jako E < E or E = E
     *
     * Nejprve prepiseme null na kompatibilni operator, jelikoz instrukce null neumi pracovat s null
     * Pote sepiseme bud < nebo > instrukci (zalezi <= nebo >=)
     * Pote napiseme instrukci pro ==
     * Nakonec je vysledek cele operace ulozen do final_expr
     * */

    #define FREE_EXPR_NAMES_SOFT_COMP\
        free(final_expr_name);\
        free(ineq_expr_name);\
        free(eq_expr_name);
    #define FREE_EXPR_DATA_TYPES_SOFT_COMP\
        free(final_expr_data_type);\
        free(ineq_expr_data_type);\
        free(eq_expr_data_type);

    _Pragma("GCC diagnostic push")
    _Pragma("GCC diagnostic ignored \"-Wuse-after-free\"")

    //vygenerovani retezcu s datovymi typy pro nove promenne
    char *final_expr_data_type = (char *)calloc(1 + 1, sizeof(char));
    char *ineq_expr_data_type = (char *)calloc(1 + 1, sizeof(char));
    char *eq_expr_data_type = (char *)calloc(1 + 1, sizeof(char));
    if(final_expr_data_type == NULL || ineq_expr_data_type == NULL || eq_expr_data_type == NULL) {
        FREE_EXPR_DATA_TYPES_SOFT_COMP
    }
    //vygenerovani jmen pro nove promenne
    char *final_expr_name = get_rand_name(symb_table, "_expression");
    bst_insert(symb_table, final_expr_name, var_id, final_expr_data_type);
    char *ineq_expr_name = get_rand_name(symb_table, "_expression");
    bst_insert(symb_table, ineq_expr_name, var_id, ineq_expr_data_type);
    char *eq_expr_name = get_rand_name(symb_table, "_expression");
    bst_insert(symb_table, eq_expr_name, var_id, eq_expr_data_type);
    if(EXIT_CODE != 0) {
        FREE_EXPR_NAMES_SOFT_COMP
        FREE_EXPR_DATA_TYPES_SOFT_COMP
        return;
    }
    final_expr_data_type[0] = 'b';
    ineq_expr_data_type[0] = 'b'; 
    eq_expr_data_type[0] = 'b'; 

    //generace kodu pro definovani promennych
    BOTUP_DEFVAR_COND(final_expr_name)
    BOTUP_DEFVAR_COND(ineq_expr_name)
    BOTUP_DEFVAR_COND(eq_expr_name)

    #define CONV_NULL_SOFT_COMP(WHICH_OP_KEY)\
            case 'i':\
                fprintf(stdout, "MOVE %s@%s int@0\n", frame_name, WHICH_OP_KEY);\
                break;\
            case 'f':\
                fprintf(stdout, "MOVE %s@%s float@0x0p+0\n", frame_name, WHICH_OP_KEY);\
                break;\
            case 's':\
                fprintf(stdout, "MOVE %s@%s string@\n", frame_name, WHICH_OP_KEY);\
                break;\
            case 'n':\
                fprintf(stdout, "MOVE %s@%s int@0\n", frame_name, WHICH_OP_KEY);\
                break;\
            default:\
                EXIT_CODE = 7;\
                FREE_EXPR_NAMES_SOFT_COMP\
                FREE_EXPR_DATA_TYPES_SOFT_COMP\
                fprintf(stderr, "Chyba na řádku: %d\nBoolean hodnotu nelze použít ve výrazu\n", operator_token->line_num);\
                return;

    //prepsani null na neco schudnejsiho
    if(first_data_type == 'n') {        // null >= ?
        switch(second_data_type) {
            CONV_NULL_SOFT_COMP(first_operand->key)
        }
    } else if(second_data_type == 'n') {  // null >= ?
        switch(first_data_type) {
            CONV_NULL_SOFT_COMP(second_operand->key)
        }
    } else if((first_data_type == 's' && second_data_type != 's') ||    // s >= i/f/b nebo naopak
              (second_data_type == 's' && first_data_type != 's')) {
        EXIT_CODE = 7;
        FREE_EXPR_NAMES_SOFT_COMP
        FREE_EXPR_DATA_TYPES_SOFT_COMP
        fprintf(stderr, "Chyba na řádku: %d\nŘetězec nelze porovnávat s číslem nebo booleanem\n", operator_token->line_num);
        return;
    } else if(first_data_type == 'i' && second_data_type == 'f') {      // i >= f
        fprintf(stdout, "INT2FLOAT %s@%s %s@%s\n", frame_name, first_operand->key,
                                                 frame_name, first_operand->key);
    } else if(first_data_type == 'f' && second_data_type == 'i') {      // f >= f
        fprintf(stdout, "INT2FLOAT %s@%s %s@%s\n", frame_name, second_operand->key,
                                                 frame_name, second_operand->key);
    } else if(first_data_type == 'b' || second_data_type == 'b') {      //b >= ? nebo ? >= b
        EXIT_CODE = 7;
        FREE_EXPR_NAMES_SOFT_COMP
        FREE_EXPR_DATA_TYPES_SOFT_COMP
        fprintf(stderr, "Chyba na řádku: %d\nBoolean hodnotu nelze použít ve výrazu\n", operator_token->line_num);
        return;
    } 

    //generovani kodu
    fprintf(stdout, "%s %s@%s %s@%s %s@%s\n", opcode,
                                            frame_name, ineq_expr_name,
                                            frame_name, first_operand->key,
                                            frame_name, second_operand->key);
    fprintf(stdout, "EQ %s@%s %s@%s %s@%s\n", frame_name, eq_expr_name,
                                            frame_name, first_operand->key,
                                            frame_name, second_operand->key);
    fprintf(stdout, "OR %s@%s %s@%s %s@%s\n", frame_name, final_expr_name,
                                            frame_name, ineq_expr_name,
                                            frame_name, eq_expr_name);
    fprintf(stdout, "\n");

    replace_and_push_exp(4, stack, final_expr_name, operator_token->line_num);
}

void parse_strict_ineq(bst_node_t *first_operand, bst_node_t *second_operand,
                                token_t *operator_token, char frame_name[3],
                                bst_node_t **symb_table, stack_t *stack, bool in_loop) {
    char opcode[2 + 1] = "";    //EQ nebo LS
    char first_data_type = first_operand->data_type[0];
    char second_data_type = second_operand->data_type[0];

    if(operator_token->variant == grt_var) {
        strcpy(opcode, "GT");
    } else {
        strcpy(opcode, "LT");
    }

    //vygenerovani jmena nove promenne, prirazeni datoveho typu
    char *new_expr_name = get_rand_name(symb_table, "_expression");
    char *new_data_type = (char *)calloc(1 + 1, sizeof(char));
    if(EXIT_CODE != 0 || new_data_type == NULL) {
        free (new_expr_name);
        free(new_data_type);
        return;
    }
    new_data_type[0] = 'b';
    
    if((first_data_type == 's' && second_data_type != 's') ||   //str op int/float/null -> chyba
       (second_data_type == 's' && first_data_type != 's')) {
        EXIT_CODE = 7;
        fprintf(stderr, "Chyba v datovém typu operandů na řádku: %d\n%s nebo %s nelze implicitně konvertovat na číslo\n", operator_token->line_num, first_operand->key, second_operand->key);
        free (new_expr_name);
        free(new_data_type);
        return;
    } else if(first_data_type == 'n' || second_data_type == 'n') {  //melo by se priradit false
        BOTUP_DEFVAR_COND(new_expr_name)
        fprintf(stdout, "MOVE %s@%s bool@false\n", frame_name, new_expr_name);

    #define DEFVAR_AND_EXEC_STRICT_COMP\
        BOTUP_DEFVAR_COND(new_expr_name)\
        fprintf(stdout, "%s %s@%s %s@%s %s@%s\n", opcode, frame_name, new_expr_name,\
                                                    frame_name, first_operand->key,\
                                                    frame_name, second_operand->key);

    } else if(first_data_type == 'i' && second_data_type == 'f') {  //konverze int->float
        fprintf(stdout, "INT2FLOAT %s@%s %s@%s\n", frame_name, first_operand->key,
                                                   frame_name, first_operand->key);
        DEFVAR_AND_EXEC_STRICT_COMP
    } else if(first_data_type == 'f' && second_data_type == 'i') { //konverze int->float
        fprintf(stdout, "INT2FLOAT %s@%s %s@%s\n", frame_name, second_operand->key,
                                                   frame_name, second_operand->key);
        DEFVAR_AND_EXEC_STRICT_COMP
    } else if(first_data_type != 'b' && second_data_type != 'b') { //i op i | f op f | s op s
        DEFVAR_AND_EXEC_STRICT_COMP
    } else {    //chyba! nejaky operand je boolean!
        EXIT_CODE = 7;
        fprintf(stderr, "Chyba v datovém typu operandů na řádku: %d\n%s nebo %s nelze implicitně konvertovat na číslo\n", operator_token->line_num, first_operand->key, second_operand->key);
        free (new_expr_name);
        free(new_data_type);
        return;
    }

    bst_insert(symb_table, new_expr_name, var_id, new_data_type); //var_id je enum
    if(EXIT_CODE != 0) {
        free (new_expr_name);
        free(new_data_type);
        return;
    }

    replace_and_push_exp(4, stack, new_expr_name, operator_token->line_num);
    _Pragma("GCC diagnostic pop")
}

/* Funkce pro zpracovani operatoru === a !=== */
void parse_identity(bst_node_t *first_operand, bst_node_t *second_operand,
                                token_t *operator_token, char frame_name[3],
                                bst_node_t **symb_table, stack_t *stack, bool in_loop) {
    //ulozeni, protoze hybeme se stackem pred pouzitim operator_token => bude z nej neco jineho nez cekame
    token_var operator = operator_token->variant;
    int line_num = operator_token->line_num;

    //nagenerovani noveho jmena  pro vysledek operace
    char *new_expr_name = get_rand_name(symb_table, "_expression");
    char *new_data_type = (char *)calloc(1 + 1, sizeof(char));

    if(EXIT_CODE != 0 || new_data_type == NULL) {
        free (new_expr_name);
        return;
    }
    new_data_type[0] = 'b';

    //vlozeni nove promenne do tabulky symbolu
    bst_insert(symb_table, new_expr_name, var_id, new_data_type); //var_id je enum
    if(EXIT_CODE != 0 || new_data_type == NULL) {
        free(new_expr_name);
        free(new_data_type);
        return;
    }

    replace_and_push_exp(4, stack, new_expr_name, line_num);
    if(EXIT_CODE != 0 || new_data_type == NULL) {
        return;
    }

    BOTUP_DEFVAR_COND(new_expr_name)

    if(first_operand->data_type[0] != second_operand->data_type[0]) {
        if(operator == eq_var) {
            fprintf(stdout, "MOVE %s@%s bool@false\n", frame_name, new_expr_name);        
        } else {
            fprintf(stdout, "MOVE %s@%s bool@true\n", frame_name, new_expr_name);        
        }
    } else {    //operatory maji stejny typ, musim porovnat hodnoty
            fprintf(stdout, "EQ %s@%s %s@%s %s@%s\n", frame_name, new_expr_name,
                                                      frame_name, first_operand->key,
                                                      frame_name, second_operand->key);
    }
    return;
}


/* funkce pro zpracovani pravidel E -> E op E pro {+ - * / .} */
void parse_numer_and_conc(bst_node_t *first_operand, bst_node_t *second_operand,
                                token_t *operator_token, char frame_name[3],
                                bst_node_t **symb_table, stack_t *stack, bool in_loop) {
    char first_data_type = first_operand->data_type[0];
    char second_data_type = second_operand->data_type[0];
    char result_data_type = '?';
    char opcode[6 + 1] = "";
    char *new_data_type;
    token_var operator = operator_token->variant;

    _Pragma("GCC diagnostic push")
    _Pragma("GCC diagnostic ignored \"-Wimplicit-fallthrough\"")

    /* provedeni nezbytnych konverzi a urceni druhu operace*/
    switch(operator) {
        case add_oper_var:
            strcpy(opcode, "ADD");
        case sub_oper_var:
            if(strlen(opcode) == 0) {
                strcpy(opcode, "SUB");
            }
        case mul_oper_var:
            if(first_data_type == 'i') {        // i + ?
                result_data_type = 'i'; //pokud se nenamatchuje zadna podminka, druhy operand je int
                if(second_data_type == 'f') {
                    fprintf(stdout, "INT2FLOAT %s@%s %s@%s\n",
                            frame_name, first_operand->key, frame_name, first_operand->key);
                    result_data_type = 'f';
                } else if (second_data_type == 'n') {
                    fprintf(stdout, "MOVE %s@%s int@0\n", frame_name, second_operand->key);
                    result_data_type = 'i';
                } else if (second_data_type != 'i'){
                    EXIT_CODE = 7;
                    fprintf(stderr, "Chyba v datovém typu operandů na řádku: %d\n%s nelze implicitně konvertovat na číslo\n", operator_token->line_num, second_operand->key);
                    return;
                }
            } else if(first_data_type == 'f') { //f + ?
                result_data_type = 'f'; //pokud se nenamatchuje zadna podminka, druhy operand je float
                if(second_data_type == 'i') {   //f + i
                    fprintf(stdout, "INT2FLOAT %s@%s %s@%s\n",
                            frame_name, second_operand->key, frame_name, second_operand->key);
                    result_data_type = 'f';
                } else if (second_data_type == 'n') {   //f + null
                    fprintf(stdout, "MOVE %s@%s float@0x0p+0\n", frame_name, second_operand->key);
                    result_data_type = 'f';
                } else if (second_data_type != 'f'){    //f + string/bool <- chyba
                    EXIT_CODE = 7;
                    fprintf(stderr, "Chyba v datovém typu operandů na řádku: %d\n%s nelze implicitně konvertovat na číslo\n", operator_token->line_num, second_operand->key);
                    return;
                }
            } else if(first_data_type == 'n') {
                if(second_data_type == 'i') {
                    fprintf(stdout, "MOVE %s@%s int@0\n", frame_name, first_operand->key);
                    result_data_type = 'i';
                } else if(second_data_type == 'f') {
                    fprintf(stdout, "MOVE %s@%s float@0x0p+0\n", frame_name, first_operand->key);
                    result_data_type = 'f';
                } else if(second_data_type == 'n') {
                    fprintf(stdout, "MOVE %s@%s int@0\n", frame_name, first_operand->key);
                    fprintf(stdout, "MOVE %s@%s int@0\n", frame_name, second_operand->key);
                    result_data_type = 'i';
                } else {
                    EXIT_CODE = 7;
                    fprintf(stderr, "Chyba v datovém typu operandů na řádku: %d\n%s nelze implicitně konvertovat na číslo\n", operator_token->line_num, second_operand->key);
                    return;
                }
            } else {
                    EXIT_CODE = 7;
                    fprintf(stderr, "Chyba v datovém typu operandů na řádku: %d\n%s nelze implicitně konvertovat na číslo\n", operator_token->line_num, first_operand->key);
                    return;
            }

            if(strlen(opcode) == 0) {
                strcpy(opcode, "MUL");
            }
            break;
        case div_oper_var:
            if(first_data_type == 'i') {
                    fprintf(stdout, "INT2FLOAT %s@%s %s@%s\n",
                            frame_name, first_operand->key, frame_name, first_operand->key);
            }
            if(second_data_type == 'i') {
                    fprintf(stdout, "INT2FLOAT %s@%s %s@%s\n",
                            frame_name, second_operand->key, frame_name, second_operand->key);
            }
            if(first_data_type == 'n') {
                    fprintf(stdout, "MOVE %s@%s float@0x0p+0\n", frame_name, first_operand->key);
            }
            if(second_data_type == 'n') {
                    fprintf(stdout, "MOVE %s@%s float@0x0p+0\n", frame_name, second_operand->key);
            }
            if(first_data_type == 's' || first_data_type == 'b') {
                EXIT_CODE = 7;
                fprintf(stderr, "Chyba v datovém typu operandů na řádku: %d\n%s nelze implicitně konvertovat na číslo\n", operator_token->line_num, first_operand->key);
                return;
            }
            if(second_data_type == 's' || second_data_type == 'b') {
                EXIT_CODE = 7;
                fprintf(stderr, "Chyba v datovém typu operandů na řádku: %d\n%s nelze implicitně konvertovat na číslo\n", operator_token->line_num, second_operand->key);
                return;
            }

            result_data_type = 'f'; //vysledkem deleni je vzdy float
            strcpy(opcode, "DIV");
            break;
        case oper_conc_var:
            if(first_data_type == 'n') {
                    fprintf(stdout, "MOVE %s@%s string@\n", frame_name, first_operand->key);
            }
            if(second_data_type == 'n') {
                    fprintf(stdout, "MOVE %s@%s string@\n", frame_name, second_operand->key);
            }
            if(first_data_type != 's' && first_data_type != 'n') {
                EXIT_CODE = 7;
                fprintf(stderr, "Chyba v datovém typu operandů na řádku: %d\n%s nelze implicitně konvertovat na řetězec\n", operator_token->line_num, first_operand->key);
                return;
            }
            if(second_data_type != 's' && second_data_type != 'n') {
                EXIT_CODE = 7;
                fprintf(stderr, "Chyba v datovém typu operandů na řádku: %d\n%s nelze implicitně konvertovat na řetězec\n", operator_token->line_num, second_operand->key);
                return;
            }
            result_data_type = 's'; //vysledkem konkatenace je vzdy retezec
            strcpy(opcode, "CONCAT");
            break;
        default:
            break;
    }

    //vygenerovani noveho klice pro promennou s vysledkem operace
    char *new_expr_name = get_rand_name(symb_table, "_expression");
    new_data_type = (char *)calloc(1 + 1, sizeof(char));
    MEM_ERR_WR(new_expr_name)
    new_data_type[0] = result_data_type;

    //vlozeni nove promenne do tabulky symbolu
    bst_insert(symb_table, new_expr_name, var_id, new_data_type); //var_id je enum
    MEM_ERR

    //nahrazeni: < E op E -> E
    replace_and_push_exp(4, stack, new_expr_name, operator_token->line_num);
    MEM_ERR

    //generovani kodu
    BOTUP_DEFVAR_COND(new_expr_name)
    fprintf(stdout, "%s %s@%s %s@%s %s@%s\n", 
            opcode,
            frame_name, new_expr_name,
            frame_name, first_operand->key,
            frame_name, second_operand->key);
    fprintf(stdout, "\n");

    return;
}

//bottom up parser error handle, pouze probublane - ze lex analyzy a chyby pameti z ADT
#define BUP_ERR_HANDLE\
                if(EXIT_CODE != 0) {\
                    psa_stack_dispose(stack);\
                    return NULL;\
                }
    
/* Funkce implementujici syntaktickou analyzu zdola nahoru
 * Vraci ukazatel do tabulky symbolu na promennou, do niz
 * byl ulozen vysledek celeho vyrazu*/
bst_node_t *bottom_up_parser(stack_t *global_token_stack,
                        int *global_stack_index,
                        bst_node_t **symb_table,    //tabulka symbolu, ze ktere se ma cerpat
                        bool in_function,           //urcuje, zda-li se ma definovat promenne v lokalnim nebo globalnim frameu - LF/GF
                        bool parsing_assignment,    //urcuje, zda se parsuje prirazeni nebo podminka
                        bool in_loop,
                        const prec_table_t precedence_table[NUM_OF_TOKEN_VARS][NUM_OF_TOKEN_VARS]) {
    stack_t *stack = psa_stack_init();
    token_t *current_input; 

    token_t *top_term_of_stack;     //dle tohoto konci analyza, ignoruje tokeny s expression_var
    token_t *top_token_of_stack;    //pouzite pri ukoncovaci podmince, neignoruje expression

    token_var ending_token;
    prec_table_t table_cell;

    /* nastaveni, do jakeho ramce budou promenne definovany */
    char frame_name[3];
    if(in_function == false) {
        strncpy(frame_name, "GF", 3);
    } else {
        strncpy(frame_name, "LF", 3);
    }

    /* nastavi ukoncovaci varianty tokenu - u prirazeni ';', u podminek '{'*/
    if(parsing_assignment == true) { //bottom up parser zpracovava vyraz v prirazeni
        token_t *end_token = create_token(NULL, semicol_var, 0);
        psa_stack_push(stack, end_token);   //na stack se hodi ; jako ukoncovaci znak
        ending_token = semicol_var;         //analyza konci, kdyz stack_top(term) = ; && vstup = ;
    } else { //bottom up parser zpracovava podminku
        token_t *end_token = create_token(NULL, open_curl_var, 0);
        psa_stack_push(stack, end_token);   //na stack se hodi { jako ukoncovaci znak
        ending_token = open_curl_var;       //analyza konci, kdyz stack_top(term) = { && vstup = {
    }

    current_input = copy_token(global_token_stack, global_stack_index);

    do {
        top_term_of_stack = psa_stack_top_term(stack);

        table_cell = precedence_table[top_term_of_stack->variant][current_input->variant];

        switch(table_cell) {
            case eq:
                psa_stack_push(stack, current_input);
                BUP_ERR_HANDLE;
                current_input = copy_token(global_token_stack, global_stack_index);
                BUP_ERR_HANDLE;
                break;
            case ls:
                psa_stack_split_top(stack); //zameni term! 'a' na vrcholu zasobniku za 'a<'
                BUP_ERR_HANDLE;
                psa_stack_push(stack, current_input);
                BUP_ERR_HANDLE;
                current_input = copy_token(global_token_stack, global_stack_index);
                BUP_ERR_HANDLE;
                break;
            case gr:
                /* snaha najit shodu s nejakym z pravidel */
                ;
                token_t *top_of_stack = psa_stack_get_nth(stack, 0);
                token_t *second_from_top = psa_stack_get_nth(stack, 1);
                token_t *third_from_top = psa_stack_get_nth(stack, 2);
                token_t *fourth_from_top = psa_stack_get_nth(stack, 3);

                if (top_of_stack->variant >= identif_variable_var &&
                top_of_stack->variant <= null_var &&
                second_from_top->variant == less_prec_var) {  // E -> id/int/float/str/null
                    char *new_data_type;
                    char *new_key;
                        
                    /* urceni klice a jmena pro novou pomocnou a jeji vlozeni na zasobnik*/
                    new_key = get_rand_name(symb_table, "_expression");
                    new_data_type = (char *)calloc(1 + 1, sizeof(char));
                    BUP_ERR_HANDLE;

                    switch(top_of_stack->variant) {
                        case integ_var:
                            new_data_type[0] = 'i';
                            break;
                        case float_var:
                            new_data_type[0] = 'f';
                            break;
                        case string_lit_end_var:
                            new_data_type[0] = 's';
                            break;
                        case null_var:
                            new_data_type[0] = 'n';
                            break;
                        case identif_variable_var:
                            ;
                            bst_node_t *from_symb_table = bst_search(*symb_table, top_of_stack->content);
                            if(from_symb_table != NULL && from_symb_table->sym_var == var_id) {
                                new_data_type[0] = from_symb_table->data_type[0];
                            } else {
                                EXIT_CODE = 5;
                                fprintf(stdout, "Chyba na řádku: %d\nPoužití nedefinované proměnné %s\n",
                                        top_of_stack->line_num, top_of_stack->content);
                                psa_stack_dispose(stack);
                                free(new_key);
                                free(new_data_type);
                                return NULL;
                            }
                            break;
                        default:
                            break;
                    }

                    bst_insert(symb_table, new_key, var_id, new_data_type); //var_id je enum
                    if(EXIT_CODE != 0) {
                        psa_stack_dispose(stack);
                        free(new_key);
                        free(new_data_type);
                        return NULL;
                    }

                    /* generovani kodu, podivne poradi (generujeme pred popem), kvuli
                     * nutnosti vytisknout term pred jeho popnutim, u expr neni nutne*/
                    if(in_loop == false) {
                        fprintf(stdout, "DEFVAR %s@%s\n", frame_name, new_key);
                    } else {
                        char *no_redef_label = get_rand_name(symb_table, "_defvar_skip");
                        if(EXIT_CODE != 0) {
                            return NULL;
                        }
                        bst_insert(symb_table, no_redef_label, label_id, NULL);
                        if(EXIT_CODE != 0) {
                            return NULL;
                        }
                        fprintf(stdout, "TYPE %s@%s %s@%s\n", frame_name, NO_REDEF_TYPE_VAR,
                                                              frame_name, new_key);
                        fprintf(stdout, "JMPIFNEQ %s %s@%s string@\n", no_redef_label,
                            frame_name, NO_REDEF_TYPE_VAR);
                        fprintf(stdout, "DEFVAR %s@%s\n", frame_name, new_key);
                        fprintf(stdout, "LABEL %s\n", no_redef_label);
                    }
                    fprintf(stdout, "MOVE %s@%s ", frame_name, new_key); 
                    parse_switch(top_of_stack, frame_name);

                    /* odstraneni <y ze stacku*/
                    psa_stack_pop(stack);
                    psa_stack_pop(stack);
                    BUP_ERR_HANDLE

                    /* zkopirovani new_key do dalsiho stringu, jeden pro tabulku symbolu,
                     * druhy pro zasobnik, zamezeni double free*/
                    char *key_for_expr_token = calloc(strlen(new_key) + 1, sizeof(char)); 
                    BUP_ERR_HANDLE
                    strncpy(key_for_expr_token, new_key, strlen(new_key));
                    psa_stack_push(stack, create_token(key_for_expr_token, expression_var, 0));
                    if(EXIT_CODE != 0) {
                        psa_stack_dispose(stack);
                        free(key_for_expr_token);
                        return NULL;
                    }

                } else if 
                (top_of_stack->variant == cls_rnd_var &&
                second_from_top->variant == expression_var &&
                third_from_top->variant == open_rnd_var &&
                fourth_from_top->variant == less_prec_var) {  // E -> ( E )

                    /* negenerujeme instrukce, pouze zkopirujeme expr token
                     * ze stacku odstranime < ( E ) a zpatky tam natlacime
                     * zkopirovany expr token*/
                    replace_and_push_exp(4, stack, second_from_top->content, second_from_top->line_num);
                    if(EXIT_CODE != 0) {
                        psa_stack_dispose(stack);
                        return NULL;
                    }

                } else if(top_of_stack->variant == expression_var &&
                second_from_top->variant >= add_oper_var && second_from_top->variant <= not_eq_var &&
                third_from_top->variant == expression_var &&
                fourth_from_top->variant == less_prec_var) { // E -> E op E

                    bst_node_t *first_operand = bst_search(*symb_table, third_from_top->content);
                    bst_node_t *second_operand = bst_search(*symb_table, top_of_stack->content);

                    switch(second_from_top->variant) {
                        case add_oper_var:
                        case sub_oper_var:
                        case mul_oper_var:
                        case div_oper_var:
                        case oper_conc_var:
                            parse_numer_and_conc(first_operand, second_operand,
                                second_from_top, frame_name,
                                symb_table, stack, in_loop);
                            BUP_ERR_HANDLE
                            break;
                        case eq_var:
                        case not_eq_var:
                            parse_identity(first_operand, second_operand,
                                second_from_top, frame_name,
                                symb_table, stack, in_loop);
                            BUP_ERR_HANDLE
                            break;
                        case grt_var:
                        case less_var:
                            parse_strict_ineq(first_operand, second_operand,
                                second_from_top, frame_name,
                                symb_table, stack, in_loop);
                            BUP_ERR_HANDLE
                            break;
                        case grt_eq_var:
                        case less_eq_var:
                            parse_soft_ineq(first_operand, second_operand,
                                second_from_top, frame_name,
                                symb_table, stack, in_loop);
                            BUP_ERR_HANDLE
                            break;
                        default:
                            fprintf(stderr, "Neznámý operátor! E op E");
                            break;
                    }
                }
                
                else {  //nynejsi situace neodpovida zadnemu z pravidel = chyba
                    psa_stack_dispose(stack);
                    fprintf(stderr, "Neočekávaný lexém %s ve výrazu na řádku: %d!\n",
                            current_input->content, current_input->line_num);
                    EXIT_CODE = 2;
                    return NULL;
                }
                break;
            case ER:
                psa_stack_dispose(stack);
                fprintf(stderr, "Neočekávaný lexém %s ve výrazu na řádku: %d!\n",
                        current_input->content, current_input->line_num);
                EXIT_CODE = 2;
                return NULL;
        }

    top_token_of_stack = psa_stack_get_nth(stack, 0);
    top_term_of_stack = psa_stack_top_term(stack);

    } while (current_input->variant != ending_token || top_term_of_stack->variant != ending_token || top_token_of_stack->variant != expression_var);

    bst_node_t *returned_node = bst_search(*symb_table, top_token_of_stack->content);
    psa_stack_dispose(stack);
    free(current_input);
    //v pripade uspechu, navraceni ukazatele na promennou, v niz je ulozen vysledek vyrazu
    return returned_node;
}

/*
Funkce pro generovani nahodneho jmena, zatim neobsazeneho v tabulce symbolu, pomocne promenne pro vyrazy.
Nahodne, abychom snizili sanci degenerace stromu, jimz je
reprezentovana tabulka symbolu.
Vraci dynamicky alokovany string "_expressionDDD", kde D je cislo 0-9
*/
char *get_rand_name(bst_node_t **symb_table, char *beginning) {
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

token_t *copy_token(stack_t *global_token_stack, int *global_stack_index) {
    token_t *token_from_global_stack = next_stack_token(global_token_stack, global_stack_index);
    token_t *copy = malloc(sizeof(token_t));
    if(copy == NULL) {
        fprintf(stderr, "Chyba alokace paměti!\n");
        EXIT_CODE = 99;
        return NULL;
    }
    if(token_from_global_stack->content != NULL) {
        copy->content = calloc(strlen(token_from_global_stack->content) + 1, 1);
        strcpy(copy->content, token_from_global_stack->content);
    } else {
        copy->content = NULL; 
    }

    copy->variant = token_from_global_stack->variant;
    copy->line_num = token_from_global_stack->line_num;

    return copy;
}



