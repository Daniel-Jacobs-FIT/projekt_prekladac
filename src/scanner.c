#include <string.h>
#include <math.h>
#include "scanner.h"

const char *OPEN_SIGN = "<?php";
const char *TOKEN_VAR_NAMES[] = {ALL_TOKEN_VARS}; //TODO
const int KEYWORD_COUNT = 13;
const char *keywords[] = {"else", "float", "?float", "function", "if", "int", "?int", "null", "return", "string", "?string", "void", "while"};

/*makro pro vypsani chybove hlasky pri ziskani neznameho znaku*/
#define ERR_CASE\
	token->variant = err_var;\
	fsm_state = default_s;

/*makro pro testovani jestli prosel malloc nebo realloc*/
#define MALLOC_CHECK(PTR)\
if(PTR == NULL)\
{\
    fprintf(stderr, "Chyba alokace paměti!\n");\
    EXIT_CODE = 99;\
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

/* Funkce pro logiku fsm */
scanner_state_t float_e_num_logic(int input, token_t *token)
{
    scanner_state_t fsm_state;
	if(EXIT_CODE != 0)  //selhani alokace pameti
	{
		ERR_CASE
        return fsm_state;
	}

	input = getc(stdin);
	if(input >= '0' && input <= '9')
	{
		ungetc(input, stdin);
		return float_e_num_s;
	}else
	{
		token->variant = float_var;
		ungetc(input, stdin);
		return default_s;
	}
}

scanner_state_t float_e_sign_logic(int input, token_t *token)
{
    scanner_state_t fsm_state;
    inf_char_input(input, token);
	if(EXIT_CODE != 0)   //selhani alokace pameti
	{
		ERR_CASE
        return fsm_state;
	}

	input = getc(stdin);
	if(input >= '0' && input <= '9')
	{
		ungetc(input, stdin);
		fsm_state = float_e_num_s;
	}
	else    //chyba v lexemu
	{
        EXIT_CODE = 1;
        fprintf(stderr, "Chyba v lexému %s na řádku: %d\nNevalidní exponent\n",
                token->content, token->line_num);
		ERR_CASE
	}
    return fsm_state;
}

scanner_state_t float_e_logic(int input, token_t *token)
{
	scanner_state_t fsm_state;
    inf_char_input(input, token);
	if(EXIT_CODE != 0)  //selhani alokace pameti
	{
        ERR_CASE
        return fsm_state;
	}

	input = getc(stdin);
	switch(input)
	{
		case '+':
		case '-':
			ungetc(input, stdin);
			fsm_state = float_e_sign_s;
            break;
		default:
			if(input >= '0' && input <= '9')
			{
				ungetc(input, stdin);
				fsm_state = float_e_num_s;
			}
			else
			{
                EXIT_CODE = 1;
                fprintf(stderr, "Chyba v lexému %s na řádku: %d\nNevalidní exponent\n",
                        token->content, token->line_num);
                ERR_CASE
			}
	}
    return fsm_state;
}

scanner_state_t float_dot_num_logic(int input, token_t *token)
{
	scanner_state_t fsm_state;
	if(EXIT_CODE != 0)  //selhani alokace pameti
	{
          ERR_CASE
          return fsm_state;
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
				token->variant = float_var;
				ungetc(input, stdin);
				return default_s;
			}
	}
}
scanner_state_t float_dot_logic(int input, token_t *token)
{
	scanner_state_t fsm_state;
	if(EXIT_CODE != 0)  //selhani alokace pameti
	{
        ERR_CASE
        return fsm_state;
	}
	
	input = getc(stdin);
	if(input >= '0' && input <= '9')
	{
		ungetc(input, stdin);
		fsm_state = float_dot_num_s;
	}else
	{
        EXIT_CODE = 1;
        fprintf(stderr, "Chyba v lexému %s na řádku: %d\nNevalidní desetinná část\n",
                token->content, token->line_num);
        ERR_CASE
	}
    return fsm_state;
}

