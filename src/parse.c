#include <stdio.h>
#include <stdlib.h>

#include "scanner.h"
#include "syntree.h"
#include "token.h"
#include "parse.h"


static struct {
  syntax_tree_t *head_node;
  token_t *head_token;
} parse_tree_head;


void parse_throw( const token_t *t ) {

  unsigned int linecount, charcount;
  const char *c;


  if ( t != NULL ) {

    linecount = 0;
    charcount = 0;
    c = parse_tree_head.head_token->token.location;

    while ( c != t->token.location ) {
      if ( *c++ == '\n' ) {
        linecount++;
        charcount = 0;
      }
      else {
        charcount++;
      }
    }

    fprintf( stderr,
             "Error found at line %u position %u\n", linecount, charcount );

  }


  parse_clear();

  exit(EXIT_FAILURE);

}


void parse_clear( void ) {

  ST_delete( parse_tree_head.head_node );
  free( parse_tree_head.head_token );

}


syntax_tree_t *parse_start( const char *s ) {

  token_t *tokens;
  unsigned int tokencount;


  tokens = scan_tokens( s, &tokencount );

  if ( tokens == NULL ) {
    return NULL;
  }

  parse_tree_head.head_token = tokens;

  parse_tree_head.head_node = expression( &tokens );

  if ( tokens->token.id != _EOF_TOKEN_ ) {
    expect( (const token_t **) &tokens, SEMICOLON_TOKEN );
  }

  return parse_tree_head.head_node;

}
