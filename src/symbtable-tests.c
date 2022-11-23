#include "symbtable.h"

const char *subtree_prefix = "  |";
const char *space_prefix = "   ";

typedef enum direction { left, right, none } direction_t;

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
    bst_print_subtree(tree, "", none);
  } else {
    printf("Tree is empty\n");
  }
  printf("\n");
}

int main(int argc, char *argv[]) {

    char *function_name = "abs";
    char *fcn =/*insert cast :)*/malloc(strlen(function_name) + 1);
    strcpy(fcn, function_name);
    char *data_ts = "i";
    char *data_ts_mall =/*insert cast :)*/malloc(strlen(data_ts) + 1);
    strcpy(data_ts_mall, data_ts);
    symtab_node_t variant = var_id;

    bst_node_t *testing_tree;
    bst_init(&testing_tree);
    bst_print_tree(testing_tree);
    bst_insert(&testing_tree, fcn, variant, data_ts_mall);
    bst_print_tree(testing_tree);
    bst_dispose(&testing_tree);
    return 0;
}
