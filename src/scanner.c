//#include <string.c> //docasne pro vyvoj string.c
#include <string.h>
#include <math.h>
#include "scanner.h"

const char *OPEN_SIGN = "<?php";
const char *TOKEN_VAR_NAMES[] = {ALL_TOKEN_VARS}; //TODO
const int KEYWORD_COUNT = 13;
const char *keywords[] = {"else", "float", "?float", "function", "if", "int", "?int", "null", "return", "string", "?string", "void", "while"};
static char buffer[5];
static int decimal = 0;
static int string_buffer_count = 0;

/*makro pro vypsani chybove hlasky pri ziskani neznameho znaku*/
#define ERR_CASE(NAME)\
{\
	token->variant = err_var;\
	state_err(NAME, input, token->line_num);\
	return default_s;\
}

/*makro pro testovani jestli prosel malloc nebo realloc*/
#define MALLOC_CHECK(NAME)\
if(NAME == NULL)\
{\
	fprintf(stderr, "failed to allocate memory\n");\
	return 1;\
}

int inf_char_input(int input, token_t *token)
{
	if(token->content == NULL)
	{
		char *content = (char *)malloc(2);
		MALLOC_CHECK(content);
		content[0] = (char)input;
		content[1] = '\0';
		token->content = content;
	}
	else
	{
		int str_size = strlen(token->content);
		char *content = (char *)realloc((void *)token->content, str_size+2);
		MALLOC_CHECK(content);
		content[str_size] = (char)input;
		content[str_size+1] = '\0';
		token->content = content;
	}
	return 0;
}

/*
funkce na vypisovani error message pri ziskani neplatneho charu
	param: fce_name -> jmeno funkce ve ktere byl char precten
	param: err_char -> character ktery zpusobil chybu
	param: line_num -> cislo radku na kterem byla chyba zpusobena
*/
void state_err(char *fce_name, int err_char, int line_num)
{
	printf("Error: in function %s: unexpected ", fce_name);
	if(err_char == EOF)
	{
		printf("EOF at line %d\n", line_num);
	}
	else if(err_char == 10)
	{
		printf("'new line' at line %d\n", line_num);
	}
	else
	{	
		printf("char '%c' at line %d\n", err_char, line_num);
	}
}

/* Funkce pro logiku fsm */
scanner_state_t float_e_num_logic(int input, token_t *token)
{
	if(inf_char_input(input, token) != 0)
	{
		//memory allocation failed
		return -1;
	}

	input = getc(stdin);
	if(input >= '0' && input <= '9')
	{
		ungetc(input, stdin);
		return float_e_num_s;
	}else
	{
		token->variant = float_e_num_var;	
		ungetc(input, stdin);
		return default_s;
	}
}

scanner_state_t float_e_sign_logic(int input, token_t *token)
{
	if(inf_char_input(input, token) != 0)
	{
		//memory allocation failed
		return -1;
	}

	input = getc(stdin);
	if(input >= '0' && input <= '9')
	{
		ungetc(input, stdin);
		return float_e_num_s;
	}
	else
	{
		ERR_CASE("float_e_sign_logic");
	}
}

scanner_state_t float_e_logic(int input, token_t *token)
{
	if(inf_char_input(input, token) != 0)
	{
		//memory allocation failed
		return -1;
	}

	input = getc(stdin);
	switch(input)
	{
		case '+':
		case '-':
			ungetc(input, stdin);
			return float_e_sign_s;
		default:
			if(input >= '0' && input <= '9')
			{
				ungetc(input, stdin);
				return float_e_num_s;
			}
			else
			{
				ERR_CASE("float_e_logic");
			}
	}
}

