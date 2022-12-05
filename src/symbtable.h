#ifndef SYMTABLE_H
#define SYMTABLE_H

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"

typedef enum {var_id, func_id} symtab_node_t;

//uzel stromu
typedef struct bst_node {
  char *key;                // klic - jmeno funkce/promenne
  symtab_node_t sym_var;    // varianta symbolu - identifikator funkce nebo promenne
  char *data_type;          // datovy typ symbolu, u funkce typ navratove hodnoty a zbyle dat. typy parametru
  struct bst_node *left;    // levý potomek
  struct bst_node *right;   // pravý potomek
} bst_node_t;

void bst_init(bst_node_t **tree);
void bst_insert(bst_node_t **tree, char *key, symtab_node_t sym_var, char *data_type);
bst_node_t *bst_search(bst_node_t *tree, char *key);
void bst_delete(bst_node_t **tree, char *key);
void bst_dispose(bst_node_t **tree);

void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree);

void bst_print_node(bst_node_t *node);

#endif
