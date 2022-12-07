/* Projekt: Implementace překladače imperativního jazyka IFJ22
 * Autoři: xuherp02, xvolr00, xvener01, xjacob00*/
#include "topdown-parser.h"

int GET_NEXT_TOKEN_INDEX = 0;
#define NUM_OF_BUILT_IN_FUNCS 11
int BUILTIN_FUNCTIONS_COUNT = NUM_OF_BUILT_IN_FUNCS ;
int BUILTIN_TYPES_COUNT = 6;
const char *built_in_types[] =  {"float", "?float", "int", "?int", "string", "?string"};
int IN_WHILE_LOOPS = 0;
bst_node_t *CURRENT_FUNCTION = NULL;
bool FOUND_RETURN = false;

const char built_in_functions[NUM_OF_BUILT_IN_FUNCS][10] = {"reads", "readi", "readf", "write", "floatval", "intval", "strval", "strlen", "substring", "ord", "chr"};
const char built_in_data_types[NUM_OF_BUILT_IN_FUNCS][6] = {"s", "i", "f", "*", "+f", "+i", "+s", "siis", "si", "is"};
const char built_in_source[NUM_OF_BUILT_IN_FUNCS][1680] = {
    "JUMP readi_end\nLABEL readi_start\nCREATEFRAME\nPUSHFRAME\n\nDEFVAR LF@readi_input\nREAD LF@readi_input int\nPUSHS LF@readi_input\n\nPOPFRAME\nRETURN\nLABEL readi_end\n",
    "JUMP readf_end\nLABEL readf_start\nCREATEFRAME\nPUSHFRAME\n\nDEFVAR LF@readf_input\nREAD LF@readf_input float \nPUSHS LF@readf_input\n\nPOPFRAME\nRETURN\nLABEL readf_end\n",
    "JUMP reads_end\nLABEL reads_start\nCREATEFRAME\nPUSHFRAME\n\nDEFVAR LF@reads_input\nREAD LF@reads_input string \nPUSHS LF@reads_input\n\nPOPFRAME\nRETURN\nLABEL reads_end\n ",
    "JUMP write_end\nLABEL write_start\nCREATEFRAME\nPUSHFRAME\nDEFVAR LF@num_of_terms\nPOPS LF@num_of_terms\nDEFVAR LF@current_term\nLABEL write_while_start\nJUMPIFEQ write_while_end LF@num_of_terms int@0\nPOPS LF@current_term\nWRITE LF@current_term\nSUB LF@num_of_terms LF@num_of_terms int@1\nJUMP write_while_start\nLABEL write_while_end\nPOPFRAME\nRETURN\nLABEL write_end\n",
    "JUMP floatval_end\nLABEL floatval_start\nCREATEFRAME\nPUSHFRAME\nDEFVAR LF@floatval_input\nDEFVAR LF@floatval_output\nDEFVAR LF@fv_type\nDEFVAR LF@fv_int_type\nPOPS LF@floatval_input\nTYPE LF@fv_type LF@floatval_input\nSTRI2INT LF@fv_int_type LF@fv_type int@0 \nJUMPIFEQ fv_int_label LF@fv_int_type int@105\nJUMPIFEQ fv_nil_label LF@fv_int_type int@110 \nJUMPIFEQ fv_float_label LF@fv_int_type int@102\nLABEL fv_int_label\nINT2FLOAT LF@floatval_output LF@floatval_input\nPUSHS LF@floatval_output\nJUMP floatval_finish\nLABEL fv_float_label\nMOVE LF@floatval_output LF@floatval_input\nPUSHS LF@floatval_output\nJUMP floatval_finish\nLABEL fv_nil_label\nMOVE LF@floatval_output float@0x0p+0\nPUSHS LF@floatval_output\nJUMP floatval_finish\nLABEL floatval_finish\nPOPFRAME\nRETURN\nLABEL floatval_end\n",
    "JUMP intval_end\nLABEL intval_start\nCREATEFRAME\nPUSHFRAME\nDEFVAR LF@intval_input\nDEFVAR LF@intval_output\nDEFVAR LF@intval_type\nDEFVAR LF@intval_int_type\nPOPS LF@intval_input\nTYPE LF@intval_type LF@intval_input\nSTRI2INT LF@intval_int_type LF@intval_type int@0 \nJUMPIFEQ intval_int_label LF@intval_int_type int@105\nJUMPIFEQ intval_nil_label LF@intval_int_type int@110 \nJUMPIFEQ intval_float_label LF@intval_int_type int@102\nLABEL intval_int_label\nMOVE LF@intval_output LF@intval_input\nPUSHS LF@intval_output\nJUMP intval_finish\nLABEL intval_float_label\nFLOAT2INT LF@intval_output LF@intval_input\nPUSHS LF@intval_output\nJUMP intval_finish\nLABEL intval_nil_label\nMOVE LF@intval_output int@0\nPUSHS LF@intval_output\nJUMP intval_finish\nLABEL intval_finish\nPOPFRAME\nRETURN\nLABEL intval_end\n",
    "#MISSING STRVAL!\n",  //TODO
    "JUMP strlen_end\nLABEL strlen_start\nCREATEFRAME\nPUSHFRAME\nDEFVAR LF@strlen_input\nDEFVAR LF@strlen_output\nPOPS LF@strlen_input\nSTRLEN LF@strlen_output LF@strlen_input\nPUSHS LF@strlen_output\nPOPFRAME\nRETURN\nLABEL strlen_end\n",
    "JUMP substring_end\nLABEL substring_start\nCREATEFRAME\nPUSHFRAME\nDEFVAR LF@substring_input_string\nDEFVAR LF@substring_input_i \nDEFVAR LF@substring_input_j\nDEFVAR LF@substring_output\nDEFVAR LF@temp_string\nDEFVAR LF@condition_bool\nDEFVAR LF@substring_strlen\nPOPS LF@substring_input_string\nPOPS LF@substring_input_i\nPOPS LF@substring_input_j\nSTRLEN LF@substring_strlen LF@substring_input_string\nLT LF@condition_bool LF@substring_input_j int@0\nJUMPIFEQ index_error LF@condition_bool bool@true\nLT LF@condition_bool LF@substring_input_i int@0\nJUMPIFEQ index_error LF@condition_bool bool@true\nGT LF@condition_bool LF@substring_input_i LF@substring_input_j\nJUMPIFEQ index_error LF@condition_bool bool@true\nGT LF@condition_bool LF@substring_input_j LF@substring_strlen\nJUMPIFEQ index_error LF@condition_bool bool@true\nGT LF@condition_bool LF@substring_input_i LF@substring_strlen\nJUMPIFEQ index_error LF@condition_bool bool@true\nEQ LF@condition_bool LF@substring_input_i LF@substring_strlen\nJUMPIFEQ index_error LF@condition_bool bool@true\nJUMPIFEQ index_error LF@substring_input_i LF@substring_input_j\nGETCHAR LF@substring_output LF@substring_input_string LF@substring_input_i\nLABEL string_loop\nADD LF@substring_input_i LF@substring_input_i int@1\nJUMPIFEQ substring_push LF@substring_input_i LF@substring_input_j\nGETCHAR LF@temp_string LF@substring_input_string LF@substring_input_i\nCONCAT LF@substring_output LF@substring_output LF@temp_string \nJUMP string_loop\nLABEL substring_push\nPUSHS LF@substring_output\nWRITE LF@substring_output\nJUMP substring_finish\nLABEL index_error\nPUSHS nil@nil\nLABEL substring_finish\nPOPFRAME\nRETURN\nLABEL substring_end\n",
    "JUMP ord_end\nLABEL ord_start\nCREATEFRAME\nPUSHFRAME\nDEFVAR LF@ord_temp\nDEFVAR LF@ord_input\nDEFVAR LF@ord_output\nPOPS LF@ord_input\nSTRLEN LF@ord_temp LF@ord_input\nJUMPIFEQ strlen_zero LF@ord_temp int@0\nSTRI2INT LF@ord_temp LF@ord_input int@0 \nMOVE LF@ord_output LF@ord_temp\nJUMP strlen_finish\nLABEL strlen_zero\nMOVE LF@ord_output int@0\nJUMP strlen_finish\nLABEL strlen_finish\nPUSHS LF@ord_output\nPOPFRAME\nRETURN\nLABEL ord_end\n",
    "JUMP chr_end\nLABEL chr_start\nCREATEFRAME\nPUSHFRAME\nDEFVAR LF@chr_input\nDEFVAR LF@chr_output\nPOPS LF@chr_input\nINT2CHAR LF@chr_output LF@chr_input\nWRITE LF@chr_output\nPOPFRAME\nRETURN\nLABEL chr_end\n"};




