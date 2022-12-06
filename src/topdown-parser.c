#include "topdown-parser.h"

int GET_NEXT_TOKEN_INDEX = 0;
int BUILTIN_FUNCTIONS_COUNT = 8;
const char *builtin_functions[] = {"reads", "readi", "readf", "write", "strlen", "substring", "ord", "chr"};
int BUILTIN_TYPES_COUNT = 6;
const char *builtin_types[] =  {"float", "?float", "int", "?int", "string", "?string"};
int IN_WHILE_LOOPS = 0;
bst_node_t *CURRENT_FUNCTION = NULL;

/*
--- What do I need to do
create a function for every part of the grammer we have defined
look at the rules and decide if things are as they should be as defined in the grammer
generate IFJ code for each thing I parse
	- also I am probably only printing terminals, since anything else is just going to get broken down into terminals or error
	- probably a printf statement, dont know why I would do it any other way
	- the printing process can be done later as so I can program the bare bones code and add the IFJ code later, after its working
call bottom up parser for expressions (needs correct data types)
make a frame (table of symbols) for checking if variables are being defined or the value is being changed
It seems like I only should eat tokens (increment the "stack" index) when I find it in a terminal, cause I dont know what all it can be
--- prob more but this is what I can think of ATM
*/
int FCALL_nt(stack_t *, bst_node_t **, bst_node_t **, char *);
int TL_nt(stack_t *, char *, bst_node_t **);
int ASG_nt(stack_t *, bst_node_t **, bst_node_t **);
int STAT_nt(stack_t *, bst_node_t **, bst_node_t **);
int PL_nt(stack_t *, char *, bst_node_t **);
int SS_nt(stack_t *, bst_node_t **, bst_node_t **);
int FDEF_nt(stack_t *, bst_node_t **);
int SSD_nt(stack_t *, bst_node_t **);
int IF_nt(stack_t *, bst_node_t **, bst_node_t **);
int RET_nt(stack_t *, bst_node_t **, bst_node_t **);
int CYC_nt(stack_t *, bst_node_t **, bst_node_t **);

#define ERROR_OUT(MSG)\
	printf("%s\n", MSG);\
	return 1;

