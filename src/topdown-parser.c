#include "topdown-parser.h"

int GET_NEXT_TOKEN_INDEX = 0;
int BUILTIN_FUNCTIONS_COUNT = 8;
const char *builtin_functions[] = {"reads", "readi", "readf", "write", "strlen", "substring", "ord", "chr"};
int BUILTIN_TYPES_COUNT = 6;
const char *builtin_types[] =  {"float", "?float", "int", "?int", "string", "?string"};

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
int PL_nt(stack_t *, char *, bst_node_t **);
int SS_nt(stack_t *, bst_node_t **, bst_node_t **);
int FDEF_nt(stack_t *, bst_node_t **);
int SSD_nt(stack_t *, bst_node_t **);


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

//0 successes, else fail
int push_all_tokens_to_stack(stack_t *stack)
{
	token_t *token = NULL;
	while((token = get_token())->variant != end_prg_var)
	{
		if(token->variant == err_var)
		{
			psa_stack_dispose(stack);
			return 2;
		}
		if(psa_stack_push(stack, token) == -1)
		{
			//realloc error
			psa_stack_dispose(stack);
			return 1;
		}
	}
	//add the end_prg_var into the stack
	if(psa_stack_push(stack, token) == -1)
	{
		//realloc error
		psa_stack_dispose(stack);
		return 1;
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
		//return successes
		return 0;
	}
	else if(token->variant == identif_keyword_var)
	{
		if(strcmp(token->content, "function") == 0)
		{
			if(FDEF_nt(stack, global_symbtab) != 0)
			{
				return 1;
			}
		}else
		{
			printf("NOT DEVELOPED YET");
			return 1;
			//STAT_nt(stack, global_symbtab);
		}
	}else
	{
		//STAT_nt(stack, global_symbtab);
		printf("NOT DEVELOPED YET");
		return 1;
	}
	SSD_nt(stack, global_symbtab);
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

int PL_nt(stack_t *stack, char *types, bst_node_t **local_symbtab)
{
	//right now I am expecting one of these:
	/*
		PL -> )
		PL -> par_t id )
		PL -> par_t id, PL
	*/
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
				bst_insert(local_symbtab, token->content, var_id, type);
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

/* int STAT_nt(stack_t *stack, bst_node_t **global_symbtab) */
/* { */

/* } */

int SS_nt(stack_t *stack, bst_node_t **global_symbtab, bst_node_t **local_symbtab)
{
	token_t *token = next_stack_token(stack, &GET_NEXT_TOKEN_INDEX);
	if(token->variant == cls_curl_var)
	{
		printf("}\n");
		return 0;
	}else
	{
		printf("SS PART NOT FINNISHED YET\n");
		return 1;
	}
}

/* int RET_nt(stack_t *stack, bst_node_t **global_symbtab) */
/* { */
	
/* } */

/* int ASG_nt(stack_t *stack, bst_node_t **global_symbtab) */
/* { */

/* } */

/* int IF_nt(stack_t *stack, bst_node_t **global_symbtab) */
/* { */

/* } */

/* int CYC_nt(stack_t *stack, bst_node_t **global_symbtab) */
/* { */

/* } */

/* int FCALL_nt(stack_t *stack, bst_node_t **global_symbtab) */
/* { */

/* } */


/* int TL_nt(stack_t *stack, bst_node_t **global_symbtab) */
/* { */

/* } */

