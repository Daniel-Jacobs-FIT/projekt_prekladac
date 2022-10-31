#include "scanner.h"

const char *OPEN_SIGN = "<?php";
const char *TOKEN_VAR_NAMES[] = {ALL_TOKEN_VARS}; //TODO

/*
funkce na vypisovani error message pri ziskani neplatneho charu
	param: err_char -> character ktery zpusobyl chybu
	param: line_num -> cislo radku na kterem byla chyba zpusobena
*/
void state_err(char err_char, int line_num)
{
	if(err_char == EOF)
	{
		printf("Error: unexpected EOF at line %d\n", line_num);
	}
	else
	{
		printf("Error: unexpected char '%c' at line %d\n", err_char, line_num);
	}
}

/* Funkce pro logiku fsm */
scanner_state_t end_prg_logic(token_t *token)
{
	token->variant = end_prg_var;
	return default_s;
}

scanner_state_t end_sign_logic(char input, token_t *token)
{
	int content_lenght = 0;
	switch(input)
	{
		case '>':
			//strlen() vraci delku stringu minus '\0' a proto musime pridat 2 chary
			content_lenght = strlen(token->content);
			char *new_content =(char *)realloc((void *)token->content, content_lenght+2); 
			if(new_content == NULL)
			{
				fprintf(stderr, "failed to allocate memory with realloc\n");
				return -1;
			}
			token->content = new_content;
			token->content[content_lenght+1] = input;
			token->content[content_lenght+2] = '\0';
			return end_sign_s;
		case EOF:
			ungetc((int)input, stdin);
			return end_prg_s;
		default:
			//error case
			token->variant = err_var;
			state_err(input, token->line_num);
			return default_s;
			
	}
}

scanner_state_t id_or_end_logic(char input, token_t *token)
{
	//can be id (starts with aA-zZ or '_'), variable (	
	if(token->content == NULL)
	{
		//malloc(2) protoze sizeof(char) + '?'	
		char *content = (char *)malloc(2);
		if(content == NULL)
		{
			//malloc fail;
			fprintf(stderr, "failed to initialize memory with malloc\n");
			return -1;
		}
		content[1] = '\0';
		content[0] = input;
		token->content = content;
		return id_or_end_s;
	}
	switch(input)
	{
		case '>':
			return end_sign_s;
			break;
		case '_':
			return identif_s;
			break;
		default:
			if((input >= 'A' && input <= 'Z')  || (input >= 'a' && input <= 'z') || (input >= '0' && input <= '9'))
			{
				ungetc((int)input, stdin);
				return identif_s;
			}
			else
			{
				//error case
				token->variant = err_var;
				state_err(input, token->line_num);
				return default_s;
			}
	}
}

scanner_state_t integ_logic(char input, token_t *token)
{
	//faster than always looking at strlen()
	static int next_index = 0;
	
	if(token->content == NULL)
	{
		//malloc(1) protoze sizeof(char) je 1
		char *content = (char *)malloc(1);
		if(content == NULL)
		{
			//malloc fail;
			fprintf(stderr, "failed to initialize memory with malloc\n");
			return -1;
		}
		// start arr: [\0]
		content[0] = '\0';
		token->content = content;
	}
	switch(input)
	{
		case '.':
			return float_dot_s;
		case 'E':
		case 'e':
			return float_e_s;
		default:
			if(input >= '0' && input <= '9')
			{
				//next_index+2 protoze indexujeme od 0
				char *new_content =(char *)realloc((void *)token->content, next_index+2); 
				if(new_content == NULL)
				{
					fprintf(stderr, "failed to allocate memory with realloc\n");
					return -1;
				}
				token->content = new_content;
				
				token->content[next_index] = input;

				//next_index+1 vzdy ukazuje na posledni misto ve stringu	
				token->content[next_index+1] = '\0';
				return integ_s;
			}
			else
			{
				token->variant = integ_var;
				ungetc((int)input, stdin);
				
				next_index = 0;
				return default_s;
			}
	}
}


scanner_state_t default_logic(char input, token_t *token)
{
	char cmp = input;
	ungetc((int)input, stdin);
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
		case 9: //tab
		case ' ':
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
				//error case
				token->variant = err_var;
				state_err(cmp, token->line_num);
				return default_s;
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

/* Funkce implementujici automat ridici lexikalni analyzu
 * input = znak nacteny ze vstupu
 * token = token, do ktereho se ma zapsat lexem
 * */
scanner_state_t fsm_step(char input, token_t *token) {
    static scanner_state_t fsm_state = default_s;

    switch(fsm_state) {
        case default_s :
			fsm_state = default_logic(input, token);
			break;
        case div_oper_s :
            break;
        case com_oneline_s : 
            break; 
        case com_block_s : 
            break;
        case com_block_end_s :
            break;
        case num_oper_s :
            break;
        case oper_conc_s :
            break;
        case not_eq1_s :
            break;
        case not_eq2_s :
            break;
        case not_eq3_s :
            break;
        case eq_or_assign_s :
            break;
        case eq2_s :
            break;
        case eq3_s :
            break;
        case grt_s :
            break;
        case grt_eq_s :
            break;
        case less_s :
            break;
        case less_eq_s :
            break;
        case open_rnd_s :
            break;
        case cls_rnd_s :
            break;
        case open_curl_s :
            break;
        case cls_curl_s :
            break;
        case semicol_s :
            break;
        case string_lit_end_s :
            break;
        case string_lit_s :
            break;
        case esc_char_s :
            break;
        case esc_seq_s :
            break;
        case hex1_s :
            break;
        case hex2_s :
            break;
        case oct1_s :
            break;
        case oct2_s :
            break;
        case integ_s :
			fsm_state = integ_logic(input, token);
            break;
        case float_dot_s :
            break;
        case float_dot_num_s :
            break;
        case float_e_s :
            break;
        case float_e_sign_s :
            break;
        case float_e_num :
            break;
        case id_or_end_s :
			fsm_state = id_or_end_logic(input, token);
            break;
        case identif_s :
            break;
        case end_sign_s :
			fsm_state = end_sign_logic(input, token);
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
    char input_char = getc(stdin);
    static int line_counter = 1;

    if(input_char == '\n') ++line_counter; 

    token_t current_token = create_token(NULL, none, 0);

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
	}
	printf("Token:\"%s\" Variant:%s Line:%d\n", token->content, TOKEN_VAR_NAMES[token->variant], token->line_num); 
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
