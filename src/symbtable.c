#include "symbtable.h"

/*
funkce pro inicializaci stromu
    param: tree ukazatel na ukazatel na strom
 */
void bst_init(bst_node_t **tree) {
	(*tree) = NULL;
}

/*
Funkce pro nalezeni symbolu ve strome
    return: vraci NULL, pokud nic nenajde, pokud najde, vraci ukazatel na prislusny uzel 
    tree = ukazatel na korenovy uzel
    key = klic 
*/
bst_node_t *bst_search(bst_node_t *tree, char *key) {
	if(tree == NULL)
	{
		return NULL;
	}
	if(strcmp(key, tree->key) == 0) //nasli jsme uzel s hledanym klicem
	{
		return tree;
	}
	else if(strcmp(key, tree->key) < 0) //hledame v levem podstrome
	{
		if(tree->left == NULL)
		{
			return NULL;
		}else
		{
			return bst_search(tree->left, key);
		}
	}
	else //hledame v pravem podstrome
	{
		if(tree->right == NULL)
		{
			return NULL;
		}else
		{
			return bst_search(tree->right, key);	
		}
	}
}

/*
Funkce pro vlozeni uzlu s daty symbolu do stromu. Pokud uzel s danym klicem existuje, prepise jeho hodnoty
    tree = ukazatel na ukazatel na strom
    key = retezec s klicem
    sym_var = enum pro variantu symbolu funkce/promenna
    data_type = retezec s datovym typem promenne/datovymi typy navratove hodnoty funkce a jejich parametru
*/
void bst_insert(bst_node_t **tree, char *key, symtab_node_t sym_var, char *data_type) {
	if((*tree) == NULL)
	{
		bst_node_t *new_node = (bst_node_t *)malloc(sizeof(bst_node_t));
		if(new_node == NULL)
		{
			fprintf(stderr, "malloc failed\n");
			return;
		}
		new_node->left = NULL;
		new_node->right = NULL;
		new_node->key = key;
		new_node->sym_var = sym_var;
		new_node->data_type = data_type;
		
		(*tree) = new_node;
	}
	if(strcmp(key, (*tree)->key) == 0) //nasli jsme uzel se stejnym klicem
	{
		(*tree)->sym_var = sym_var;
		(*tree)->data_type = data_type;
	}
	else if(strcmp(key, (*tree)->key) < 0) //novy uzel ma byt zarazen do leveho podstromu
	{
		if((*tree)->left == NULL)
		{
			bst_node_t *new_node = (bst_node_t *)malloc(sizeof(bst_node_t));
			if(new_node == NULL)
			{
				fprintf(stderr, "malloc failed\n");
				return;
			}
			new_node->left = NULL;
			new_node->right = NULL;
            new_node->key = key;
            new_node->sym_var = sym_var;
            new_node->data_type = data_type;

			(*tree)->left = new_node;
		}else
		{
			 bst_insert(&(*tree)->left, key, sym_var, data_type);
		}
	}
	else
	{
		if((*tree)->right == NULL)
		{
			bst_node_t *new_node = (bst_node_t *)malloc(sizeof(bst_node_t));
			if(new_node == NULL)
			{
				fprintf(stderr, "malloc failed\n");
				return;
			}
			new_node->left = NULL;
			new_node->right = NULL;
            new_node->key = key;
            new_node->sym_var = sym_var;
            new_node->data_type = data_type;

			(*tree)->right = new_node;
		}else
		{
			 bst_insert(&(*tree)->right, key, sym_var, data_type);	
		}
	}
}

/*
Pomocna funkce pro bst_delete, ktera nahradi uzel jeho nejpravejsim potomkem v levem podstromu
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
	//funkce predpoklada ze hodnota tree neni null
	if((*tree)->right == NULL)
	{
		bst_node_t *tbd = (*tree);
        free(target->key);
        free(target->data_type);
		target->key = tbd->key;
		target->sym_var = tbd->sym_var;
        target->data_type = tbd->data_type;
		(*tree) = tbd->left;
		free(tbd);
		return;
	}
	if((*tree)->right->right != NULL)
	{
		bst_replace_by_rightmost(target, &(*tree)->right);
	}else
	{
		bst_node_t *tbd = (*tree)->right;
        free(target->key);
        free(target->data_type);
		target->key = tbd->key;
		target->sym_var = tbd->sym_var;
        target->data_type = tbd->data_type;
		(*tree)->right = tbd->left;
		free(tbd);
	}
}

/*
Funkce pro odstraneni uzlu ve strome, pokud uzel s danym klicem neexistuje, nic se neprovede, pokud ma jeden podstrom, zdedi jej otec. Pokud ma oba podstromy je nahrazen nejpravejsim uzlem leveho podstromu.
    tree = ukazatel na ukazatel na strom
    key = retezec s klicem
 */