//TODO smazat, depracated:
typedef enum direction { left, right, nil } direction_t;
const char *subtree_prefix = "  |";
const char *space_prefix = "   ";
void bst_print_node(bst_node_t *);
char *make_prefix(char *, const char *);
void bst_print_subtree(bst_node_t *, char *, direction_t);
void bst_print_tree(bst_node_t *);




void bst_print_node(bst_node_t *node) {
    printf("[%s, %d, %s]", node->key, node->sym_var, node->data_type);
}

char *make_prefix(char *prefix, const char *suffix) {
  char *result = (char *)malloc(strlen(prefix) + strlen(suffix) + 1);
  strcpy(result, prefix);
  result = strcat(result, suffix);
  return result;
}

void bst_print_subtree(bst_node_t *tree, char *prefix, direction_t from) {
  if (tree != NULL) {
    char *current_subtree_prefix = make_prefix(prefix, subtree_prefix);
    char *current_space_prefix = make_prefix(prefix, space_prefix);

    if (from == left) {
      printf("%s\n", current_subtree_prefix);
    }

    bst_print_subtree(
        tree->right,
        from == left ? current_subtree_prefix : current_space_prefix, right);

    printf("%s  +-", prefix);
    bst_print_node(tree);
    printf("\n");

    bst_print_subtree(
        tree->left,
        from == right ? current_subtree_prefix : current_space_prefix, left);

    if (from == right) {
      printf("%s\n", current_subtree_prefix);
    }

    free(current_space_prefix);
    free(current_subtree_prefix);
  }
}