scanner_state_t float_dot_num_logic(int input, token_t *token)
{
	if(inf_char_input(input, token) != 0)
	{
		//memory allocation failed
		return -1;
	}
	input = getc(stdin);
	switch(input)
	{
		case 'E':
		case 'e':
			ungetc(input, stdin);
			return float_e_s;
		default:
			if(input >= '0' && input <= '9')
			{
				ungetc(input, stdin);
				return float_dot_num_s;
			}else
			{
				token->variant = float_dot_num_var;	
				ungetc(input, stdin);
				return default_s;
			}
	}
}
scanner_state_t float_dot_logic(int input, token_t *token)
{
	if(inf_char_input(input, token) != 0)
	{
		//memory allocation failed
		return -1;
	}
	
	input = getc(stdin);
	if(input >= '0' && input <= '9')
	{
		ungetc(input, stdin);
		return float_dot_num_s;
	}else
	{
		ERR_CASE("float_dot_logic");
	}
}

scanner_state_t integ_logic(int input, token_t *token)
{
	if(inf_char_input(input, token) != 0)
	{
		//memory allocation failed
		return -1;
	}

	input = getc(stdin);
	switch(input)
	{
		case '.':
			ungetc(input, stdin);
			return float_dot_s;
		case 'e':
		case 'E':
			ungetc(input, stdin);
			return float_e_s;
		default:
			if(input >= '0' && input <= '9')
			{
				ungetc(input, stdin);
				return integ_s;
			}else
			{
				token->variant = integ_var;	
				ungetc(input, stdin);
				return default_s;
			}
	}
}


scanner_state_t identif_logic(int input, token_t *token)
{
	if(inf_char_input(input, token) != 0)
	{
		//memory allocation failed
		return -1;
	}

	input = getc(stdin);
	if((input >= 'A' && input <= 'Z')  || (input >= 'a' && input <= 'z') || (input >= '0' && input <= '9') || input == '_')
	{
		ungetc(input, stdin);
		return identif_s;
	}
	else
	{
		if(token->content[0] != '$')
		{
			bool is_keyword = false;
			for(int i = 0; i < KEYWORD_COUNT; i++)
			{
				if(strcmp(token->content, keywords[i]) == 0)
				{
					is_keyword = true;
					break;
				}
			}
			if(is_keyword == false)
			{
				printf("Error: in function identif_logic: expected keyword instead of '%s'\n", token->content);
				token->variant = err_var;
				return default_s;
			}
		}
		token->variant = identif_var;
		ungetc(input, stdin);
		return default_s;
	}
}

scanner_state_t expect_eof_logic(token_t *token)
{
	int input = getc(stdin);
	switch(input)
	{
		case EOF:
			return end_prg_s;
		default:
			ERR_CASE("expect_eof_logic");
	}
}

scanner_state_t end_prg_logic(token_t *token)
{
	token->variant = end_prg_var;
	return default_s;
}

scanner_state_t end_sign_logic(token_t *token)
{
	int input = getc(stdin);
	switch(input)
	{
		case EOF:
			return end_prg_s;
		case 10: //eol
			ungetc(input, stdin);
			return expect_eof_s;
		default:
			ERR_CASE("end_sign_logic");	
	}
}

scanner_state_t id_or_end_logic(int input, token_t *token)
{
	if(inf_char_input(input, token) != 0)
	{
		//memory allocation failed
		return -1;
	}

	input = getc(stdin);
	switch(input)
	{
		case '>':
			ungetc(input, stdin);
			free(token->content);
			token->content = NULL;
			return end_sign_s;
		case '_':
			ungetc(input, stdin);
			return identif_s;
		default:
			if((input >= 'A' && input <= 'Z')  || (input >= 'a' && input <= 'z') || (input >= '0' && input <= '9'))
			{
				ungetc(input, stdin);
				return identif_s;
			}
			else
			{
				ERR_CASE("id_or_end_logic");
			}
	}
}