void bst_delete(bst_node_t **tree, char *key) {
	if((*tree) == NULL)
	{
		//vymazani z prazdneho stromu
		return;
	}
	if(strcmp((*tree)->key, key) == 0) //nasli jsme uzel se zadanym klicem
	{
		bst_node_t *tbd = (*tree);
		if((*tree)->left == NULL && (*tree)->right == NULL)
		{
			(*tree) = NULL;
            free(tbd->key);
            free(tbd->data_type);
			free(tbd);
		}
		else if((*tree)->left != NULL && (*tree)->right == NULL)
		{
			(*tree) = tbd->left;
            free(tbd->key);
            free(tbd->data_type);
			free(tbd);
		}
		else if((*tree)->left == NULL && (*tree)->right != NULL)
		{
			(*tree) = tbd->right;
            free(tbd->key);
            free(tbd->data_type);
			free(tbd);
		}
		else
		{
			bst_replace_by_rightmost(tbd, &tbd->left);
		}
	}
	else if(strcmp(key, (*tree)->key) < 0) //hledany uzel je levem podstrome 
	{
		if((*tree)->left == NULL)
		{
			return;
		}

		if(strcmp(key, (*tree)->left->key) == 0)
		{
			bst_node_t *tbd = (*tree)->left;
			if(tbd->left == NULL && tbd->right == NULL)
			{
				(*tree)->left = NULL;
                free(tbd->key);
                free(tbd->data_type);
				free(tbd);
			}
			else if(tbd->left != NULL && tbd->right == NULL)
			{
				(*tree)->left = tbd->left;
                free(tbd->key);
                free(tbd->data_type);
				free(tbd);
			}
			else if(tbd->left == NULL && tbd->right != NULL)
			{
				(*tree)->left = tbd->right;
                free(tbd->key);
                free(tbd->data_type);
				free(tbd);
			}
			else
			{
				bst_replace_by_rightmost(tbd, &tbd->left);
			}
		}
		else
		{
			bst_delete(&(*tree)->left, key);
		}
	}
	else
	{
		if((*tree)->right == NULL)
		{
			return;
		}

		if(strcmp(key, (*tree)->right->key) == 0)
		{
			bst_node_t *tbd = (*tree)->right;
			if(tbd->left == NULL && tbd->right == NULL)
			{
				(*tree)->right = NULL;
                free(tbd->key);
                free(tbd->data_type);
				free(tbd);
			}
			else if(tbd->left != NULL && tbd->right == NULL)
			{
				(*tree)->right = tbd->left;
                free(tbd->key);
                free(tbd->data_type);
				free(tbd);
			}
			else if(tbd->left == NULL && tbd->right != NULL)
			{
				(*tree)->right = tbd->right;
                free(tbd->key);
                free(tbd->data_type);
				free(tbd);
			}
			else
			{
				bst_replace_by_rightmost(tbd, &tbd->left);
			}
		}
		else
		{
			bst_delete(&(*tree)->right, key);
		}
	}
}

/*
Funkce zrusi strom, dealokuje vsechny jeho zdroje a zdroje jeho uzlu
 */
void bst_dispose(bst_node_t **tree) {
	if((*tree) == NULL)
	{
		//mazani prazdneho stromu
		return;
	}

	bst_node_t *left_child = (*tree)->left;
	bst_node_t *right_child = (*tree)->right;
    free((*tree)->key);
    free((*tree)->data_type);
	free((*tree));

	if(left_child != NULL)
	{
		bst_dispose(&left_child);
	}
	if(right_child != NULL)
	{
		bst_dispose(&right_child);
	}
	(*tree) = NULL;
}