void bst_print_tree(bst_node_t *tree) {
  printf("Binary tree structure:\n");
  printf("\n");
  if (tree != NULL) {
    bst_print_subtree(tree, "", nil);
  } else {
    printf("Tree is empty\n");
  }
  printf("\n");
}

//end temp def
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

#define ERROR_OUT(MSG, LNUM, ERR_CODE)\
	fprintf(stderr, MSG, LNUM);\
	EXIT_CODE = ERR_CODE;\
	return 1;

int inf_char_str(char **str, int input)
{
	if((*str) == NULL)
	{
		char *content = (char *)malloc(2);
		if(content == NULL)
		{
			ERROR_OUT("\nChyba na řádku: %d\nchyba při alokaci paměti\n", 1, 99);
		}
		content[0] = (char)input;
		content[1] = '\0';
		(*str) = content;
	}
	else {
		int str_size = strlen((*str));
		char *content = (char *)realloc((void *)(*str), str_size+2);
		if(content == NULL)
		{
			ERROR_OUT("\nChyba na řádku: %d\nchyba při alokaci paměti\n", 1, 99);
		}
		content[str_size] = (char)input;
		content[str_size+1] = '\0';
		(*str) = content;
	}
	return 0;
}

//1 if true, else 0
int is_builtin_fce(char *str)
{
	for(int i = 0; i < BUILTIN_FUNCTIONS_COUNT; i++)
	{
		if(strcmp(str, built_in_functions[i]) == 0)
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
		if(strcmp(str, built_in_types[i]) == 0)
		{
			return 1;
		}
	}
	return 0;
}

//return type int (0 means successes, anything else is an error code)
//do you create the stack before or in PRG, logic seems to say in PRG, cause why would you want to deal with it in main
//no tokens needed either since it can get the all by itself
#define free_prg()\
bst_dispose(global_symbtab);\
free(global_symbtab);\
psa_stack_dispose(stack);

int PRG_nt()
{
    prolog_check();
    if(EXIT_CODE != 0) {
        return 1;   //nemusime nic uvolnovat, jeste se nic nealokovalo
    }

	stack_t *stack = psa_stack_init();
	bst_node_t **global_symbtab = (bst_node_t **)malloc(sizeof(bst_node_t *));
	if(global_symbtab == NULL)
	{
		free_prg();
		ERROR_OUT("\nChyba na řádku: %d\nchyba při alokaci paměti\n", 1, 99);
	}
	bst_init(global_symbtab);
	int ret_val = push_all_tokens_to_stack(stack);
	if(ret_val == 1)
	{
		free_prg();
		ERROR_OUT("\nChyba na řádku: %d\npři vkládání tokenů na stack\n", 1, 99);
	}
	if(ret_val == 2)
	{
		free_prg();
		ERROR_OUT("\nChyba na řádku: %d\npři lexikální analýze\n", 1, 1);
	}

    fprintf(stdout, ".IFJcode22\n");

    in_built_func_dump(global_symbtab);
	
	SSD_nt(stack, global_symbtab);
	free_prg();
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
}

