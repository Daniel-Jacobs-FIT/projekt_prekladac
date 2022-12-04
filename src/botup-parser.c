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

int inf_char_conv(char **str, int input)
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
                inf_char_conv(&new_str, helper_str[j]);
				if(EXIT_CODE != 0)
				{
					return;
				}
			}
			free(helper_str);
		}
		else
		{
            inf_char_conv(&new_str, token->content[i]);
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

/* Funkce pro zpracovani operatoru === a !=== */
void parse_identity(bst_node_t *first_operand, bst_node_t *second_operand,
                                token_t *operator_token, char frame_name[3],
                                bst_node_t **symb_table, stack_t *stack) {
    //vygenerovani noveho klice pro promennou s vysledkem operace
    /* Co delat?
     * - kontrola typu:
     *   stejne u === -> dalsi praca
     *   jine   u === -> do nove promenne false
     *   stejne u !== -> dalsi praca
     *   jine   u !== -> do nove promenne true
     * - dalsi praca?
     *   porovnani instrukci EQ,
     *   do nove promenne se zapise vysledek
     * - vloz novou promennou do tabulky symbolu
     * - popni 4 tokeny ze stacku
     * - vloz na stack novou expression s typem bool*/
    /* Napady:
     *      nikdy nevyhodi chybu => nova promenna se generuje vzdy */

    //ulozeni, protoze hybeme se stackem pred pouzitim operator_token => bude z nej neco jineho nez cekame
    token_var operator = operator_token->variant;
    int line_num = operator_token->line_num;

    //nagenerovani noveho jmena  pro vysledek operace
    char *new_expr_name = get_rand_var_name(symb_table);
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

    fprintf(stdout, "DEFVAR %s@%s\n", frame_name, new_expr_name);

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
                                bst_node_t **symb_table, stack_t *stack) {
    char first_data_type = first_operand->data_type[0];
    char second_data_type = second_operand->data_type[0];
    char result_data_type = '?';
    char opcode[6 + 1] = "";
    char *new_data_type;
    token_var operator = operator_token->variant;

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
    char *new_expr_name = get_rand_var_name(symb_table);
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
    fprintf(stdout, "DEFVAR %s@%s\n", frame_name, new_expr_name);
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
                    return;\
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
                BUP_ERR_HANDLE;
                current_input = get_token();
                BUP_ERR_HANDLE;
                break;
            case ls:
                psa_stack_split_top(stack); //zameni term! 'a' na vrcholu zasobniku za 'a<'
                BUP_ERR_HANDLE;
                psa_stack_push(stack, current_input);
                BUP_ERR_HANDLE;
                current_input = get_token();
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
                    new_key = get_rand_var_name(symb_table);
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
                                return;
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
                        return;
                    }

                    /* generovani kodu, podivne poradi (generujeme pred popem), kvuli
                     * nutnosti vytisknout term pred jeho popnutim, u expr neni nutne*/
                    fprintf(stdout, "DEFVAR %s@%s\n", frame_name, new_key); 
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
                        return;
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
                        return;
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
                                symb_table, stack);
                            BUP_ERR_HANDLE
                            break;
                        case eq_var:
                        case not_eq_var:
                            parse_identity(first_operand, second_operand,
                                second_from_top, frame_name,
                                symb_table, stack);
                            BUP_ERR_HANDLE
                            break;
                        default:
                            break;
                    }
                }
                
                else {  //nynejsi situace neodpovida zadnemu z pravidel = chyba
                    psa_stack_dispose(stack);
                    fprintf(stderr, "Neočekávaný lexém %s ve výrazu na řádku: %d!\n",
                            current_input->content, current_input->line_num);
                    EXIT_CODE = 2;
                    return;
                }
                break;
            case ER:
                psa_stack_dispose(stack);
                fprintf(stderr, "Neočekávaný lexém %s ve výrazu na řádku: %d!\n",
                        current_input->content, current_input->line_num);
                EXIT_CODE = 2;
                return;
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
