#include "symbtable.h"

const char *subtree_prefix = "  |";
const char *space_prefix = "   ";
char sample_keys[][10] = {"funkce_05", "funkce_10", "funkce_0", "funkce_03", "funkce_07", "funkce_06", "funkce_99"};
char sample_types[][4] = {"vi", "vis", "sff", "fis", "v", "fff", "ii"};
#define SAMPLES_NUM 7

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

void bst_insert_many_f(bst_node_t **tree, char keys[][10], char types[][4]) {
    char *f_name;
    char *d_types;
    for(int i = 0; i < SAMPLES_NUM; i++) {
        f_name = (char *)malloc(strlen(keys[i]) + 1);
        strcpy(f_name, keys[i]);
        d_types = (char *)malloc(strlen(types[i]) + 1);
        strcpy(d_types, types[i]);

        bst_insert(tree, f_name, func_id, d_types);
    }
}

#define INIT(NAME)\
    bst_init(&testing_tree);\
    printf("=====%s TEST=====\n", NAME);\
    bst_insert_many_f(&testing_tree, sample_keys, sample_types);\
    bst_print_tree(testing_tree);

#define DISPOSE\
    bst_print_tree(testing_tree);\
    bst_dispose(&testing_tree);\
    printf("=========================================\n");

int main(int argc, char *argv[]) {

    EXIT_CODE = 0;

    bst_node_t *testing_tree;
    bst_node_t *found_node;

    INIT("SEARCH EXISTING")
    found_node = NULL;
    found_node = bst_search(testing_tree, sample_keys[3]);
    printf("Searched for key: %s\n", sample_keys[3]);
    printf("Found: ");
    if(found_node == NULL) {
        printf("Nothing\n");
    } else {
        bst_print_node(found_node);
        printf("\n\n");
    }
    DISPOSE

    INIT("SEARCH MISSING")
    found_node = NULL;
    found_node = bst_search(testing_tree, "missing_function");
    printf("Searched for key: missing_function\n");
    printf("Found: ");
    if(found_node == NULL) {
        printf("Nothing\n");
    } else {
        bst_print_node(found_node);
        printf("\n\n");
    }
    DISPOSE

    INIT("DELETE LEAF")
    bst_delete(&testing_tree, sample_keys[5]);
    DISPOSE

    INIT("DELETE ONLY LEFT")
    bst_delete(&testing_tree, sample_keys[4]);
    DISPOSE

    INIT("DELETE ONLY RIGHT")
    bst_delete(&testing_tree, sample_keys[2]);
    DISPOSE

    INIT("DELETE BOTH")
    bst_delete(&testing_tree, sample_keys[1]);
    DISPOSE

    return EXIT_CODE;
}