scanner_state_t integ_logic(int input, token_t *token)
{
    inf_char_input(input, token);
	scanner_state_t fsm_state;
	if(EXIT_CODE != 0)  //selhani alokace pameti
	{
          ERR_CASE
          return fsm_state;
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
    inf_char_input(input, token);
	scanner_state_t fsm_state;
	if(EXIT_CODE != 0)  //selhani alokace pameti
	{
          ERR_CASE
          return fsm_state;
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
			if(is_keyword == true)
			{
                if(strcmp(token->content, "null") == 0) { //zpracovani null - muze se vyskytovat ve vyrazu
                    token->variant = null_var;
                } else {
                    token->variant = identif_keyword_var;
                }
			}
			else
			{
                if(token->content[0] == '?') {
                    EXIT_CODE = 1;
                    fprintf(stderr, "Chyba v lexému %s na řádku: %d\nNevalidní identifikátor\n",
                            token->content, token->line_num);
                    ERR_CASE
                }
                token->variant = identif_function_var;
			}
		}
        else
        {
            token->variant = identif_variable_var;
        }
		//should run always
		ungetc(input, stdin);
		return default_s;
	}
}

scanner_state_t expect_eof_logic(token_t *token)
{
	int input = getc(stdin);
	scanner_state_t fsm_state;
	switch(input)
	{
		case EOF:
			return end_prg_s;
		default:
            EXIT_CODE = 1;
            fprintf(stderr, "Chyba v lexému %s na řádku: %d\nOčekáván konec souboru\n",
                    token->content, token->line_num);
            ERR_CASE
            return fsm_state;
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
	scanner_state_t fsm_state;
	switch(input)
	{
		case EOF:
			return end_prg_s;
		case 10: //eol
			ungetc(input, stdin);
			return expect_eof_s;
		default:
            EXIT_CODE = 1;
            fprintf(stderr, "Chyba v lexému %s na řádku: %d\nOčekáván konec souboru\n",
                    token->content, token->line_num);
            ERR_CASE
            return fsm_state;
	}
}

scanner_state_t id_or_end_logic(int input, token_t *token)
{
	scanner_state_t fsm_state;
    inf_char_input(input, token);
	if(EXIT_CODE != 0)  //chyba alokace pameti
	{
        ERR_CASE
        return fsm_state;
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
                EXIT_CODE = 1;
                fprintf(stderr, "Chyba v lexému %s na řádku: %d\nOčekáván identifikátor nebo ?>\n",
                        token->content, token->line_num);
                ERR_CASE
                return fsm_state;
			}
	}
}

