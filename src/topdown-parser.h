#ifndef TOPDOWN_PARSER_H_
#define TOPDOWN_PARSER_H_

#include "scanner.h"
#include "psa_stack.h"
#include "symbtable.h"
#include "botup-parser.h"
#include "utils.h"

int PRG_nt();
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
void in_built_func_dump(bst_node_t **);

#endif