scanner_state_t default_logic(int input, token_t *token)
{
	int cmp = input;
	ungetc(input, stdin);
	switch(cmp)
	{
		case '/':
			return div_oper_s;
		case '+':
		case '-':
		case '*':
			return num_oper_s;
		case '.':
			return oper_conc_s;
		case '!':
			return not_eq1_s;
		case '=':
			return eq_or_assign_s;
		case '>':
			return grt_s;
		case '<':
			return less_s;
		case '(':
			return open_rnd_s;
		case ')':
			return cls_rnd_s;
		case '{':
			return open_curl_s;
		case ';':
			return semicol_s;
		case '}':
			return cls_curl_s;
		case '"':
			return string_lit_s;
		case '_' :
		case '$' :
			return identif_s;
		case '?':
			return id_or_end_s;
		case EOF:
			return end_prg_s;
		case 10: //eol
			token->line_num++;
			getc(stdin);
			return default_s;
		case 9: //tab
		case ' ':
			getc(stdin);
			return default_s;
		default:
			if((cmp >= 'A' && cmp <= 'Z') || (cmp >= 'a' && cmp <= 'z'))
			{	
				return identif_s;
			}
			else if(cmp >= '0' && cmp <= '9')
			{
				return integ_s;
			}else
			{
				ERR_CASE("default_logic");
			}
	}
}

/* Funkce pro overeni, ze standartni vstup na zacatku obsahuje znaky SIGN
 * sign = znacka ke kontrole 
 * */
bool string_check(char *sign) {
    int sign_len = strlen(sign);
    ++sign_len; //inkrementace, aby se terminalni znak \0 vesel do bufferu

    char buffer[sign_len];

    fgets(buffer, sign_len, stdin);
    
    if(!strcmp(sign, buffer)) {
        return true; 
    } else {
        return false;
    }
}
//EDIT: zmena input z char na int, aby byl zachycen EOF
void string_buffer(token_t token, int *input) {

    //string_buffer_count = (string_buffer_count + 1) % 5;
    buffer[string_buffer_count % 5] = *input;
    if((string_buffer_count % 5) == 4) {
        if(realloc(token.content, sizeof(char) * 5) == NULL)
		{} //EDIT: oprava gcc warningu -Wempty-body 

        for(int i = 0; i < 5; i++) {
            printf("%d, %s\n", i, (char *)input);//EDIT: cast na (char *) aby se input vytiskl pres %s
            token.content[string_buffer_count - 4 + i] = buffer[i];
        }
    }
    
    string_buffer_count++;

}

int to_decimal(char input, int type, int position) {
    int base;

    if(type == 16)
        base = 16;
    if(type == 8)
        base = 8;

    if(input >= '0' && input <= '9') {
        decimal += (input - 48) * pow(base, position);
    }
    else if(input >= 'A' && input <= 'F') {
        decimal += (input - 55) * pow(base, position);
    }
    else if(input >= 'a' && input <= 'f') {
        decimal += (input - 87) * pow(base, position);
    }
    return decimal;
}

/* Funkce implementujici automat ridici lexikalni analyzu
 * input = znak nacteny ze vstupu
 * token = token, do ktereho se ma zapsat lexem
 * */