scanner_state_t default_logic(int input, token_t *token)
{
	int cmp = input;
	ungetc(input, stdin);
	scanner_state_t fsm_state;
	switch(cmp)
	{
		case '/':
			getc(stdin);
			return div_oper_s;
		case '+':
			return add_oper_s;
		case '-':
			return sub_oper_s;
		case '*':
			return mul_oper_s;
		case '.':
			return oper_conc_s;
		case '!':
			getc(stdin);
			return not_eq1_s;
		case '=':
			getc(stdin);
			return eq_or_assign_s;
		case '>':
			getc(stdin);
			return grt_s;
		case '<':
			getc(stdin);
			return less_s;
		case '(':
			return open_rnd_s;
		case ')':
			return cls_rnd_s;
		case ',':
			//TODO check the underlying cause of this 
			//getc(stdin);
			return comma_s;
		case ':':
			//TODO here as well
			//getc(stdin);
			return colon_s;
		case '{':
			return open_curl_s;
		case ';':
			return semicol_s;
		case '}':
			return cls_curl_s;
		case '"':
            getc(stdin);
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
                EXIT_CODE = 1;
                fprintf(stderr, "Chyba v lexému %s na řádku: %d\nNeočekávaný znak: %c\n",
                        token->content, token->line_num, cmp);
                ERR_CASE
                return fsm_state;
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

/* Funkce pro zpracovani oktalovych sekvenci v retezcovych literalech */
bool oct_sequence(int input, token_t *token) {
	char position1 = input; //musi byt 0-3, jinak se nedostane do stavu oct_sequence
	char position2 = getc(stdin);
	char position3 = getc(stdin);
	int result = 0;

	//kontrola jestli jsou position2 a position3 validni oct cisla
	if((strchr("01234567", position2) != NULL) && (strchr("01234567", position3) != NULL)) {

		//vypocet indexu, position1 * 8^2 + position2 * 8^1 + position3 * 8^0
		//48 je rozdil mezi ASCII hodnotou znaku a pozadovaneho cisla
        result += (position1 - 48) * pow(8, 2);
		result += (position2 - 48) * pow(8, 1);
		result += (position3 - 48) * pow(8, 0);

		//int_input(result, token);
        inf_char_input(result, token);

		return true;
	}
	else {
		//v pripade nevalidniho znaku vrati znaky na stdin
		ungetc(position3, stdin);
		ungetc(position2, stdin);
		inf_char_input('\\', token);
		return false;
	}
}

bool hex_sequence(int input, token_t *token) {

	//Vstup do funkce kdyz nalezne "\x", pokud je input a jeho nasledujici
	//prvek validni hex charakter, ulozi hodnotu do tokenu a vrati true, 
	//jinak vrati false a input se ulozi do tokenu bez prevedeni
	char position1 = input;
	char position2 = getc(stdin);
	int result = 0;

	//Kontrola jestli je druhy position2 validni hex znak, position1
	//je zkontrolovano pri vstupu do funkce
	if (strchr("0123456789aAbBcCdDeEfF", position2) != NULL) {

		//Vypocet prvniho indexu
		//48, 55 a 87 jsou rozdily mezi ASCII kodem znaku a int 
		//reprezentujici pozadovane cislo
		if(position1 >= '0' && position1 <= '9') {
        	result += (position1 - 48) * pow(16, 1);
    	}
    	else if(position1 >= 'A' && position1 <= 'F') {
        	result += (position1 - 55) * pow(16, 1);
   		}
    	else if(position1 >= 'a' && position1 <= 'f') {
        	result += (position1 - 87) * pow(16, 1);
    	}

		//Vypocet druheho indexu
		if(position2 >= '0' && position2 <= '9') {
        	result += (position2 - 48) * pow(16, 0);
    	}
    	else if(position2 >= 'A' && position2 <= 'F') {
        	result += (position2 - 55) * pow(16, 0);
   		}
    	else if(position2 >= 'a' && position2 <= 'f') {
        	result += (position2 - 87) * pow(16, 0);
    	}

		//int_input(result, token);
        inf_char_input(result, token);

		//return true, info pro hex_s aby se neukladalo nic
		//dalsiho do tokenu
		return true;
	}
	else {
		//return false a ungetc, hex vlozi input bez uprav do tokenu
		ungetc(position2, stdin);
		return false;
	}
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
            else {
				ungetc(input,stdin);
                token->variant=div_oper_var;
				fsm_state = default_s;
                break;
			}
        case com_oneline_s : 
           if(input == '\n'){
				ungetc(input,stdin);
				fsm_state = default_s;
                break;
            }
            else{
               fsm_state=com_oneline_s;
			   break;
            }
        case com_block_s : 
            if(input == '*'){
                fsm_state=com_block_end_s;
                break;
            }
			else if(input == EOF){
                EXIT_CODE = 1;
                fprintf(stderr, "Neukončený blokový komentář na řádku: %d\n",
                        token->line_num);
                ERR_CASE
				break;
			}
			else if(input == '\n'){
                token->line_num++;
				fsm_state=com_block_s;
                break;
			}
            else{
                fsm_state=com_block_s;
                break;
            }
        case com_block_end_s :
            if(input == '/'){
				fsm_state = default_s;
                break;
            }
			else if(input == '\n'){
                token->line_num++;
				fsm_state=com_block_s;
                break;
			}
			else{
                fsm_state=com_block_s;
                break;
			}
        case add_oper_s :
           token->variant=add_oper_var;
		   fsm_state = default_s;
            break;
		case sub_oper_s :
           token->variant=sub_oper_var;
		   fsm_state = default_s;
            break;
		case mul_oper_s:
			token->variant=mul_oper_var;
			fsm_state = default_s;
            break;
        case oper_conc_s :
             token->variant=oper_conc_var;
			 fsm_state = default_s;
            break;
        case not_eq1_s :
            if(input == '='){
                fsm_state=not_eq2_s;
                break;
            }
			ungetc(input,stdin);
            EXIT_CODE = 1;
            fprintf(stderr,"Neplatný lexém '!%c' na řádku: %d\n",
                    input, token->line_num);
            ERR_CASE
            break;
        case not_eq2_s :
            if(input == '='){
                fsm_state=not_eq3_s;
                break;
            }
            EXIT_CODE = 1;
            fprintf(stderr,"Neplatný lexém '!=%c' na řádku: %d\n",
                    input, token->line_num);
            ERR_CASE
            break;
        case not_eq3_s :
			ungetc(input,stdin);
            token->variant=not_eq_var;
			fsm_state = default_s;
            break;
        case eq_or_assign_s :
            if(input == '='){
                fsm_state=eq2_s;
                break;
            }
			ungetc(input,stdin);
            token->variant=assign_var;
			fsm_state = default_s;
            break;
        case eq2_s :
            if(input == '='){
                fsm_state=eq3_s;
                break;
            }
			ungetc(input,stdin);
			fsm_state = default_s;
            EXIT_CODE = 1;
            fprintf(stderr,"Neplatný lexém '==%c' na řádku: %d\n",
                    input, token->line_num);
            ERR_CASE
            break;
            
        case eq3_s :
			ungetc(input,stdin);
            token->variant=eq_var;
			fsm_state = default_s;
            break;
        case grt_s :
            if(input == '='){
				ungetc(input, stdin);
                fsm_state=grt_eq_s;
                break;
            }
            else {
				ungetc(input, stdin);
                token->variant=grt_var;
				fsm_state = default_s;
                break;
            }
            
        case grt_eq_s :
            token->variant=grt_eq_var; 
			fsm_state = default_s;
            break;
        case less_s :
            if(input == '='){
				ungetc(input, stdin);
                fsm_state=less_eq_s;
                break;
            }
            else{
			   ungetc(input, stdin);
               token->variant=less_var;
			   fsm_state = default_s;
               break; 
            }
        case less_eq_s :
            token->variant=less_eq_var;
			fsm_state = default_s;
            break;
        case open_rnd_s :
            token->variant=open_rnd_var;
			fsm_state = default_s;
            break;
        case cls_rnd_s :
            token->variant=cls_rnd_var;
			fsm_state = default_s;
            break;
		case comma_s:
			token->variant = comma_var;
			fsm_state = default_s;
			break;
		case colon_s:
			token->variant = colon_var;
			fsm_state = default_s;
			break;
        case open_curl_s :
            token->variant=open_curl_var;
			fsm_state = default_s;
            break;
        case cls_curl_s :
            token->variant=cls_curl_var;
			fsm_state = default_s;
            break;
        case semicol_s :
            token->variant=semicol_var;
			fsm_state = default_s;
            break;
        case string_lit_end_s :
            ungetc(input, stdin);
			token->variant = string_lit_end_var;
			fsm_state = default_s;
            break;
        case string_lit_s :
			//input = getc(stdin);


            if(input == '"') {
                fsm_state = string_lit_end_s;
                break;
            }
            else if(input == '\\'){
                fsm_state = esc_char_s;
            }
			else {
                if(input == '\n') {
                    token->line_num++;
                }
                else if(input < 31) {
                    EXIT_CODE = 1;
                    fprintf(stderr,"Neplatný znak %c v řetězcovém literálu %s na řádku: %d\n",
                            input, token->content, token->line_num);
                    ERR_CASE
                    break;
                }
                inf_char_input(input, token);
				if(EXIT_CODE != 0) {
                    ERR_CASE
                }
			} 	
            break;
            
		//ESCAPE SEQUENCES
        case esc_char_s :
            if(strchr("nrtve\\$", input) != NULL){ //escape sequence

				switch(input)
				{
					//Cisla pro input jsou ASCII kody pozadovanych znaku
					case 'n': 
						inf_char_input(10, token); //newline
						break;
					case 'r':
						inf_char_input(13, token); //carriage return
						break;
					case 't': 
						inf_char_input(9, token); //tab
						break;
					case 'v': 
						inf_char_input(11, token); //vertical tab
						break;
					case 'e': 
						inf_char_input(27, token); //escape
						break;
					case '\\': 
						inf_char_input(input, token); //backslash
						break;
					case '$': 
						inf_char_input(input, token); //dollar sign
						break;
					case '"': 
						inf_char_input('\\', token); //uvozovky
						inf_char_input('"', token); //uvozovky
						break;
					default:
						ungetc(input, stdin);


				}
                fsm_state = esc_seq_s;
                if(EXIT_CODE != 0) {
                    ERR_CASE
                }
                break;
            }
            
			else if(strchr("x", input) != NULL) { //hex sequence
                fsm_state = hex1_s;
                break;
            }
            else if(strchr("0123", input) != NULL){ //octal sequence

				if(oct_sequence(input, token) == false){
					inf_char_input(input, token);
				}
				fsm_state = string_lit_s;
                if(EXIT_CODE != 0) {
                    ERR_CASE
                }
                break;
            }
            else {
                fsm_state = string_lit_s;
                break;
            }
        case esc_seq_s :

		//Escape sequence content save by mel pro prehled byt az tady, aby v esc_seq_s nemusela
		//byt volana funkce pro ukladani normalnich znaku, ted tam vsak je jinak to preskoci
		//vzdy prvni znak po escape sequence
		//Pripadne asi pujde se stavu esc_seq_s kompletne zbavit
            if(input == '"') {
                fsm_state = string_lit_end_s;
                break;
            }
            else {
			    inf_char_input(input, token);
                if(EXIT_CODE != 0) {
                    ERR_CASE
                }
                fsm_state = string_lit_s;
                break;
            }
        case oct1_s:
            break;
        case hex1_s:
        
            //if (strchr("0123456789aAbBcCdDeEfF", input) != NULL) {
				//Pokud to neni valid hex sekvence, nacte momentalni znak
				//a prepne se do stavu string_lit_s
            if(hex_sequence(input, token) == false){	

					inf_char_input('\\', token);
					inf_char_input('x', token);
					inf_char_input(input, token);
			}
			fsm_state = string_lit_s;
            if(EXIT_CODE != 0) {
                ERR_CASE
            }
            break;
        case integ_s :
			fsm_state = integ_logic(input, token);
            if(EXIT_CODE != 0) {
                ERR_CASE
            }
            break;
        case float_dot_s :
            inf_char_input(input, token);
			fsm_state = float_dot_logic(input, token);
            if(EXIT_CODE != 0) {
                ERR_CASE
            }
            break;
        case float_dot_num_s :
            inf_char_input(input, token);
			fsm_state = float_dot_num_logic(input, token);
            if(EXIT_CODE != 0) {
                ERR_CASE
            }
            break;
        case float_e_s :
			fsm_state = float_e_logic(input, token);
            if(EXIT_CODE != 0) {
                ERR_CASE
            }
            break;
        case float_e_sign_s :
            fsm_state = float_e_sign_logic(input, token);
            if(EXIT_CODE != 0) {
                ERR_CASE
            }
			break;
        case float_e_num_s :
            fsm_state = float_e_num_logic(input, token);
            inf_char_input(input, token);
            if(EXIT_CODE != 0) {
                ERR_CASE
            }
            break;
        case id_or_end_s :
			fsm_state = id_or_end_logic(input, token);
            if(EXIT_CODE != 0) {
                ERR_CASE
            }
            break;
        case identif_s :
			fsm_state = identif_logic(input, token);
            if(EXIT_CODE != 0) {
                ERR_CASE
            }
            break;
        case end_sign_s :
			fsm_state = end_sign_logic(token);
            if(EXIT_CODE != 0) {
                ERR_CASE
            }
            break;
		case expect_eof_s :
			fsm_state = expect_eof_logic(token);
            if(EXIT_CODE != 0) {
                ERR_CASE
            }
			break;
        case end_prg_s :
			fsm_state = end_prg_logic(token);
            if(EXIT_CODE != 0) {
                ERR_CASE
            }
            break;
    }  
    return fsm_state;
}

/* Funkce pro parser, na pozadani cte vstup, dokud neprecte dost informaci
 * pro vytvoreni tokenu, ktery vraci
 * */
token_t *get_token() {
    int input_char;
    static int line_counter = 1;

    token_t *current_token = create_token(NULL, none, line_counter);
    while(current_token->variant == none) {
        input_char = getc(stdin);
        fsm_step(input_char, current_token);
    }

    line_counter = current_token->line_num;
    return current_token;
}

/* Funkce pro praci s tokeny*/

/* Funkce pro vytvoreni tokenu - argumenty funkce budou jednotlive polozky tokenu
 * Pro zvyseni citelnosti kodu pri tvorbe tokenu
 * content = obsah tokenu (nactene symboly)
 * token_var = varianta tokenu
 * linu_num = cislo radku, na kterem byl token nacteny
 * */
token_t *create_token(char* content, token_var variant, int line_num) {
    token_t *token = (token_t *)malloc(sizeof(token_t));
    if(token == NULL) { //chyba alokace pameti
        EXIT_CODE = 99;
        free(content);
    } else {
        token->content = content;
        token->variant = variant;
        token->line_num = line_num;
    }
    return token;
}

//vytvoreni tokenu s contentem ktery je retezcovy literal
token_t *create_token_from_lit(char *content, token_var variant, int line_num)
{
	token_t *token = (token_t *)malloc(sizeof(token_t));
	char *token_content = (char *)malloc(strlen(content)+1);
    if(token == NULL || token_content == NULL) { //chyba alokace pameti
        EXIT_CODE = 99;
        free(token_content);    //free(null) neprovede zadnou operaci
        free(token);
    } else {
        strcpy(token_content, content);
        token->content = token_content;
        token->line_num = line_num;
        token->variant = variant;
    }
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

    if (token->content != NULL) { //content bude uvolnen pouze, pokud neni null
        free(token->content);
        token->content = NULL;
    }

    token->variant = none;
    token->line_num = 0;
	free(token);
}
