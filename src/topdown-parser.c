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

//0 sucesses, else fail
int push_all_tokens_to_stack(stack *stack)
{
	token_t *token = NULL;
	while((token = get_token())->variant != end_prg_var)
	{
		if(psa_stack_push(stack, token) == -1)
		{
			//realloc error
			psa_stack_dispose(stack);
			return 1;
		}
	}
	return 0;
}

//return type int (0 means sucesses, anything else is an error code)
//do you create a stuff before or in PRG, logic seems to say in PRG, cause why would you want to deal with it in main
//no tokens needed either since it can get the all by itself
int PRG_nt()
{
	stack_t *stack = psa_stack_init();
	bst_node_t **global_symbtable = (bst_node_t **)malloc(sizeof(bst_node_t *));
	if(global_symbtable == NULL)
	{
		//TODO malloc failed
	}
	bst_init(global_symbtable);
	if(push_all_tokens_to_stack(stack) == 1)
	{
		//TODO failed to push all tokens to stack
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
SSD_nt(stack_t *stack, bst_node_t **global_symbtab)
{
	
}