scanner_state_t fsm_step(int input, token_t *token) {
    static scanner_state_t fsm_state = default_s;

    switch(fsm_state) {
        case default_s :
			fsm_state = default_logic(input, token);
			break;
         case div_oper_s :
            if(input == '/'){
                fsm_state=com_oneline_s;
                break;
            }
            else if(input == '*') {
                fsm_state=com_block_s;
                break;
            }
            else 
                token->variant=div_oper;
                break;
        case com_oneline_s : 
           if(input == 'EOL'){
                token->variant=none;
                break;
           }
            else{
               fsm_state=com_oneline_s;
            }
        case com_block_s : 
            if(input == '*'){
                fsm_state=com_block_end_s;
                break;
            }
            else{
                fsm_state=com_block_s;
                break;
            }
        case com_block_end_s :
            if(input == '/'){
                 token->variant=none;
                 break;
            }
            fsm_state=com_block_end_s;
            break;
           
        case num_oper_s :
           token->variant=num_oper_var;
            break;
        case oper_conc_s :
             token->variant=oper_conc_var;
            break;
        case not_eq1_s :
            if(input == '='){
                fsm_state=not_eq2_s;
                break;
            }
            else{
                state_err(input,token->line_num);
                break;
            }
        case not_eq2_s :
            if(input == '='){
                fsm_state=not_eq3_s;
                break;
            }
            else{
                state_err(input,token->line_num);
                break;
            }
        case not_eq3_s :
            token->variant=not_eq_var;
            break;
        case eq_or_assign_s :
            if(input == '='){
                fsm_state=eq2_s;
                break;
            }
            else{
                token->variant=eq_or_assign_var;
                break;
            }
        case eq2_s :
            if(input == '='){
                fsm_state=eq3_s;
                break;
            }
            else{
                state_err(input,token->line_num);
                break;
            }
        case eq3_s :
            token->variant=eq_var;
            break;
        case grt_s :
            if(input == '='){
                fsm_state=grt_eq_s;
                break;
            }
            else{
                token->variant=grt_var;
                break;
            }
            
        case grt_eq_s :
            token->variant=grt_eq_var   ;
            break;
        case less_s :
            if(input == '='){
                fsm_state=less_eq_s;
                break;
            }
            else{
               token->variant=less_var;
               break; 
            }
        case less_eq_s :
            token->variant=less_eq_var;
            break;
        case open_rnd_s :
            token->variant=open_rnd_var;
            break;
        case cls_rnd_s :
            token->variant=cls_rnd_var;
            break;
        case open_curl_s :
            token->variant=open_curl_var;
            break;
        case cls_curl_s :
            token->variant=cls_curl_var;
            break;
        case semicol_s :
            token->variant=semicol_var;
            break;
        case string_lit_end_s :
            token->variant = string_lit_end_var;
			fsm_state = default_s;
            break;
        case string_lit_s :
            if(input == '"') {
                fsm_state = string_lit_end_s;
                break;
            }
            else if(input == '\\'){
                fsm_state = esc_char_s;
            }
            else if((input >= 'A' && input <= 'Z') || (input >= 'a' && input <= 'z') || (input >= '0' && input <= '9') ) { //alfanumericke znaky
				if(inf_char_input(input, token) != 0)
					return -1;
			}
			else {
				ERR_CASE("Invalid characters");
			} 	
            break;
            
		//ESCAPE SEQUENCES
        case esc_char_s :
            if(strchr("nrtve\\$", input) != NULL){ //escape sequence

				switch(input)
				{
					case 'n': 
						inf_char_input(10, token); //newline
						break;
					case 'r': //????;
						break;
					case 't': 
						inf_char_input(9, token); //tab
						break;
					case 'v': 
						inf_char_input(input, token); //WIP
						break;
					case 'e': 
						inf_char_input(input, token); //WIP
						break;
					case '\\': 
						inf_char_input(input, token);
						break;
					case '$': 
						inf_char_input(input, token); 
						break;
				}
                fsm_state = esc_seq_s;
                break;
            }
            else if(strchr("x", input) != NULL) { //hex sequence
                fsm_state = hex1_s;
                decimal = 0;
                break;
            }
            else if(strchr("0123", input) != NULL){ //octal sequence
                fsm_state = oct1_s;
                decimal = 0;
                to_decimal(input, 8, 2);
                break;
            }
            else {
                fsm_state = string_lit_s;
                break;
            }
        case esc_seq_s :
            if(input == '"') {
                fsm_state = string_lit_end_s;
                break;
            }
            else {
                fsm_state = string_lit_s;
                break;
            }
        case hex1_s :
            if(input == '"') {
                fsm_state = string_lit_end_s;
                break;
            }
            else if (strchr("0123456789aAbBcCdDeEfF", input) != NULL) {
                //to_decimal(input, 16, 1);
                fsm_state = hex2_s;
                break;
            }
            else {
                fsm_state = string_lit_s;
                break;
            }
        case hex2_s :
             if(input == '"') {
                fsm_state = string_lit_end_s;
                break;
            }
            else if (strchr("123456789aAbBcCdDeEfF", input) != NULL) {
                fsm_state = string_lit_s;
				to_decimal(input, 16, 0);
				if(inf_char_input(decimal, token) != 0)
					return -1;
				printf("%d\n", decimal);
                break;
            }
            else {
                fsm_state = string_lit_s;
                break;
            }
        case oct1_s :
            if(input == '"') {
                fsm_state = string_lit_end_s;
                break;
            }
            else if (strchr("01234567", input) != NULL) {
                to_decimal(input, 8, 1);
                fsm_state = oct2_s;
                break;
            }
            else {
                fsm_state = string_lit_s;
                break;
            }
        case oct2_s :
            if(input == '"') {
                fsm_state = string_lit_end_s;
                break;
            }
            else if (strchr("01234567", input) != NULL) {
                printf("%d\n", to_decimal(input, 8, 0));
                fsm_state = string_lit_s;
                break;
            }
            else {
                fsm_state = string_lit_s;
                break;
            }
        case integ_s :
			fsm_state = integ_logic(input, token);
            break;
        case float_dot_s :
			fsm_state = float_dot_logic(input, token);
            break;
        case float_dot_num_s :
			fsm_state = float_dot_num_logic(input, token);
            break;
        case float_e_s :
			fsm_state = float_e_logic(input, token);
            break;
        case float_e_sign_s :
            fsm_state = float_e_sign_logic(input, token);
			break;
        case float_e_num_s :
            fsm_state = float_e_num_logic(input, token);
            break;
        case id_or_end_s :
			fsm_state = id_or_end_logic(input, token);
            break;
        case identif_s :
			fsm_state = identif_logic(input, token);
            break;
        case end_sign_s :
			fsm_state = end_sign_logic(token);
            break;
		case expect_eof_s :
			fsm_state = expect_eof_logic(token);
			break;
        case end_prg_s :
			fsm_state = end_prg_logic(token);
            break;
        //TODO lze zjednodusit -> sloucit stavy, obsluha EOF, obsluha chyby, ...
    }  
    return fsm_state;
}

