//#include <string.c> //docasne pro vyvoj string.c
#include <string.h>
#include <math.h>
#include "scanner.h"

const char *OPEN_SIGN = "<?php";
const char *TOKEN_VAR_NAMES[] = {ALL_TOKEN_VARS}; //TODO
static char buffer[5];
static int decimal = 0;
static int string_buffer_count = 0;

#define MALLOC_CHECK(NAME)\
if(NAME == NULL)\
{\
	fprintf(stderr, "failed to initialize memory with malloc\n");\
	return -1;\
}

/*
funkce na vypisovani error message pri ziskani neplatneho charu
	param: err_char -> character ktery zpusobil chybu
	param: line_num -> cislo radku na kterem byla chyba zpusobena
*/
void state_err(char *fce_name, int err_char, int line_num)
{
	printf("Error: in functions %s unexpected ", fce_name);
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
scanner_state_t identif_logic(int input, token_t *token)
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
		//strlen vrati velikost bez '\0' tak musime ji pridat + velikost 1 charu
		char *content = (char *)realloc((void *)token->content, str_size+2);
		MALLOC_CHECK(content);
		content[str_size] = (char)input;
		content[str_size+1] = '\0';
		token->content = content;
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
				token->variant = err_var;
				state_err("identif_logic", input, token->line_num);
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
		/* case 10: //eol */
		/* 	//strlen() vraci delku stringu minus '\0' a proto musime pridat 2 chary */
		/* 	content_lenght = strlen(token->content); */
		/* 	char *new_content =(char *)realloc((void *)token->content, content_lenght+2); */ 
		/* 	if(new_content == NULL) */
		/* 	{ */
		/* 		fprintf(stderr, "failed to allocate memory with realloc\n"); */
		/* 		return -1; */
		/* 	} */
		/* 	token->content = new_content; */
		/* 	token->content[content_lenght] = (char)input; */
		/* 	token->content[content_lenght+1] = '\0'; */
		/* 	return end_sign_s; */
		case EOF:
			return end_prg_s;
		default:
			//error case
			token->variant = err_var;
			state_err("expect_eof_logic", input, token->line_num);
			return default_s;
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
			/* //strlen() vraci delku stringu minus '\0' a proto musime pridat 2 chary */
			/* content_lenght = strlen(token->content); */
			/* char *new_content =(char *)realloc((void *)token->content, content_lenght+2); */ 
			/* if(new_content == NULL) */
			/* { */
			/* 	fprintf(stderr, "failed to allocate memory with realloc\n"); */
			/* 	return -1; */
			/* } */
			/* token->content = new_content; */
			/* token->content[content_lenght] = (char)input; */
			/* token->content[content_lenght+1] = '\0'; */
			/* return end_sign_s; */
		case EOF:
			return end_prg_s;
		case 10: //eol
			ungetc(input, stdin);
			return expect_eof_s;
		default:
			//error case
			token->variant = err_var;
			state_err("end_sight_logic", input, token->line_num);
			return default_s;
			
	}
}

scanner_state_t id_or_end_logic(int input, token_t *token)
{
	if(token->content == NULL)
	{
		char *content = (char *)malloc(2);
		MALLOC_CHECK(content);
		content[0] = (char)input;
		content[1] = '\0';
		token->content = content;
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
				//error case
				token->variant = err_var;
				state_err("id_or_end_logic", input, token->line_num);
				return default_s;
			}
	}
}

scanner_state_t integ_logic(int input, token_t *token)
{
	static int next_index = 0;
	
	if(token->content == NULL)
	{
		//malloc(1) protoze sizeof(char) je 1
		char *content = (char *)malloc(1);
		MALLOC_CHECK(content);
		content[0] = '\0';
		token->content = content;
	}
	switch(input)
	{
		case '.':
			ungetc(input, stdin);
			return float_dot_s;
		case 'E':
		case 'e':
			ungetc(input, stdin);
			return float_e_s;
		default:
			if(input >= '0' && input <= '9')
			{
				//next_index+2 protoze indexujeme od 0
				char *new_content =(char *)realloc((void *)token->content, next_index+2); 
				MALLOC_CHECK(new_content)
				token->content = new_content;
				
				token->content[next_index] = (char)input;

				//next_index+1 vzdy ukazuje na posledni misto ve stringu	
				token->content[next_index+1] = '\0';
				return integ_s;
			}
			else
			{
				token->variant = integ_var;
				ungetc(input, stdin);
				
				next_index = 0;
				return default_s;
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
				//error case
				token->variant = err_var;
				state_err("default_logic", cmp, token->line_num);
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
            token->variant = string_lit_end_var;
            /* token->content = &input; *///EDIT: mel by dat malloc a ulozit do neho content input, takhle se ulozi stracena pamet
            break;
        case string_lit_s :
            if(input == '"') {
                fsm_state = string_lit_end_s;
                printf("scan lit end\n");
                break;
            }
            else if(input == '\\'){
                fsm_state = esc_char_s;
                printf("esc char state\n");
            }
            else {
                string_buffer(*token, &input);
            }
            break;
            
        case esc_char_s :
            if(strchr("nrtve\\$", input) != NULL){ //escape sequence
                fsm_state = esc_seq_s;
                printf("esc_seq\n");
                break;
            }
            else if(strchr("x", input) != NULL) { //hex sequence
                fsm_state = hex1_s;
                decimal = 0;
                printf("hex1_s\n");
                break;
            }
            else if(strchr("0123", input) != NULL){ //octal sequence
                fsm_state = oct1_s;
                decimal = 0;
                to_decimal(input, 8, 2);
                printf("oct_start\n");
                break;
            }
            else {
                fsm_state = string_lit_s;
                printf("string_lit_s\n");
                break;
            }
        case esc_seq_s :
            if(input == '"') {
                fsm_state = string_lit_end_s;
                printf("scan lit end\n");
                break;
            }
            else {
                fsm_state = string_lit_s;
                printf("string_lit\n");
                break;
            }
        case hex1_s :
            if(input == '"') {
                fsm_state = string_lit_end_s;
                printf("scan lit end\n");
                break;
            }
            else if (strchr("0123456789aAbBcCdDeEfF", input) != NULL) {
                to_decimal(input, 16, 1);
                fsm_state = hex2_s;
                printf("hex2_s\n");
                break;
            }
            else {
                fsm_state = string_lit_s;
                printf("string_lit\n");
                break;
            }
        case hex2_s :
             if(input == '"') {
                fsm_state = string_lit_end_s;
                printf("scan lit end\n");
                break;
            }
            else if (strchr("123456789aAbBcCdDeEfF", input) != NULL) {
                fsm_state = string_lit_s;
                printf("%d\n", to_decimal(input, 16, 0));
                printf("string_lit\n");
                break;
            }
            else {
                fsm_state = string_lit_s;
                printf("string_lit\n");
                break;
            }
        case oct1_s :
            if(input == '"') {
                fsm_state = string_lit_end_s;
                printf("scan lit end\n");
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
                printf("scan lit end\n");
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
