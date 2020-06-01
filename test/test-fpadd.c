#include <stdio.h>
#include <stdlib.h>

#include <gmp.h>
#include <mpfr.h>

#include "token.h"
#include "scanner.h"


#if (MPFR_VERSION<MPFR_VERSION_NUM(3,0,0))
#define DEFAULT_RND GMP_RNDN
#else
#define DEFAULT_RND MPFR_RNDN
#endif

#define DEFAULT_PREC 53
#define DEFAULT_OBASE 10


int main( int argc, char** argv ) {

  token_t *tokens;
  mpfr_t acc, opnd, frac;
  unsigned int nullcount, tokencount, charwidth;
  double d;
  char *it;


  if ( argc < 2 ) {
    fprintf( stderr, "Usage: %s [string]\n", argv[0] );
	return 1;
  }


  for ( (it = argv[1]) && (nullcount = 1); nullcount != argc; it++ ) {
    if ( *it == '\0' && ++nullcount != argc ) {
      *it = ' ';
    }
  }

  mpfr_init2( acc, DEFAULT_PREC );
  mpfr_set_ui( acc, 0, DEFAULT_RND );
  mpfr_init2( opnd, DEFAULT_PREC);
  mpfr_init2( frac, DEFAULT_PREC );

  tokens = scan_tokens( argv[1], &tokencount );

  if ( tokens == NULL ) {
    return 1;
  }


  for (;tokencount != 0 ;tokencount--) {

    if ( token_to_mpf( tokens+tokencount-1, opnd ) == -1 ) {
      mpfr_clear( acc );
      mpfr_clear( opnd );
      mpfr_clear( frac );
      free( tokens );
      return 1;
    }

    mpfr_add( acc, acc, opnd, DEFAULT_RND );

  }


  mpfr_frac( frac, acc, DEFAULT_RND );

  for ( charwidth = 0; !mpfr_integer_p(frac); charwidth++ ) {
      mpfr_mul_ui( frac, frac, DEFAULT_OBASE, DEFAULT_RND );
  }

  if ( charwidth == 0 ) {
    charwidth++;
  }

  d = mpfr_get_d( acc, DEFAULT_RND );
  fprintf( stdout, "%.*f = %lx\n", charwidth, d , *((unsigned long *) &d) );

  mpfr_clear( acc );
  mpfr_clear( opnd );
  mpfr_clear( frac );
  free( tokens );
  return 0;

}