#define free_fdef()\
bst_dispose(local_symbtab);\
free(local_symbtab);

int FDEF_nt(stack_t *stack, bst_node_t **global_symbtab)
{
	//GET_NEXT_TOKEN_INDEX gets incremented
	FOUND_RETURN = false;
	token_t *token = next_stack_token(stack, &GET_NEXT_TOKEN_INDEX);
	//printf("%s ", token->content); -> would print the 'function' keyword
	token = psa_stack_get_nth_rev(stack, GET_NEXT_TOKEN_INDEX);
	char *fce_name = token->content;
	if(token->variant != identif_function_var)
	{
		ERROR_OUT("\nChyba na řádku: %d\nočekávané jméno funkce\n", token->line_num, 2);
	}else
	{
		//check if redef
		//check if inbuilt
		if(bst_search((*global_symbtab), token->content) != NULL)
		{
			ERROR_OUT("\nChyba na řádku: %d\npokus o redefinici funkce\n", token->line_num, 3);
		}
		else if(is_builtin_fce(token->content) == 1)
		{
			ERROR_OUT("\nChyba na řádku: %d\npokus o redefinici zabudované funkce\n", token->line_num, 3);
		}
		else
		{
			//get params/return type in a string ie. float f(int, int) = "iif" <- malloc'ed
			//and the key(function name) that is malloced as well
			char *types = (char *)malloc(1);
			if(types == NULL)
			{
				ERROR_OUT("\nChyba na řádku: %d\nchyba při alokaci paměti\n", token->line_num, 99);
			}
			types[0] = '\0';
			char *new_key = (char *)calloc(strlen(token->content)+1, 1);
			if(new_key == NULL)
			{
				ERROR_OUT("\nChyba na řádku: %d\nchyba při alokaci paměti\n", token->line_num, 99);
			}
			strcpy(new_key, token->content);
			bst_insert(global_symbtab, new_key, func_id, types);
			CURRENT_FUNCTION = bst_search((*global_symbtab), token->content);
			printf("JUMP %s-end\n", fce_name);
			printf("LABEL %s-call\n", fce_name);

			//get f_id again just to move pointer
			next_stack_token(stack, &GET_NEXT_TOKEN_INDEX);
			//eat the expected '(' char and check if it actually is an '('
			token = next_stack_token(stack, &GET_NEXT_TOKEN_INDEX);
			if(token->variant == open_rnd_var)
			{
				bst_node_t **local_symbtab = (bst_node_t **)malloc(sizeof(bst_node_t *));
				if(local_symbtab == NULL)
				{
					ERROR_OUT("\nChyba na řádku: %d\nchyba při alokaci paměti\n", token->line_num, 99);
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
							types = CURRENT_FUNCTION->data_type;
							if(token->content[0] != '?')
							{
								inf_char_str(&types, token->content[0]);
								if(EXIT_CODE != 0)
								{
									return 1;
								}
							}
							else
							{
								inf_char_str(&types, token->content[1]);
								if(EXIT_CODE != 0)
								{
									return 1;
								}
							}
							CURRENT_FUNCTION->data_type = types;
							//TODO
							//printf(": %s ", token->content);
							
							token = next_stack_token(stack, &GET_NEXT_TOKEN_INDEX);
							if(token->variant == open_curl_var)
							{
								//printf("{ \n");
								if(SS_nt(stack, global_symbtab, local_symbtab) != 0)
								{
									return 1;
								}
								//at the end of a function I need:
								if(types[strlen(types)-1] != 'v')
								{
									if(FOUND_RETURN == false)
									{
										ERROR_OUT("\nChyba na řádku: %d\nočekávaný return\n", token->line_num, 6);
									}
								}
								CURRENT_FUNCTION = NULL;
								printf("POPFRAME\n");
								printf("RETURN\n");
								printf("LABEL %s-end\n", fce_name);
								free_fdef();
								return 0;
							}else
							{
								free_fdef();
								ERROR_OUT("\nChyba na řádku: %d\nočekávaný lexém '{' po návratovém typu\n", token->line_num, 2);
							}
						}
						else
						{
							free_fdef();
							ERROR_OUT("\nChyba na řádku: %d\nočekávaný zabudovaný datový typ\n", token->line_num, 2);
						}
					}
					else
					{
						free_fdef();
						ERROR_OUT("\nChyba na řádku: %d\nočekávané klíčové slovo\n", token->line_num, 2);
					}
				}else
				{
					free_fdef();
					ERROR_OUT("\nChyba na řádku: %d\nočekávaný lexém ':' po parametrech funkce\n", token->line_num, 2);
				}
			}else
			{
				ERROR_OUT("\nChyba na řádku: %d\nočekávaný lexém '('\n", token->line_num, 2);
			}
		}
	}
	return 0;
}

