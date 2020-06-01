#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"


size_t fprint_token( FILE *stream, const token_t *t ) {

  return fwrite( t->token.location, sizeof(char), t->token.size, stream );

}

size_t sprint_token( char **buffer, const token_t *t ) {

  *buffer = (char *) malloc( sizeof(char *) * (t->token.size + 1) );

  if ( *buffer == NULL ) {
    return 0;
  }

  strncpy( *buffer, t->token.location, sizeof(char *) * t->token.size );
  *(*buffer + t->token.size) = '\0';

  return (t->token.size + 1);

}

size_t print_token( const token_t *t ) {

  return fprint_token( stdout, t );

}
