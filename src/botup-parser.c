#include "botup-parser.h"

const prec_table_t ass_table[NUM_OF_TOKEN_VARS][NUM_OF_TOKEN_VARS] = {
//       +   -   .   *   /   >   >=  <   <=  === !== id  f   i   s   nil exp (   )   ;   {
/* + */ {gr, gr, gr, ls, ls, gr, gr, gr, gr, gr, gr, ls, ls, ls, ER, ls, ls, ls, gr, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* - */ {gr, gr, gr, ls, ls, gr, gr, gr, gr, gr, gr, ls, ls, ls, ER, ls, ls, ls, gr, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* . */ {gr, gr, gr, ls, ls, gr, gr, gr, gr, gr, gr, ls, ER, ER, ls, ls, ls, ls, gr, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* * */ {gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, ls, ls, ls, ER, ls, ls, ls, gr, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* / */ {gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, ls, ls, ls, ER, ls, ls, ls, gr, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
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
/* + */ {gr, gr, gr, ls, ls, gr, gr, gr, gr, gr, gr, ls, ls, ls, ER, ls, ls, ls, gr, ER, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* - */ {gr, gr, gr, ls, ls, gr, gr, gr, gr, gr, gr, ls, ls, ls, ER, ls, ls, ls, gr, ER, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* . */ {gr, gr, gr, ls, ls, gr, gr, gr, gr, gr, gr, ls, ER, ER, ls, ls, ls, ls, gr, ER, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* * */ {gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, ls, ls, ls, ER, ls, ls, ls, gr, ER, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* / */ {gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, ls, ls, ls, ER, ls, ls, ls, gr, ER, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
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

void parse_switch(token_t *token)
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
		default:
			printf("invalid token variant in parse_switch\n");
			break;
	}
}

int inf_char_conv(char **str, int input)
{
	if((*str) == NULL)
	{
		char *content = (char *)malloc(2);
		if(content == NULL)
		{
			fprintf(stderr, "failed to allocate memory\n");
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
			fprintf(stderr, "failed to allocate memory\n");
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
			snprintf(helper_str, digit_count+2, "\\%s%d", extra_zero,(int)token->content[i]);
			for(int j = 0; helper_str[j] != '\0'; j++)
			{
				if(inf_char_conv(&new_str, helper_str[j]) == -1)
				{
					return;
				}
			}
			free(helper_str);
		}
		else
		{
			if(inf_char_conv(&new_str, token->content[i]) == -1)
			{
				return;
			}
		}
	}
	free(token->content);
	token->content = new_str;
	printf("string@%s\n", token->content);
}

/* funkce pro zpracovani pravidel E -> E op E pro {+ - * / .}
 * TODO vraci true pri uspechu a false pri neuspechu*/
void parse_numer_and_conc(bst_node_t *first_operand, bst_node_t *second_operand,
                                token_var operator, char frame_name[3],
                                bst_node_t **symb_table, stack_t *stack) {
    char first_data_type = first_operand->data_type[0];
    char second_data_type = second_operand->data_type[0];
    char result_data_type = '?';
    char opcode[6 + 1] = "";
    char *new_data_type;

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
                if(second_data_type == 'f') {
                    fprintf(stdout, "INT2FLOAT %s@%s %s@%s\n",
                            frame_name, first_operand->key, frame_name, first_operand->key);
                    result_data_type = 'f';
                } else if (second_data_type == 'n') {
                    fprintf(stdout, "MOVE %s@%s int@0\n", frame_name, second_operand->key);
                    result_data_type = 'i';
                } else {
                    //TODO: zpracuj chybu
                }
            } else if(first_data_type == 'f') {
                if(second_data_type == 'i') {
                    fprintf(stdout, "INT2FLOAT %s@%s %s@%s\n",
                            frame_name, second_operand->key, frame_name, second_operand->key);
                    result_data_type = 'f';
                } else if (second_data_type == 'n') {
                    fprintf(stdout, "MOVE %s@%s float@0x0p+0\n", frame_name, second_operand->key);
                    result_data_type = 'f';
                } else {
                    //TODO: zpracuj chybu
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
                    //TODO: zpracuj chybu
                }
            } else {
                    //TODO: zpracuj chybu
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
            if(first_data_type == 's' || first_data_type == 'b' || 
              second_data_type == 's' || second_data_type == 'b') {
                    //TODO: zpracuj chybu
            }

            result_data_type = 'f';
            strcpy(opcode, "DIV");
            break;
        case oper_conc_var:
            if(first_data_type == 'n') {
                    fprintf(stdout, "MOVE %s@%s string@\n", frame_name, first_operand->key);
            }
            if(second_data_type == 'n') {
                    fprintf(stdout, "MOVE %s@%s string@\n", frame_name, second_operand->key);
            }
            if((first_data_type != 's' && first_data_type != 'n') ||
              (second_data_type != 's' && second_data_type != 'n')) {
                    //TODO: zpracuj chybu
            }
            strcpy(opcode, "CONCAT");
            break;
        default:
            break;
    }

    //vygenerovani noveho klice pro promennou s vysledkem operace
    char *new_expr_name = get_rand_var_name(symb_table);
    new_data_type = (char *)calloc(1 + 1, sizeof(char));
    new_data_type[0] = result_data_type;

    //vlozeni nove promenne do tabulky symbolu
    bst_insert(symb_table, new_expr_name, var_id, new_data_type); //var_id je enum

    //odstraneni < E op E z vrcholu zasobniku
    psa_stack_pop(stack);
    psa_stack_pop(stack);
    psa_stack_pop(stack);
    psa_stack_pop(stack);

    /* zkopirovani new_data_type do dalsiho stringu, jeden pro tabulku symbolu,
     * druhy pro zasobnik, zamezeni double free*/
    char *key_for_expr_token = calloc(strlen(new_expr_name) + 1, sizeof(char)); 
    strncpy(key_for_expr_token, new_expr_name, strlen(new_expr_name));
    psa_stack_push(stack, create_token(key_for_expr_token, expression_var, 0));

    //generovani kodu
    fprintf(stdout, "DEFVAR %s@%s\n", frame_name, new_expr_name);
    fprintf(stdout, "%s %s@%s %s@%s %s@%s\n", 
            opcode,
            frame_name, new_expr_name,
            frame_name, first_operand->key,
            frame_name, second_operand->key);
    fprintf(stdout, "\n");

    return;
}

void bottom_up_parser(token_t *from_top_down,       //token, kterym ma zacit analyza, vzdy
                        bst_node_t **symb_table,    //tabulka symbolu, ze ktere se ma cerpat
                        bool in_function,           //urcuje, zda-li se ma definovat promenne v lokalnim nebo globalnim frameu - LF/GF
                        bool parsing_assignment,    //urcuje, zda se parsuje prirazeni nebo podminka
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

    current_input = from_top_down;  //prvni token je vzdy predan shora

    do {
        top_term_of_stack = psa_stack_top_term(stack);

        table_cell = precedence_table[top_term_of_stack->variant][current_input->variant];

        switch(table_cell) {
            case eq:
                psa_stack_push(stack, current_input);
                current_input = get_token();
                break;
            case ls:
                psa_stack_split_top(stack); //zameni term! 'a' na vrcholu zasobniku za 'a<'
                psa_stack_push(stack, current_input);
                current_input = get_token();
                break;
            case gr:
                /* snaha najit shodu s nejakym z pravidel */
                ;
                token_t *top_of_stack = psa_stack_get_nth(stack, 0);
                token_t *second_from_top = psa_stack_get_nth(stack, 1);
                token_t *third_from_top = psa_stack_get_nth(stack, 2);
                token_t *fourth_from_top = psa_stack_get_nth(stack, 3);

                if (top_of_stack->variant >= identif_variable_var &&
                top_of_stack->variant <= expression_var &&
                second_from_top->variant == less_prec_var) {  // E -> id/int/float/str/null
                    char *new_data_type;
                    char *new_key;
                        
                    /* urceni klice a jmena pro novou pomocnou a jeji vlozeni na zasobnik*/
                    new_key = get_rand_var_name(symb_table);
                    new_data_type = (char *)calloc(1 + 1, sizeof(char));
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
                                fprintf(stdout, "POMOC! PROMENNA NEEXISTUJE V TABULCE SYMBOLU!!!\n");
                                //chyba, referovana promenna neexistuje, spachej sebevrazdu
                            }
                            break;
                        default:
                            break;
                    }
                    bst_insert(symb_table, new_key, var_id, new_data_type); //var_id je enum

                    /* generovani kodu */
                    fprintf(stdout, "DEFVAR %s@%s\n", frame_name, new_key); 
                    fprintf(stdout, "MOVE %s@%s ", frame_name, new_key); 
                    parse_switch(top_of_stack);
                    /* odstraneni <y ze stacku*/
                    psa_stack_pop(stack);
                    psa_stack_pop(stack);
                    /* zkopirovani new_key do dalsiho stringu, jeden pro tabulku symbolu,
                     * druhy pro zasobnik, zamezeni double free*/
                    char *key_for_expr_token = calloc(strlen(new_key) + 1, sizeof(char)); 
                    strncpy(key_for_expr_token, new_key, strlen(new_key));
                    psa_stack_push(stack, create_token(key_for_expr_token, expression_var, 0));
                } else if 
                (top_of_stack->variant == cls_rnd_var &&
                second_from_top->variant == expression_var &&
                third_from_top->variant == open_rnd_var &&
                fourth_from_top->variant == less_prec_var) {  // E -> ( E )

                    /* negenerujeme instrukce, pouze zkopirujeme expr token
                     * ze stacku odstranime < ( E ) a zpatky tam natlacime
                     * zkopirovany expr token*/
                    size_t expr_cont_len = strlen(second_from_top->content);
                    char *expr_cont_copy = calloc(expr_cont_len + 1, sizeof(char)); 
                    strncpy(expr_cont_copy, second_from_top->content, expr_cont_len);
                    /* odstraneni < ( E ) ze stacku*/
                    psa_stack_pop(stack);
                    psa_stack_pop(stack);
                    psa_stack_pop(stack);
                    psa_stack_pop(stack);
                    /* natlaceni noveho E na zasobnik*/
                    psa_stack_push(stack, create_token(expr_cont_copy, expression_var, 0));

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
                                second_from_top->variant, frame_name,
                                symb_table, stack);
                            break;
                        default:
                            break;
                    }
                }

                break;
            case ER: //chyba!!!!
                //return?
                break;
        }

    top_token_of_stack = psa_stack_get_nth(stack, 0);
    top_term_of_stack = psa_stack_top_term(stack);

    } while (current_input->variant != ending_token || top_term_of_stack->variant != ending_token || top_token_of_stack->variant != expression_var);

    //sdelit top_down parseru v jake promenne je ulozen vysledek vyrazu
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
