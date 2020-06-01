#ifndef _SYNTREE_H
#define _SYNTREE_H

#include <stdarg.h>
#include <stdbool.h>
#include "token.h"


#define ST_mknode( ... )\
  _ST_mknode( __VA_ARGS__, (syntax_tree_t *) NULL )


typedef struct syntax_tree {

  token_t *t;
  struct syntax_tree **subtree;

} syntax_tree_t;



extern syntax_tree_t *_ST_mknode( token_t *t, ... );

extern void ST_delete( syntax_tree_t *node );

extern bool ST_isleaf( syntax_tree_t *node );

#endif
