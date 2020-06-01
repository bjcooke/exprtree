#include <stdio.h>
#include "parse.h"
#include "token.h"


void expect( const token_t **tokens, tokenid_t id ) {

  token_t dummy;

  dummy.token.id = id;

  if ( (*tokens)->token.id != id ) {
    fprintf( stderr, "Unexpected token " );
    fprint_token( stderr, *tokens );
    fprintf( stderr, " of type " );
    fprint_token_id( stderr, *tokens );
    fprintf( stderr, "; Expected " );
    fprint_token_id( stderr, &dummy );
    fprintf( stderr, " token.\n" );

    parse_throw( *tokens );
  }
  else {
    (*tokens)++;
  }

}
