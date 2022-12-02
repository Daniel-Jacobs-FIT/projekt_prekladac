#include "psa_stack.h"

//nextIndex zacina na 0
token_t *next_stack_token(stack_t *stack, int *nextIndex)
{
	token_t *token = NULL;
	if(psa_stack_is_empty(stack))
	{
		return NULL;
	}
	if(*nextIndex > stack->top)
	{
		//index out of bounds
		//might remove the nextIndex wrap around
		*nextIndex = 0;
		return NULL;
	}
	token = psa_stack_get_nth_rev(stack, *nextIndex);
	*nextIndex += 1;
	return token;
}
/*
   pri chybe allokaci vraci null
*/
stack_t *psa_stack_init() {
	stack_t *stack = (stack_t *)malloc(sizeof(stack_t));
	if(stack == NULL)
	{
		return NULL;
	}
	stack->arr = (token_t **)malloc(sizeof(token_t *));
	if(stack->arr == NULL)
	{
		free(stack);
		return NULL;
	}
	stack->arr[0] = NULL;
	stack->top = -1;
	return stack;
}

/* 
   zjisti jestli je prazdny pokud top ukazuje na null
*/
bool psa_stack_is_empty(stack_t *stack) {
	//stack je prazdny pokud top je -1, implicitne taky vime ze stack->arr[0] == NULL
	return (stack->top == -1);
}

/* 
Vrati hodnotu z vrcholu zasobniku, ignoruje tokeny s variantou expression
*/
token_t *psa_stack_top_term(stack_t *stack) {
	if(psa_stack_is_empty(stack))
	{
		//vrati null pokud je stack prazdny
		return NULL;
	}

    int i = 0;
    token_t *top_token;
    do {
        top_token = stack->arr[stack->top - i];
        i++;
    } while (top_token->variant == expression_var && stack->top - i >= 0);

	return stack->arr[stack->top - (i - 1)]; //i - 1 protoze i je inkrementovano pri poslednim pruchodu cyklem
}

/* -
Vrati hodnotu n-teho elementu of vrchu
Bude potreba v urcite situaci u precedencni analyzy
Indexuje se od 0, tzn. psa_stack_get_nth(stack, 0) je to same jako psa_stack_top_term(stack) pokud je na vrcholu terminal
*/
token_t *psa_stack_get_nth(stack_t *stack, int n) {
	int index = stack->top - n;
	if(index > stack->top || index < 0)
	{
		//index out of range
		return NULL;
	}
	
	return stack->arr[index];
}

/*
funguje stejne jako psa_stack_get_nth krome toho ze jede od konce stacku
*/
token_t *psa_stack_get_nth_rev(stack_t *stack, int n) {
	int index = n;
	if(index > stack->top || index < 0)
	{
		//index out of range
		return NULL;
	}
	
	return stack->arr[index];
}
/* 
Provede realokaci (zvetseni zasovnku o 1) a vlozi hodnotu elementu na vrchol zasobniku
Pri selhani realokace pameti vrati -1
*/
int psa_stack_push(stack_t *stack, token_t *item) {
	if(psa_stack_is_empty(stack))
	{
		stack->arr[++stack->top] = item;
	}
	else //index neni -1
	{
		
		++stack->top;
		//pricitame dalsi +1 potoze indexujem od 0 a chceme dalsi prvek
		token_t **arr = (token_t **)realloc(stack->arr, sizeof(token_t *) * (stack->top+1));
		if(arr == NULL)
		{
			return -1;
		}
		stack->arr = arr;
		stack->arr[stack->top] = item;
	}
	return 1;
}

int psa_stack_split_top(stack_t *stack) {
    token_t *less_prec = create_token(NULL, less_prec_var, 0);

    if(psa_stack_top_term(stack) == psa_stack_get_nth(stack, 0)) { //na vrcholu neni expression
        psa_stack_push(stack, less_prec);
    } else { //na vrcholu lezi expression
        token_t *expression_on_top = psa_stack_get_nth(stack, 0);
		++stack->top;   //pricitame dalsi +1 potoze indexujem od 0 a chceme dalsi prvek
		token_t **arr = (token_t **)realloc(stack->arr, sizeof(token_t *) * (stack->top+1));
		if(arr == NULL)
		{
			return -1;
		}
        
        stack->arr = arr;
        stack->arr[stack->top - 1] = less_prec;
        stack->arr[stack->top] = expression_on_top;
    }
    return 1;
}

/* 
Provede realokaci (zmenseni(!) zasovnku o 1) a vlozi hodnotu elementu na vrchol zasobniku
Pri selhani realokace pameti vrati -1
*/
int psa_stack_pop(stack_t *stack) {
	if(psa_stack_is_empty(stack))
	{
		//pokud je stack prazdny
		return 0;
	}
	else
	{
		token_t **arr = NULL;
		token_t *tbd = psa_stack_get_nth(stack, 0);
		stack->arr[stack->top] = NULL;
		--stack->top;
		if(!(psa_stack_is_empty(stack)))
		{
			arr = (token_t **)realloc(stack->arr, sizeof(token_t *) * (stack->top+1));
			if(arr == NULL)
			{
				++stack->top;
				stack->arr[stack->top] = tbd;
				return -1;
			}
			stack->arr = arr;
		}
		free_token(tbd);
	}
	return 1;
}

/*
Provede uvolneni celeho zasobniku a uvede jej do stavu pred inicializaci
*/
void psa_stack_dispose(stack_t *stack) {
	for(int i = 0; i <= stack->top; i++)
	{
		if(stack->arr[i] != NULL)
		{
			free_token(stack->arr[i]);
		}
	}
	free(stack->arr);
	free(stack);
}
