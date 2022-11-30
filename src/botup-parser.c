#include "botup-parser.h"

const prec_table_t ass_table[NUM_OF_TOKEN_VARS][NUM_OF_TOKEN_VARS] = {
//       +   -   .   *   /   >   >=  <   <=  === !== id  f_e f   i   s   nil exp (   )   ;   {
/* + */ {gr, gr, gr, ls, ls, gr, gr, gr, gr, gr, gr, ls, ls, ls, ls, ER, ls, ls, ls, gr, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* - */ {gr, gr, gr, ls, ls, gr, gr, gr, gr, gr, gr, ls, ls, ls, ls, ER, ls, ls, ls, gr, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* . */ {gr, gr, gr, ls, ls, gr, gr, gr, gr, gr, gr, ls, ER, ER, ER, ls, ls, ls, ls, gr, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* * */ {gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, ls, ls, ls, ls, ER, ls, ls, ls, gr, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* / */ {gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, ls, ls, ls, ls, ER, ls, ls, ls, gr, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* > */ {ls, ls, ls, ls, ls, gr, gr, gr, gr, gr, gr, ls, ls, ls, ls, ls, ls, ls, ls, gr, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* >=*/ {ls, ls, ls, ls, ls, gr, gr, gr, gr, gr, gr, ls, ls, ls, ls, ls, ls, ls, ls, gr, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* < */ {ls, ls, ls, ls, ls, gr, gr, gr, gr, gr, gr, ls, ls, ls, ls, ls, ls, ls, ls, gr, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* <=*/ {ls, ls, ls, ls, ls, gr, gr, gr, gr, gr, gr, ls, ls, ls, ls, ls, ls, ls, ls, gr, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/*===*/ {ls, ls, ls, ls, ls, ls, ls, ls, ls, gr, gr, ls, ls, ls, ls, ls, ls, ls, ls, gr, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/*!==*/ {ls, ls, ls, ls, ls, ls, ls, ls, ls, gr, gr, ls, ls, ls, ls, ls, ls, ls, ls, gr, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* id*/ {gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, ER, ER, ER, ER, ER, ER, ER, ls, gr, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/*f_e*/ {gr, gr, ER, gr, gr, gr, gr, gr, gr, gr, gr, ER, ER, ER, ER, ER, ER, ER, ls, gr, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* f */ {gr, gr, ER, gr, gr, gr, gr, gr, gr, gr, gr, ER, ER, ER, ER, ER, ER, ER, ls, gr, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* i */ {gr, gr, ER, gr, gr, gr, gr, gr, gr, gr, gr, ER, ER, ER, ER, ER, ER, ER, ls, gr, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* s */ {ER, ER, gr, ER, ER, gr, gr, gr, gr, gr, gr, ER, ER, ER, ER, ER, ER, ER, ls, gr, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/*nil*/ {gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, ER, ER, ER, ER, ER, ER, ER, ls, gr, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/*exp*/ {gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, ER, ER, ER, ER, ER, ER, ER, ls, gr, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* ( */ {ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, eq, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* ) */ {gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, ER, ER, ER, ER, ER, ER, ER, ER, gr, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* ; */ {ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* { */ {ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
        {ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
        {ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
        {ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
        {ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
        {ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
        {ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
        {ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
        {ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
        {ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
        {ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
};

const prec_table_t cond_table[NUM_OF_TOKEN_VARS][NUM_OF_TOKEN_VARS] = {
//       +   -   .   *   /   >   >=  <   <=  === !== id  f_e f   i   s   nil exp (   )   ;   {
/* + */ {gr, gr, gr, ls, ls, gr, gr, gr, gr, gr, gr, ls, ls, ls, ls, ER, ls, ls, ls, gr, ER, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* - */ {gr, gr, gr, ls, ls, gr, gr, gr, gr, gr, gr, ls, ls, ls, ls, ER, ls, ls, ls, gr, ER, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* . */ {gr, gr, gr, ls, ls, gr, gr, gr, gr, gr, gr, ls, ER, ER, ER, ls, ls, ls, ls, gr, ER, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* * */ {gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, ls, ls, ls, ls, ER, ls, ls, ls, gr, ER, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* / */ {gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, ls, ls, ls, ls, ER, ls, ls, ls, gr, ER, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* > */ {ls, ls, ls, ls, ls, gr, gr, gr, gr, gr, gr, ls, ls, ls, ls, ls, ls, ls, ls, gr, ER, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* >=*/ {ls, ls, ls, ls, ls, gr, gr, gr, gr, gr, gr, ls, ls, ls, ls, ls, ls, ls, ls, gr, ER, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* < */ {ls, ls, ls, ls, ls, gr, gr, gr, gr, gr, gr, ls, ls, ls, ls, ls, ls, ls, ls, gr, ER, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* <=*/ {ls, ls, ls, ls, ls, gr, gr, gr, gr, gr, gr, ls, ls, ls, ls, ls, ls, ls, ls, gr, ER, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/*===*/ {ls, ls, ls, ls, ls, ls, ls, ls, ls, gr, gr, ls, ls, ls, ls, ls, ls, ls, ls, gr, ER, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/*!==*/ {ls, ls, ls, ls, ls, ls, ls, ls, ls, gr, gr, ls, ls, ls, ls, ls, ls, ls, ls, gr, ER, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* id*/ {gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, ER, ER, ER, ER, ER, ER, ER, ls, gr, ER, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/*f_e*/ {gr, gr, ER, gr, gr, gr, gr, gr, gr, gr, gr, ER, ER, ER, ER, ER, ER, ER, ls, gr, ER, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* f */ {gr, gr, ER, gr, gr, gr, gr, gr, gr, gr, gr, ER, ER, ER, ER, ER, ER, ER, ls, gr, ER, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* i */ {gr, gr, ER, gr, gr, gr, gr, gr, gr, gr, gr, ER, ER, ER, ER, ER, ER, ER, ls, gr, ER, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* s */ {ER, ER, gr, ER, ER, gr, gr, gr, gr, gr, gr, ER, ER, ER, ER, ER, ER, ER, ls, gr, ER, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/*nil*/ {gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, ER, ER, ER, ER, ER, ER, ER, ls, gr, ER, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/*exp*/ {gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, ER, ER, ER, ER, ER, ER, ER, ls, gr, ER, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* ( */ {ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, eq, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* ) */ {gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, gr, ER, ER, ER, ER, ER, ER, ER, ER, gr, ER, gr, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* ; */ {ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/* { */ {ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ls, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
        {ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
        {ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
        {ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
        {ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
        {ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
        {ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
        {ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
        {ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
        {ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
        {ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
};

int inf_char_conv(char *str, int input)
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

void float_parse(token_t *token)
{
	double num = atof(token->content);
	printf("float@%a\n", num);
}

//token with a string content to a specific type if string that is the put back into the string content
void string_parse(token_t *token)
{
	char *new_str = NULL;
	//I will need the size of the new string so I can allocate it, but maybe I can just use inf_char_input
	//and when I get a char that has a value less than 33 then I have to escape it, so Ill go into a sub function where it will be escaped
	//other special chars include 035, 092
	for(int i = 0; token->content[i] != '\0'; i++)
	{
		if(token->content[i] < 33 ||token->content[i] == 35 ||token->content[i] == 92)
		{
			int digit_count = 1, digit = (int)token->content[i];
			while((digit /= 10) >= 1)
			{
				digit_count++;
			}
			//use snprintf
			//+1 for the '\0' char and +1 for the '\' char
			char *helper_str = (char *)malloc(digit_count+2);
			snprintf(helper_str, digit_count+2, "\\%d", (int)token->content[i]);
			for(int j = 0; helper_str[j] != '\0'; j++)
			{
				if(inf_char_conv(new_str, helper_str[j]) == -1)
				{
					return;
				}
			}
			free(helper_str);
		}
		else
		{
			if(inf_char_conv(new_str, token->content[i]) == -1)
			{
				return;
			}
		}
	}
	free(token->content);
	token->content = new_str;
	printf("string@%s\n", token->content);
}
