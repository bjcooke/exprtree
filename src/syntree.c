#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "token.h"
#include "syntree.h"

#define BASE_CAPACITY 4


syntax_tree_t *_ST_mknode( token_t *t, ... ) {

  syntax_tree_t *this, **subtree;
  size_t capacity, size;
  va_list args;
  bool mulligan;


  this = (syntax_tree_t *) malloc( sizeof(syntax_tree_t) );

  if ( this != NULL ) {

    this->t = t;

    capacity = 0;

    subtree = NULL;
    mulligan = true; 
    while ( mulligan ) { 
      this->subtree = subtree;
      size = 0;

      va_start( args, t );
      while ( mulligan ) {
  
        if ( capacity <= size ) {

          if ( subtree != NULL ) {
            free(subtree);
          }

          capacity += sizeof(syntax_tree_t*) * (BASE_CAPACITY);
          subtree = (syntax_tree_t **) malloc( capacity );

          if ( this->subtree != NULL ) {
           free(this->subtree);
           this->subtree = NULL;
          }

          if ( subtree == NULL ) {
            mulligan = false;
          }

          break;

        }

        *subtree = va_arg( args, syntax_tree_t * );
        size++;

        if ( *subtree != NULL ) {
          subtree++;
        }
        else {
          mulligan = false;
        }

      }
      va_end( args );
    }

  }


  return this;

}


void ST_delete( syntax_tree_t *node ) {

  syntax_tree_t **subtree;


  if ( node == NULL ) {
    return;
  }

  subtree = node->subtree;

  while ( *subtree != NULL ) {
    ST_delete( *subtree );
    subtree++;
  }

  free( node->subtree );
  free( node );

}


bool ST_isleaf( syntax_tree_t *node ) {

  return *(node->subtree) == NULL;

}