int PL_nt(stack_t *stack, char *types, bst_node_t **symbtab)
{
	char *type = NULL;
	token_t *token = next_stack_token(stack, &GET_NEXT_TOKEN_INDEX);
	if(token->variant == cls_rnd_var)
	{
		//return successes
		return 0;
	}
	else if(token->variant == identif_keyword_var)
	{
add_param:
		if(is_builtin_type(token->content))
		{
			//add the data type of param 1 into types
			if(token->content[0] == '?')
			{
				inf_char_str(&types, token->content[1]);
				if(EXIT_CODE != 0)
				{
					return 1;
				}
			}else
			{
				inf_char_str(&types, token->content[0]);
				if(EXIT_CODE != 0)
				{
					return 1;
				}
			}
			CURRENT_FUNCTION->data_type = types;
			token = next_stack_token(stack, &GET_NEXT_TOKEN_INDEX);
			if(token->variant == identif_variable_var)
			{
				inf_char_str(&type, types[strlen(types)-1]);
				if(EXIT_CODE != 0)
				{
					return 1;
				}
				//this is what we want to happen
				char *new_key = (char *)calloc(strlen(token->content)+1, 1);
				if(new_key == NULL)
				{
					ERROR_OUT("\nChyba na řádku: %d\nchyba při alokaci paměti\n", token->line_num, 99);
				}
				strcpy(new_key, token->content);
				bst_insert(symbtab, new_key, var_id, type);
				type = NULL;
				printf("DEFVAR LF@%s\n", token->content);
				printf("POPS ");
				parse_switch(token, "LF");
			}
			else
			{
				ERROR_OUT("\nChyba na řádku: %d\nočekávané jméno promněné po datovém typu\n", token->line_num, 2);
			}
			token = next_stack_token(stack, &GET_NEXT_TOKEN_INDEX);
			if(token->variant == cls_rnd_var)
			{
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
				ERROR_OUT("\nChyba na řádku: %d\nočekávaný lexém ')' nebo ',' po parametru\n", token->line_num, 2);
			}
		}
		else
		{
			ERROR_OUT("\nChyba na řádku: %d\nočekávaný zabudovaný datový typ\n", token->line_num, 2);
		}
	}
	else
	{
expected_data_type:
		ERROR_OUT("\nChyba na řádku: %d\nočekávaný datový typ\n", token->line_num, 2);
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
		if(FCALL_nt(stack, global_symbtab, local_symbtab, NULL) != 0)
		{
			return 1;
		}
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
            fprintf(stdout, "JUMPIFNEQ %s %s@%s string@\n", no_redef_label,
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
		token = psa_stack_get_nth_rev(stack, GET_NEXT_TOKEN_INDEX);
		if(token->variant == identif_function_var)  //je to volani funkce
		{
			//token = next_stack_token(stack, &GET_NEXT_TOKEN_INDEX);
			char return_data_type = '\0';
			if(FCALL_nt(stack, global_symbtab, local_symbtab, &return_data_type) != 0)
			{
				return 1;
			}

			if(return_data_type == 'v')
			{
				ERROR_OUT("\nChyba na řádku: %d\npřiřazení od funkce s navratovou hodnotou void\n", token->line_num, 2);
			}

			else {
				if(local_symbtab == NULL) { //pracuju s globalni tabulkou
					if(var_in_symbtab == NULL) {    //promenna jeste nebyla definovana
						bst_insert(global_symbtab, new_var_name, var_id, new_data_type);
						var_in_symbtab = bst_search(*global_symbtab, new_var_name);
						var_in_symbtab->data_type[0] = return_data_type;
					} else {    //promenna je jiz definovana
						var_in_symbtab->data_type[0] = return_data_type;
					}
				}
			}
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

    FOUND_RETURN = true;

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
        fprintf(stdout, "JUMPIFEQ %s %s@%s bool@true\n", label_for_jmp, frame_name, jmp_decider);

    } else if(expr_result->data_type[0] == 'n') {  //s null bude porovnani vzdy false -> JUMP
        fprintf(stdout, "JUMP %s\n", label_for_jmp);
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

        fprintf(stdout, "JUMPIFEQ %s %s@%s %s\n",
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

    fprintf(stdout, "JUMP %s\n", label_for_if);
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

#define free_fcall()\
free(types);\

int FCALL_nt(stack_t *stack, bst_node_t **global_symbtab, bst_node_t **local_symbtab, char *return_type)
{
	//token variant has to be an identif_function_var ie. the f_id
	token_t *token = next_stack_token(stack, &GET_NEXT_TOKEN_INDEX), *searched_token = NULL;
	bst_node_t *searched_node = NULL;
	char *types = (char *)malloc(1);
	if(types == NULL)
	{
		free_fcall();
		ERROR_OUT("\nChyba na řádku: %d\nchyba při alokaci paměti\n", token->line_num, 99);
	}
	types[0] = '\0';
	printf("CREATEFRAME\n");
	if(local_symbtab == NULL)
	{
		//in global scope
		if((searched_node = bst_search((*global_symbtab), token->content)) == NULL)
		{
			free_fcall();
			ERROR_OUT("\nChyba na řádku: %d\nfunkce nebyla definována před použitím\n", token->line_num, 3);
		}
		else
		{
			if(searched_node->data_type[strlen(searched_node->data_type)-1] == '?')
			{
				*return_type =  searched_node->data_type[strlen(searched_node->data_type)-2];
			}
			else
			{
				*return_type =  searched_node->data_type[strlen(searched_node->data_type)-1];
			}
			//TODO check if return is ok
			//the next stack token is the thing after f_id ie. '(' char
			if(TL_nt(stack, searched_node->data_type, global_symbtab) == 0)
			{
				printf("PUSHFRAME\n");
				printf("CALL %s-call\n", token->content);
				free_fcall();
				return 0;
			}
			else
			{
				free_fcall();
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
								inf_char_str(&types, searched_token->content[1]);
								if(EXIT_CODE != 0)
								{
									return 1;
								}
							}else
							{
								inf_char_str(&types, searched_token->content[0]);
								if(EXIT_CODE != 0)
								{
									return 1;
								}
							}
							CURRENT_FUNCTION->data_type = types;
							//TODO
						}
						else if(searched_token->variant == cls_rnd_var)
						{
							//+2 so we skip the expected ':' and check the return type
							searched_token = psa_stack_get_nth_rev(stack, j+2);
							if(searched_token->variant == identif_keyword_var)
							{
								if(searched_token->content[0] == '?')
								{
									inf_char_str(&types, searched_token->content[1]);
									if(EXIT_CODE != 0)
									{
										return 1;
									}
								}else
								{
									inf_char_str(&types, searched_token->content[0]);
									if(EXIT_CODE != 0)
									{
										return 1;
									}
								}
								CURRENT_FUNCTION->data_type = types;
								//TODO
							}
							else
							{
								free_fcall();
								ERROR_OUT("\nChyba na řádku: %d\nnavratová hodnota není klíčové slovo\n", token->line_num, 2);
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
				free_fcall();
				ERROR_OUT("\nChyba na řádku: %d\nfunkce nebyla definována před použitím\n", token->line_num, 3);
			}
			else
			{
				if(types[strlen(types)-1] == '?')
				{
					*return_type =  types[strlen(types)-2];
				}
				else
				{
					*return_type =  types[strlen(types)-1];
				}

				if(TL_nt(stack, types, local_symbtab) == 0)
				{
					free_fcall();
					return 0;
				}else
				{
					free_fcall();
					return 1;
				}
			}
		}
		else //when in local scope, but we do find the definition in global symbtab
		{
			if(searched_node->data_type[strlen(searched_node->data_type)-1] == '?')
			{
				*return_type =  searched_node->data_type[strlen(searched_node->data_type)-2];
			}
			else
			{
				*return_type =  searched_node->data_type[strlen(searched_node->data_type)-1];
			}
			//printf("what: %s ", token->content);
			if(TL_nt(stack, searched_node->data_type, local_symbtab) == 0)
			{
				printf("PUSHFRAME\n");
				printf("CALL %s-call\n", token->content);
				free_fcall();
				return 0;
			}else
			{
				free_fcall();
				return 1;
			}
		}
	}
	free_fcall();
	return 0;
}

//only one symbtab because I am calling it from different places and it doesnt have to look "out of scope"
//TL_nt(stack_t *stack, bst_node_t *node, token_t *token, bst_node_t **symbtab);
int TL_nt(stack_t *stack, char *types, bst_node_t **symbtab)
{
	token_t *token = next_stack_token(stack, &GET_NEXT_TOKEN_INDEX);
	stack_t *all_params_stack = NULL;
	//all_params_stack is automatically disposed
	all_params_stack = psa_stack_init();
	int check_types_index = 0;

	if(token->variant == open_rnd_var)
	{
		while((token = copy_token(stack, &GET_NEXT_TOKEN_INDEX))->variant != cls_rnd_var)
		{
			//-2 to lose the return type and because of indexing from 0
			if(check_types_index > (int)strlen(types)-2)
			{
				ERROR_OUT("\nChyba na řádku: %d\nneočekávaně velký počet argumentů\n", token->line_num, 4);
			}
			switch(token->variant)
			{
				case identif_variable_var:
					if(bst_search((*symbtab), token->content) == NULL)
					{
						ERROR_OUT("\nChyba na řádku: %d\npromněná nebyla deklarována před použitím\n", token->line_num, 5);
					}
					psa_stack_push(all_params_stack, token);
					break;
				case integ_var:
					if(types[check_types_index++] != 'i')
					{
						ERROR_OUT("\nChyba na řádku: %d\nneočekáváný int\n", token->line_num, 4);
					}
					psa_stack_push(all_params_stack, token);
					break;
				case float_var:
					if(types[check_types_index++] != 'f')
					{
						ERROR_OUT("\nChyba na řádku: %d\nneočekáváný float\n", token->line_num, 4);
					}
					psa_stack_push(all_params_stack, token);
					break;
				case string_lit_end_var:
					if(types[check_types_index++] != 's')
					{
						ERROR_OUT("\nChyba na řádku: %d\nneočekáváný řetězec\n", token->line_num, 4);
					}
					psa_stack_push(all_params_stack, token);
					break;
				case comma_var:
					free(token);
					break;
				default:
				ERROR_OUT("\nChyba na řádku: %d\nočekáváný lexém\n", token->line_num, 2);
			}
		}
		if(check_types_index != (int)strlen(types)-1)
		{
			ERROR_OUT("\nChyba na řádku: %d\nneočekávaně malý počet argumentů\n", token->line_num, 4);
		}
		free(token);
	}
	else
	{
		ERROR_OUT("\nChyba na řádku: %d\nočekáváný lexém ')'\n", token->line_num, 2);
	}

	token = next_stack_token(stack, &GET_NEXT_TOKEN_INDEX);
	if(token->variant != semicol_var)
	{
		ERROR_OUT("\nChyba na řádku: %d\nočekáváný lexém ';'\n", token->line_num, 2);
	}
	
	token_t *print_token = NULL;
	while(!psa_stack_is_empty(all_params_stack))
	{
		print_token = psa_stack_top_term(all_params_stack);
		printf("PUSH ");
		parse_switch(print_token, "LF");
		if(psa_stack_pop(all_params_stack) == -1)
		{
				ERROR_OUT("\nChyba na řádku: %d\nChyba při realokaci paměti\n", token->line_num, 99);
		}
	}
	psa_stack_dispose(all_params_stack);

	return 0;
}

void in_built_func_dump(bst_node_t **symbtable) {
    char *func_name;
    char *func_data_types;

    for(int i = 0; i < NUM_OF_BUILT_IN_FUNCS; i++) {

        fprintf(stdout, "%s\n", built_in_source[i]);

        func_name = calloc(strlen(built_in_functions[i]) + 1, 1);
        func_data_types = calloc(strlen(built_in_data_types[i]) + 1, 1);
        if(func_name == NULL || func_data_types == NULL) {
            free(func_name);
            free(func_data_types);
            EXIT_CODE = 99;
            fprintf(stderr, "Chyba alokace paměti!\n");
            return;
        }

        strcpy(func_name, built_in_functions[i]);
        strcpy(func_data_types, built_in_data_types[i]);

        bst_insert(symbtable, func_name, func_id, func_data_types);
        if(EXIT_CODE != 0) {
            free(func_name);
            free(func_data_types);
            return;
        }
    }
}
