#include "scanner.h"

typedef enum {gr, ls, eq, ER} prec_table_t;
#define PREC_TABLE_VARS '>', '<', '=', ' '