/* Funkce pro parser, na pozadani cte vstup, dokud neprecte dost informaci
 * pro vytvoreni tokenu, ktery vraci
 * */
token_t get_token() {
    int input_char = getc(stdin);
    static int line_counter = 1;

    if(input_char == '\n') ++line_counter; 

    token_t current_token = create_token(NULL, none, 0);
	ungetc(input_char, stdin);
    while(current_token.variant == none) {
        input_char = getc(stdin);
        fsm_step(input_char, &current_token);
    }

    current_token.line_num = line_counter;
    return current_token;
}

/* Funkce pro praci s tokeny*/

/* Funkce pro vytvoreni tokenu - argumenty funkce budou jednotlive polozky tokenu
 * Pro zvyseni citelnosti kodu pri tvorbe tokenu
 * content = obsah tokenu (nactene symboly)
 * token_var = varianta tokenu
 * linu_num = cislo radku, na kterem byl token nacteny
 * */
token_t create_token(char* content, token_var variant, int line_num) {
    token_t token;
    token.content = content;
    token.variant = variant;
    token.line_num = line_num;
    return token;
}

/* Funkce pro vypsani tokenu, pro debugovaci ucely
 * token = ukazatel na token, ktery se ma vypsat
 * */
void print_token(token_t *token) {
    if(token->content == NULL)
	{
		printf("Token:\"\" Variant:%s Line:%d\n", TOKEN_VAR_NAMES[token->variant], token->line_num); 
	}else
	{
		printf("Token:\"%s\" Variant:%s Line:%d\n", token->content, TOKEN_VAR_NAMES[token->variant], token->line_num); 
	}
}

/* Funkce pro uvolneni pameti na heapu zabirane tokenem
 * token = ukazatel na token, ktery se ma  uvolnit
 * */
void free_token(token_t *token) {
    free(token->content);
    token->content = NULL;
    token->variant = none;
    token->line_num = 0;
}
