#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "scanner.h"


int main( int argc, char **argv ) {

  token_t *tokens;
  unsigned int nullcount, tokencount, i;
  char *it;


  if ( argc < 2 ) {
    fprintf(stderr, "Usage: %s [string]\n", argv[0]);
    return 1;
  }


  it = argv[0];
  nullcount = 0;
  while ( nullcount != argc ) {

    if ( *it == '\0' && ++nullcount != argc ) {
      *it = ' ';
    }

    it++;

  }


  tokens = scan_tokens( argv[1], &tokencount );

  if ( tokens == NULL ) {
    return 1;
  }


  for ( i = 0; i < tokencount; i++ ) {
    print_token_id( tokens + i );
    putchar(' ');
    print_token( tokens + i );
    putchar('\n');
  }


  free( tokens );
  tokens = NULL;

  return 0;

}