int inf_char_str(char *str, int input)
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
	else {
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

//1 if true, else 0
int is_builtin_fce(char *str)
{
	for(int i = 0; i < BUILTIN_FUNCTIONS_COUNT; i++)
	{
		if(strcmp(str, builtin_functions[i]) == 0)
		{
			return 1;
		}
	}
	return 0;
}

int is_builtin_type(char *str)
{
	for(int i = 0; i < BUILTIN_TYPES_COUNT; i++)
	{
		if(strcmp(str, builtin_types[i]) == 0)
		{
			return 1;
		}
	}
	return 0;
}

//return type int (0 means successes, anything else is an error code)
//do you create a stuff before or in PRG, logic seems to say in PRG, cause why would you want to deal with it in main
//no tokens needed either since it can get the all by itself
int PRG_nt()
{
	stack_t *stack = psa_stack_init();
	bst_node_t **global_symbtab = (bst_node_t **)malloc(sizeof(bst_node_t *));
	if(global_symbtab == NULL)
	{
		//TODO malloc failed
		printf("Malloc failed in PRG\n");
		return 1;
	}
	bst_init(global_symbtab);
	int ret_val = push_all_tokens_to_stack(stack);
	if(ret_val == 1)
	{
		//TODO failed to push all tokens to stack
		printf("failed to push all tokens to stack\n");
		return 1;
	}
	if(ret_val == 2)
	{
		//TODO gotten err var durring lexical analysis
		printf("gotten err var durring lexical analysis\n");
		return 1;
	}
	
	SSD_nt(stack, global_symbtab);
	return 0;
}

//main driving function, all the init stuff is probably already done and so all I need to do is start parsing :)
/*
SSD -> STAT SSD
SSD -> FDEF SSD
SSD -> end
*/
int SSD_nt(stack_t *stack, bst_node_t **global_symbtab)
{
	token_t *token = psa_stack_get_nth_rev(stack, GET_NEXT_TOKEN_INDEX);
	if(token->variant == end_prg_var)
	{
		return 0;
	}
	else if(token->variant == identif_keyword_var)
	{
		if(strcmp(token->content, "function") == 0)
		{
            FDEF_nt(stack, global_symbtab);
            if(EXIT_CODE != 0) {
				return 1;
            }
		}else
		{
			STAT_nt(stack, global_symbtab, NULL);
            if(EXIT_CODE != 0) {
                return 1;
            }
		}
	}else
	{
		STAT_nt(stack, global_symbtab, NULL);
        if(EXIT_CODE != 0) {
            return 1;
        }
	}
	SSD_nt(stack, global_symbtab);
    if(EXIT_CODE != 0) {
        return 1;
    }
	return 0;
	/*
		- id = <EXPR>
		- id = <FCALL>
		- if ( <EXPR> <SS> else { <SS> 
		- while ( <EXPR> <SS>
		- f_id ( <TL> ;
		- return ;
		- return <EXPR> ;
	*/
	/* if else(token->variant == identif_keyword_var || token->variant == identif_variable_var || token->variant == identif_function_var) */
	/* { */
	/* 	//identif_keyword check (cause I cant have all keywords, that would be just ridiculus) */
	/* 	if(strcmp(token->content, "if") || strcmp(token->content, "while"), strcmp(token->content, "return")) */
	/* 	{ */
	/* 	} */
	/* 	//all vars are legal, just check/add them into glabal_symtab, since I am not in a function yet */
	/* 	//with function_var I have to just check if it exists globaly, since I am not in a function yet */
	/* } */
	//what is in a fdef:
	/*
		- function f_id ( <PL> : par_t { <SS>
	*/
}
// ik that the first keyword is a function and so I can just print it
int FDEF_nt(stack_t *stack, bst_node_t **global_symbtab)
{
	//GET_NEXT_TOKEN_INDEX gets incremented
	token_t *token = next_stack_token(stack, &GET_NEXT_TOKEN_INDEX);
	printf("%s ", token->content);
	token = psa_stack_get_nth_rev(stack, GET_NEXT_TOKEN_INDEX);
	if(token->variant != identif_function_var)
	{
		//TODO ERROR EXPECTED FUNCTION NAME
		printf("ERROR EXPECTED FUNCTION NAME\n");
		return 1;
	}else
	{
		//check if redef
		//check if inbuilt
		if(bst_search((*global_symbtab), token->content) != NULL)
		{
			//TODO ERROR REDEFINITION
			printf("ERROR REDEFINITION\n");
			return 1;
		}
		else if(is_builtin_fce(token->content) == 1)
		{
			//TODO REDEFINITION OF A BUILTIN FUNCTION
			printf("REDEFINITION OF A BUILTIN FUNCTION\n");
			return 1;
		}
		else
		{
			//get params/return type in a string ie. float f(int, int) = "fii" <- malloc'ed
			//and the key(function name) that is malloced as well
			char *types = (char *)malloc(1);
			if(types == NULL)
			{
				//TODO MALLOC FAIL
				printf("MALLOC FAIL\n");
				return 1;
			}
			types[0] = '\0';
			bst_insert(global_symbtab, token->content, func_id, types);
			printf("%s(", token->content);

			//get f_id again just to move pointer
			next_stack_token(stack, &GET_NEXT_TOKEN_INDEX);
			//eat the expected '(' char and check if it actually is an '('
			token = next_stack_token(stack, &GET_NEXT_TOKEN_INDEX);
			if(token->variant == open_rnd_var)
			{
				bst_node_t **local_symbtab = (bst_node_t **)malloc(sizeof(bst_node_t *));
				if(local_symbtab == NULL)
				{
					//TODO malloc failed
					printf("MALLOC FAIL\n");
					return 1;
				}
				bst_init(local_symbtab);
				if(PL_nt(stack, types, local_symbtab) != 0)
				{
					return 1;
				}

				//": par_t { SS" <- this is what I can expect
				token = next_stack_token(stack, &GET_NEXT_TOKEN_INDEX);
				if(token->variant == colon_var)
				{
					token = next_stack_token(stack, &GET_NEXT_TOKEN_INDEX);
					if(token->variant == identif_keyword_var)
					{
						if(is_builtin_type(token->content) || strcmp(token->content, "void") == 0)
						{
							inf_char_str(types, token->content[0]);
							printf(": %s ", token->content);
							
							token = next_stack_token(stack, &GET_NEXT_TOKEN_INDEX);
							if(token->variant == open_curl_var)
							{
								printf("{ \n");
								if(SS_nt(stack, global_symbtab, local_symbtab) != 0)
								{
									return 1;
								}
								return 0;
							}else
							{
								//TODO EXPECTED '{' AFTER RETURN TYPE
								printf("EXPECTED '{' AFTER RETURN TYPE\n");
								return 1;
							}
						}
						else
						{
							//TODO EXPECTED BUILTIN TYPE
							printf("EXPECTED BUILTIN TYPE\n");
							return 1;
						}
					}
					else
					{
						//TODO EXPECTED KEYWORD
						printf("EXPECTED KEYWORD\n");
						return 1;
					}
				}else
				{
					//TODO EXPECTED ':' AFTER FUNCTION PARAMETERS
					printf("EXPECTED ':' AFTER FUNCTION PARAMETERS\n");
					return 1;
				}
			}else
			{
				//TODO EXPECTED '(' char
				printf("EXPECTED '(' char\n");
				return 1;
			}
		}
	}
	return 0;
}

int PL_nt(stack_t *stack, char *types, bst_node_t **symbtab)
{
	token_t *token = next_stack_token(stack, &GET_NEXT_TOKEN_INDEX);
	if(token->variant == cls_rnd_var)
	{
		//return successes
		printf(") ");
		return 0;
	}
	else if(token->variant == identif_keyword_var)
	{
add_param:
		if(is_builtin_type(token->content))
		{
			//add the data type of param 1 into types
			inf_char_str(types, token->content[0]);
			char *type = NULL;
			inf_char_str(type, token->content[0]);
			token = next_stack_token(stack, &GET_NEXT_TOKEN_INDEX);
			if(token->variant == identif_variable_var)
			{
				//this is what we want to happen
				bst_insert(symbtab, token->content, var_id, type);
				printf("%s ", token->content);
			}
			else
			{
				//TODO EXPECTED VARIABLE NAME AFTER DATA TYPE
				printf("EXPECTED VARIABLE NAME AFTER DATA TYPE\n");
				return 1;
			}
			token = next_stack_token(stack, &GET_NEXT_TOKEN_INDEX);
			if(token->variant == cls_rnd_var)
			{
				//i can also jump but whatever
				printf(") ");
				//return successes
				return 0;
			}
			else if(token->variant == comma_var)
			{
				token = next_stack_token(stack, &GET_NEXT_TOKEN_INDEX);
				//check if its keyword and if so, jump and if not err
				if(token->variant == identif_keyword_var)
				{
					goto add_param;
				}
				else
				{
					goto expected_data_type;
				}
			}
			else
			{
				//TODO EXPECTED EITHER ')' OR ',' AFTER PARAMETER
				printf("EXPECTED EITHER ')' OR ',' AFTER PARAMETER\n");
				return 1;
			}
		}
		else
		{
			//TODO EXPECTED BUILTIN DATA TYPE
			printf("EXPECTED BUILTIN DATA TYPE\n");
			return 1;
		}
	}
	else
	{
expected_data_type:
		//TODO EXPECTED DATA TYPE
		printf("EXPECTED DATA TYPE\n");
		return 1;
	}
	return 0;
}

int STAT_nt(stack_t *stack, bst_node_t **global_symbtab, bst_node_t **local_symbtab)
{
	token_t *first_token = psa_stack_get_nth_rev(stack, GET_NEXT_TOKEN_INDEX);
	token_t *second_token = psa_stack_get_nth_rev(stack, GET_NEXT_TOKEN_INDEX + 1);
	if(first_token->variant == identif_variable_var && second_token->variant == assign_var) {//zpracuje ASG
        ASG_nt(stack, global_symbtab, NULL);
        if(EXIT_CODE != 0) {
            return 1;
        }
    } else if(first_token->variant == identif_function_var) {   //volani void funkce
        //TODO: FCALL
            fprintf(stderr, "FCALL from statement not implemented yet!\n"); 
            EXIT_CODE = 69;
            return 1;
    } else if(first_token->variant == identif_keyword_var) {
        //makro pro ukonceni funkce z vetvi
        #define STAT_ERR_RET\
            if(EXIT_CODE != 0) {\
                return 1;\
            } else {\
                return 0;\
            }
        if(strcmp(first_token->content, "if") == 0) {           //zpracuje IF 
            IF_nt(stack, global_symbtab, local_symbtab);
            STAT_ERR_RET
        } else if(strcmp(first_token->content, "while") == 0) { //zpracuje WHILE
            CYC_nt(stack, global_symbtab, local_symbtab);
            STAT_ERR_RET
        } else if(strcmp(first_token->content, "return") == 0) {//zpracuje RETURN
            RET_nt(stack, global_symbtab, local_symbtab);
            STAT_ERR_RET
        } else {
            fprintf(stderr, "Syntaktická chyba na řádku:%d\nNeočekávaný lexém %s\n", first_token->line_num, first_token->content);
            EXIT_CODE = 2;
            return 1;
        }
    } else {    //zpracovavam expression
        bool is_in_while_loop = IN_WHILE_LOOPS;
        if(local_symbtab == NULL) { //predej globalni tabulku symbolu, nejsi ve funkci
            bottom_up_parser(stack, &GET_NEXT_TOKEN_INDEX, global_symbtab, false, true, is_in_while_loop, ass_table);
        } else {    //predej lokalni tabulku symbolu, jsi ve funkci
            bottom_up_parser(stack, &GET_NEXT_TOKEN_INDEX, global_symbtab, false, true, is_in_while_loop, ass_table);
        }
        if(EXIT_CODE != 0) {
            return 1;
        }
    }
    return 0;
}

int ASG_nt(stack_t *stack, bst_node_t **global_symbtab, bst_node_t **local_symbtab)
{
	token_t *token = next_stack_token(stack, &GET_NEXT_TOKEN_INDEX);    //vzdy identifikator promenne
    bst_node_t *var_in_symbtab;
    char *new_var_name;
    char *new_data_type;
    char frame_name[3];
    /* Nejdriv se zjisti, jestli je promenna definovana, pokud ne, vyprinti se DEFVAR
     *      a ulozi se do tabulky symbolu
     * Pote se zjisti, zda-li je za identifikatorem '=' pokud ne, je to chyba
     * Pokud je tam, muze se jednat o volani funkce nebo prirazeni vyrazu -> rozhodnout se
     * */
    bst_node_t** symbtable;

    if(local_symbtab == NULL) {
        symbtable = global_symbtab;
        strcpy(frame_name, "GF");
    } else {
        symbtable = local_symbtab;
        strcpy(frame_name, "LF");
    }

    #define ASG_DEFVAR\
        new_var_name = (char *)calloc(strlen(token->content) + 1, sizeof(char));\
        new_data_type = (char *)calloc(1, sizeof(char));\
        if(new_var_name == NULL || new_data_type == NULL) {\
            free(new_var_name);\
            free(new_data_type);\
            EXIT_CODE = 99;\
            fprintf(stderr, "Chyba alokace paměti\n");\
            return 1;\
        }\
        strcpy(new_var_name, token->content);\
		new_data_type[0] = '\0';

    //kontrola, zda-li promenna uz neni definovana
    var_in_symbtab = bst_search(*global_symbtab, token->content);
    if(var_in_symbtab == NULL) {   //promenna jeste neni definovana
        ASG_DEFVAR
        if(IN_WHILE_LOOPS == 0) {
            fprintf(stdout, "DEFVAR %s@%s\n", frame_name, token->content);
        } else {
            //vygenerovani noveho pomocneho navesti pro pripad, ze
            //parsujeme ve while-loopu, nezle tu definovat promenne,
            //musime zabranit redefinici
            char *no_redef_label = get_rand_name(symbtable, "_defvar_skip");
            if(EXIT_CODE != 0) {
                return 1;
            }
            bst_insert(symbtable, no_redef_label, label_id, NULL);
            if(EXIT_CODE != 0) {
                return 1;
            }

            fprintf(stdout, "TYPE %s@%s %s@%s\n", frame_name, NO_REDEF_TYPE_VAR,
                                                  frame_name, token->content);
            fprintf(stdout, "JMPIFNEQ %s %s@%s string@\n", no_redef_label,
                frame_name, NO_REDEF_TYPE_VAR);
            fprintf(stdout, "DEFVAR %s@%s\n", frame_name, token->content);
            fprintf(stdout, "LABEL %s\n", no_redef_label);
        }
    } else {    //promenna uz je definovana
        new_var_name = var_in_symbtab->key;
        new_data_type = var_in_symbtab->data_type;
    }
    //definovana je sice definovana ve vygenerovanem kodu, ale ne v tabulce symbolu
    //pokud by totiz byla pouzita ve vyrazu jeji definice, je to chyba
    //do tabulky symbolu bude zarazena pozdeji

	token = next_stack_token(stack, &GET_NEXT_TOKEN_INDEX); //pozere =
    //muzem ho pozrat uz tu, protoze toto je assignment

    if(token->variant != assign_var) {  //neni prirazeni - chyba
        EXIT_CODE = 2;
        free(new_var_name);
        free(new_data_type);
        fprintf(stderr, "Syntaktická chyba na řádku: %d\nOčekáváno '='\n", token->line_num);
        return 1;
    } else {
		if(token->variant == identif_function_var)  //je to volani funkce
		{
			if(FCALL_nt(stack, global_symbtab, local_symbtab, new_data_type) != 0)
			{
				if(new_data_type[strlen(new_data_type)-1] == 'n')
				{
					ERROR_OUT("ERROR ASIGNING FROM A VOID FUNCTION");
				}
				return 1;
			}

            //priradit do var_insymtab_data_type novy datatyp
            //generovat move instrukci
		} else {    //je to prirazeni vyrazu -> ENGAGE BOTTOM UP CHROUSTAC

            #define ASG_EXPR_PARSE\
                bst_node_t *expr_result;\
                expr_result = bottom_up_parser(stack, &GET_NEXT_TOKEN_INDEX, global_symbtab, false, true, IN_WHILE_LOOPS, ass_table);\
                if(EXIT_CODE != 0) {\
                    free(new_var_name);\
                    free(new_data_type);\
                    return 1;\
                }\
                fprintf(stdout, "MOVE %s@%s %s@%s\n", frame_name, new_var_name,\
                                                    frame_name, expr_result->key);\

            if(local_symbtab == NULL) { //pracuju s globalni tabulkou
                ASG_EXPR_PARSE
                if(var_in_symbtab == NULL) {    //promenna jeste nebyla definovana
                    bst_insert(global_symbtab, new_var_name, var_id, new_data_type);
                    var_in_symbtab = bst_search(*global_symbtab, new_var_name);
                    var_in_symbtab->data_type[0] = expr_result->data_type[0];
                } else {    //promenna je jiz definovana
                    var_in_symbtab->data_type[0] = expr_result->data_type[0];
                }
            } else {    //pracuju s lokalni tabulkou
                ASG_EXPR_PARSE
                if(var_in_symbtab == NULL) {    //promenna jeste nebyla definovana
                    bst_insert(local_symbtab, new_var_name, var_id, new_data_type);
                    var_in_symbtab = bst_search(*local_symbtab, new_var_name);
                    var_in_symbtab->data_type[0] = expr_result->data_type[0];
                } else {    //promenna je jiz definovana
                    var_in_symbtab->data_type[0] = expr_result->data_type[0];
                }
            }
        }
    }
    return 0;
}

int SS_nt(stack_t *stack, bst_node_t **global_symbtab, bst_node_t **local_symbtab)
{
	token_t *token = psa_stack_get_nth_rev(stack, GET_NEXT_TOKEN_INDEX);
	if(token->variant == cls_curl_var)
	{
        next_stack_token(stack, &GET_NEXT_TOKEN_INDEX); //pozere {
		return 0;
	}else
	{
        STAT_nt(stack, global_symbtab, local_symbtab);
        if(EXIT_CODE != 0) {
            return 1;
        }
	}

    SS_nt(stack, global_symbtab, local_symbtab);

    if(EXIT_CODE != 0) {
        return 1;
    } else {
        return 0;
    }
}

int RET_nt(stack_t *stack, bst_node_t **global_symbtab, bst_node_t **local_symbtab) {
	token_t *token = next_stack_token(stack, &GET_NEXT_TOKEN_INDEX);    //pozere return 
    char return_data_type;
    bst_node_t **symbtable;
    bool in_function;
    bst_node_t *expr_result;
    char frame_name[3];

    if(local_symbtab == NULL) {
        symbtable = global_symbtab;
        in_function = false;
        strcpy(frame_name, "GF");
    } else {
        symbtable = local_symbtab;
        in_function = true;
        strcpy(frame_name, "LF");
    }

    //zjistime datovy typ
    if(token->variant == semicol_var) { //prazdny vyraz
        return_data_type = 'v';
    } else {    //je tu vyraz, musime ho zpracovat
        expr_result = bottom_up_parser(stack, &GET_NEXT_TOKEN_INDEX, symbtable,
            in_function, true, IN_WHILE_LOOPS, ass_table);
        if(EXIT_CODE != 0) {
            return 1;
        }
        return_data_type = expr_result->data_type[0];
    }

    if(CURRENT_FUNCTION == NULL) {  //nejsme ve funkci -> exit z programu
        fprintf(stdout, "EXIT int@0\n");
    } else {    //jsme ve funkci, musime provest navrat z funkce a spravne predani hodnoty
        char expected_data_type = CURRENT_FUNCTION->data_type[strlen(CURRENT_FUNCTION->data_type) - 1];
        if(expected_data_type != return_data_type) {    //funkce vraci jiny typ, nez je definovan
            token = psa_stack_get_nth_rev(stack, GET_NEXT_TOKEN_INDEX);
            EXIT_CODE = 4;
            fprintf(stderr, "Chybný návratový typ na řádku %d ve funkci %s\n", token->line_num, CURRENT_FUNCTION->key);
            return 1;
        } else {    //navratovy typ je korektni
            fprintf(stdout, "PUSHS %s@%s\n", frame_name, expr_result->key);
        }
    }
    return 0;
}

/* int ASG_nt(stack_t *stack, bst_node_t **global_symbtab) */
/* { */

/* } */

int conditions_parse(bst_node_t *expr_result, bst_node_t **symbtable, char *frame_name, 
        char *label_for_jmp) {

    if(expr_result->data_type[0] == 's') {  //stringy se musi porovnat s "" a "0"
        char *jmp_empty_string = get_rand_name(symbtable, "_jmp_decide");
        char *jmp_zero_string = get_rand_name(symbtable, "_jmp_decide");
        char *jmp_decider = get_rand_name(symbtable, "_jmp_decide");
        if(EXIT_CODE != 0) {
            free(jmp_empty_string);
            free(jmp_zero_string);
            free(jmp_decider);
            return 1;
        }
        bst_insert(symbtable, jmp_empty_string, var_id, NULL);
        bst_insert(symbtable, jmp_zero_string, var_id, NULL);
        bst_insert(symbtable, jmp_decider, var_id, NULL);
        //data_type na null, nikdy se jinde nepouzije a vim, ze je to bool
        if(EXIT_CODE != 0) {
            return 1;
        }
        
        fprintf(stdout, "EQ %s@%s %s@%s string@\n", frame_name, jmp_empty_string,
                                                    frame_name, expr_result->key);
        fprintf(stdout, "EQ %s@%s %s@%s string@0\n", frame_name, jmp_zero_string,
                                                    frame_name, expr_result->key);
        fprintf(stdout, "OR %s@%s %s@%s %s@%s\n", frame_name, jmp_decider,
                                                    frame_name, jmp_zero_string,
                                                    frame_name, jmp_empty_string);
        fprintf(stdout, "JMPIFEQ %s %s@%s bool@true\n", label_for_jmp, frame_name, jmp_decider);

    } else if(expr_result->data_type[0] == 'n') {  //s null bude porovnani vzdy false -> JMP
        fprintf(stdout, "JMP %s\n", label_for_jmp);
    } else {    //ostatni se porovnava jen se jednou veci
        char compare_with_result[13];
        switch(expr_result->data_type[0]) {
            case 'b':
                strcpy(compare_with_result, "bool@false");
                break;
            case 'i':
                strcpy(compare_with_result, "int@0");
                break;
            case 'f':
                strcpy(compare_with_result, "float@0x0p+0");
                break;
            default:
                fprintf(stderr, "Interní chyba, neočekávaný datový typ v IF-u\n");
                return 1;
        }

        fprintf(stdout, "JMPIFEQ %s %s@%s %s\n",
            label_for_jmp, frame_name, expr_result->key, compare_with_result);
    }
    return 0;
}

int IF_nt(stack_t *stack, bst_node_t **global_symbtab, bst_node_t **local_symbtab) {
	next_stack_token(stack, &GET_NEXT_TOKEN_INDEX);    //pozere if

    //zvoli, ktera z tabulek se bude pouzivat
    bst_node_t **symbtable;
    bool in_function;
    char frame_name[3];
    if(local_symbtab == NULL) {
        symbtable = global_symbtab;
        in_function = false;
        strcpy(frame_name, "GF");
    } else {
        symbtable = local_symbtab;
        in_function = true;
        strcpy(frame_name, "LF");
    }

    bst_node_t *expr_result = bottom_up_parser(stack, &GET_NEXT_TOKEN_INDEX, symbtable, in_function, false, IN_WHILE_LOOPS, cond_table);
    if(EXIT_CODE != 0) {
        return 1;
    }

    //vygeneruje navesti pro podmineny skok a strci je do tabulky symbolu
    char *label_for_else = get_rand_name(symbtable, "_else_label"); //label, kam se skoci v pripade neplatnosti podminky (else)
    char *label_for_if = get_rand_name(symbtable, "_if_label");     //label, kam se skoci, aby se v pripade pravdivosti podminky neprovedl else 
    if(EXIT_CODE != 0) {
        free(label_for_else);
        free(label_for_if);
        return 1;
    }
    bst_insert(symbtable, label_for_else, label_id, NULL);
    bst_insert(symbtable, label_for_if, label_id, NULL);
    if(EXIT_CODE != 0) {
        return 1;
    }

    //zpracuje podminku a vygeneruje prislusne instrukce
    conditions_parse(expr_result, symbtable, frame_name, label_for_else);
    if(EXIT_CODE != 0) {
        return 1;
    }


    SS_nt(stack, global_symbtab, local_symbtab);
    if(EXIT_CODE != 0) {
        return 1;
    }

	token_t *first_token = next_stack_token(stack, &GET_NEXT_TOKEN_INDEX);
	token_t *second_token = next_stack_token(stack, &GET_NEXT_TOKEN_INDEX);
    //kontrola,ze se zde nachazi "else {"
    if(strcmp(first_token->content, "else") != 0)  {
        EXIT_CODE = 2;
        fprintf(stderr, "Syntaktická chyba na řádku:%d\nNeočekávaný lexém %s\n",
            first_token->line_num, first_token->content);
        return 1;
    } else if (second_token->variant != open_curl_var) {
        EXIT_CODE = 2;
        fprintf(stderr, "Syntaktická chyba na řádku:%d\nNeočekávaný lexém %s\n",
            second_token->line_num, second_token->content);
        return 1;
    }

    fprintf(stdout, "JMP %s\n", label_for_if);
    fprintf(stdout, "LABEL %s\n", label_for_else);

    SS_nt(stack, global_symbtab, local_symbtab);
    if(EXIT_CODE != 0) {
        return 1;
    }

    fprintf(stdout, "LABEL %s\n", label_for_if);
    return 0;
}

int CYC_nt(stack_t *stack, bst_node_t **global_symbtab, bst_node_t **local_symbtab)
{
	next_stack_token(stack, &GET_NEXT_TOKEN_INDEX);    //pozere while

    //zvoli, jaka tabulka se bude pouzivat a k jakemu frame-u se bude pristupovat
    bst_node_t **symbtable;
    bool in_function;
    char frame_name[3];
    if(local_symbtab == NULL) {
        symbtable = global_symbtab;
        in_function = false;
        strcpy(frame_name, "GF");
    } else {
        symbtable = local_symbtab;
        in_function = true;
        strcpy(frame_name, "LF");
    }

    //pokud jeste v tomto scopu neni definovana promenna pro ukladani typu, definujeme ji
    if(bst_search(*symbtable, NO_REDEF_TYPE_VAR) == NULL) {
        char *no_redef_type = calloc(strlen(NO_REDEF_TYPE_VAR) + 1, 1);
        if(no_redef_type == NULL) {
            EXIT_CODE = 99;
            fprintf(stderr, "Chyba alokace paměti!\n");\
            return 1;
        }
        strcpy(no_redef_type, NO_REDEF_TYPE_VAR);
        bst_insert(symbtable, no_redef_type, var_id, NULL);
        if(EXIT_CODE != 0) {
            return 1;
        }
        fprintf(stdout, "DEFVAR %s@%s\n", frame_name, no_redef_type);
    }

    //vytvoreni navesti pro konec a zacatek loop a vlozeni do tabulky symbolu 
    char *start_loop = get_rand_name(symbtable, "_loop_start");
    char *end_loop = get_rand_name(symbtable, "_loop_end");
    if(EXIT_CODE != 0) {
        free(start_loop);
        free(end_loop);
        return 1;
    }
    bst_insert(symbtable, start_loop, label_id, NULL);
    bst_insert(symbtable, end_loop, label_id, NULL);
    if(EXIT_CODE != 0) {
        return 1;
    }

    fprintf(stdout, "LABEL %s\n", start_loop);

    IN_WHILE_LOOPS++;

    //parser zpracuje podminku a ulozi vysledek do expr_result
    bst_node_t *expr_result = bottom_up_parser(stack, &GET_NEXT_TOKEN_INDEX, symbtable, in_function, false, IN_WHILE_LOOPS, cond_table);
    if(EXIT_CODE != 0) {
        return 1;
    }

    //zpracuje podminku a vygeneruje prislusne instrukce, aby se skocilo ZA cyklus
    conditions_parse(expr_result, symbtable, frame_name, end_loop);
    if(EXIT_CODE != 0) {
        return 1;
    }

    SS_nt(stack, global_symbtab, local_symbtab);
    if(EXIT_CODE != 0) {
        return 1;
    }

    IN_WHILE_LOOPS--;

    fprintf(stdout, "LABEL %s\n", end_loop);
    return 0;
}

int FCALL_nt(stack_t *stack, bst_node_t **global_symbtab, bst_node_t **local_symbtab, char *return_type)
{
	//token variant has to be an identif_function_var ie. the f_id
	token_t *token = next_stack_token(stack, &GET_NEXT_TOKEN_INDEX), *searched_token = NULL;
	bst_node_t *searched_node = NULL;
	char *types = (char *)malloc(1);
	if(types == NULL)
	{
		//TODO MALLOC FAILED
		ERROR_OUT("MALLOC FAILED");
	}
	types[0] = '\0';
	printf("%s ", token->content);
	if(local_symbtab == NULL)
	{
		//in global scope
		if((searched_node = bst_search((*global_symbtab), token->content)) == NULL)
		{
			//TODO ERROR FUNCTION IS NOT DEFINED
			ERROR_OUT("ERROR FUNCTION IS NOT DEFINED");
		}
		else
		{
			if(return_type != NULL)
			{
				if(searched_node->data_type[strlen(searched_node->data_type)-1] == '?')
				{
					inf_char_str(return_type, searched_node->data_type[strlen(searched_node->data_type)-2]);
				}
				else
				{
					inf_char_str(return_type, searched_node->data_type[strlen(searched_node->data_type)-1]);
				}
			}
			printf("%s ", token->content);
			//TODO check if return is ok
			//the next stack token is the thing after f_id ie. '(' char
			if(TL_nt(stack, searched_node->data_type, global_symbtab) == 0)
			{
				return 0;
			}
			else
			{
				return 1;
			}
		}
	}
	else //in local scope
	{
		//node is not found in global symbtab, so we check stack)
		if((searched_node = bst_search((*global_symbtab), token->content)) == NULL)
		{
			int found = 0;
			for(int i = GET_NEXT_TOKEN_INDEX; i <= stack->top; i++)
			{
				searched_token = psa_stack_get_nth_rev(stack, i);
				if(searched_token->variant == identif_function_var)
				{
					found = 1;
					for(int j = i; j <= stack->top; j++)
					{
						searched_token = psa_stack_get_nth_rev(stack, j);
						if(searched_token->variant == identif_keyword_var)
						{
							if(searched_token->content[0] == '?')
							{
								inf_char_str(types, searched_token->content[1]);
							}else
							{
								inf_char_str(types, searched_token->content[0]);
							}
						}
						else if(searched_token->variant == cls_rnd_var)
						{
							//+2 so we skip the expected ':' and check the return type
							searched_token = psa_stack_get_nth_rev(stack, j+2);
							if(searched_token->variant == identif_keyword_var)
							{
								if(searched_token->content[0] == '?')
								{
									inf_char_str(types, searched_token->content[1]);
								}else
								{
									inf_char_str(types, searched_token->content[0]);
								}
							}
							else
							{
								//TODO ERROR EXPECTED KEYWORD AS A RETURN
								ERROR_OUT("ERROR EXPECTED KEYWORD AS A RETURN");
							}
							//end the search
							goto end_loop;
						}//else skip it (ie we skip variable names)
					}
				}
			}
end_loop:
			if(!found)
			{
				//TODO ERROR FID IS NOT FOUND
				ERROR_OUT("ERROR FID IS NOT FOUND");
			}
			else
			{
				if(return_type != NULL)
				{
					if(types[strlen(types)-1] == '?')
					{
						inf_char_str(return_type, types[strlen(types)-2]);
					}else
					{
						inf_char_str(return_type, types[strlen(types)-1]);
					}
				}
				if(TL_nt(stack, types, local_symbtab) == 0)
				{
					return 0;
				}else
				{
					return 1;
				}
			}
		}
		else //when in local scope, but we do find the definition in global symbtab
		{
			if(return_type != NULL)
			{
				inf_char_str(return_type, searched_node->data_type[strlen(searched_node->data_type)-1]);
			}
			printf("%s ", token->content);
			if(TL_nt(stack, searched_node->data_type, local_symbtab) == 0)
			{
				return 0;
			}else
			{
				return 1;
			}
		}
	}
	return 0;
}

//only one symbtab because I am calling it from different places and it doesnt have to look "out of scope"
//TL_nt(stack_t *stack, bst_node_t *node, token_t *token, bst_node_t **symbtab);
int TL_nt(stack_t *stack, char *types, bst_node_t **symbtab)
{
	token_t *token = next_stack_token(stack, &GET_NEXT_TOKEN_INDEX), *searched_token = NULL;
	int check_types_index = 0;

	if(token->variant == open_rnd_var)
	{
		while((token = next_stack_token(stack, &GET_NEXT_TOKEN_INDEX))->variant != cls_rnd_var)
		{
			if(check_types_index > (int)strlen(types)-1)
			{
				//TODO ERROR TOO MANY ARGUMENTS
				ERROR_OUT("ERROR TOO MANY ARGUMENTS");
			}
			switch(token->variant)
			{
				case identif_variable_var:
					if(bst_search((*symbtab), token->content) == NULL)
					{
						//TODO ERROR VARIABLE NOT DECLARED BEFORE USE
						ERROR_OUT("ERROR VARIABLE NOT DECLARED BEFORE USE");
					}
					printf("%s ", token->content);
					break;
				case integ_var:
					if(types[check_types_index++] != 'i')
					{
						//TODO ERROR EXPECTED INT
						ERROR_OUT("ERROR EXPECTED INT");
					}
					printf("%s ", token->content);
					break;
				case float_var:
					if(types[check_types_index++] != 'f')
					{
						//TODO ERROR EXPECTED FLOAT
						ERROR_OUT("ERROR EXPECTED FLOAT");
					}
					printf("%s ", token->content);
					break;
				case string_lit_end_var:
					if(types[check_types_index++] != 's')
					{
						//TODO ERROR EXPECTED STRING
						ERROR_OUT("ERROR EXPECTED STRING");
					}
					printf("%s ", token->content);
					break;
				case comma_var:
					break;
				default:
					//TODO ERROR UNEXPECTED CHAR IN FUNCTION CALL
					ERROR_OUT("ERROR UNEXPECTED CHAR IN FUNCTION CALL");
			}
		}
		if(check_types_index != (int)strlen(types)-1)
		{
			//TODO ERROR TO FEW ARGUMENTS
			ERROR_OUT("ERROR TO FEW ARGUMENTS");
		}
	}
	else
	{
		//TODO ERROR EXPECTED '(' CHAR
		ERROR_OUT("ERROR EXPECTED '(' CHAR");
	}
	printf(") ");
	printf(";");
	return 0;
}

