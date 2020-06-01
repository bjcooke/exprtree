#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gmp.h>
#include <mpfr.h>

#include "token.h"

#if (MPFR_VERSION<MPFR_VERSION_NUM(3,0,0))
#define DEFAULT_RND GMP_RNDN
#else
#define DEFAULT_RND MPFR_RNDN
#endif


static int _token_to_mpz( const token_t *token, mpz_t value ) {

  char *tmp;


  if ( token->token.id != NUMBER_TOKEN ) {
    return (-1);
  }
  else if ( token->number.fp == true ) {
    fprintf( stderr, "Error: Float passed to integer subroutine\n" );
    return (-1);
  }


  tmp = (char *) malloc( sizeof(char) * (token->token.size + 1) );

  if ( tmp == NULL ) {
    fprintf( stderr, "Error: Failure during conversion of token to big integer.\n");
    return (-1);
  }

  memcpy( tmp, token->token.location, sizeof(char) * token->token.size );
  *(tmp + token->token.size) = '\0';


  if ( token->number.base == 16 ||
       token->number.base == 8  ||
       token->number.base == 2     )
  {
    tmp += 2;
  }

  if ( mpz_set_str( value, tmp, token->number.base ) != 0 ) {
    fprintf(stderr, "Error: Failed to initialize big integer\n");

    free(tmp);
    tmp = NULL;

    return (-1);
  }


  free(tmp);
  tmp = NULL;

  return 0;

}


int token_to_mpf( const token_t *token, mpfr_t value ) {

  char *tmp;


  if ( token->token.id != NUMBER_TOKEN ) {
    return (-1);
  }
  else if ( token->number.fp == false ) {
    fprintf( stderr, "Error: Integer passed to float subroutine\n" );
    return (-1);
  }


  tmp = (char *) malloc( sizeof(char) * (token->token.size + 1) );

  if ( tmp == NULL ) {
    fprintf( stderr, "Error: Failure during conversion of token to big float\n");
    return (-1);
  }


  memcpy( tmp, token->token.location, sizeof(char) * token->token.size );
  tmp[token->token.size] = '\0';


  if ( token->number.base == 16 ||
       token->number.base == 8  ||
       token->number.base == 2     )
  {
    tmp += 2;
  }

  if ( mpfr_set_str( value, tmp, token->number.base, DEFAULT_RND ) != 0 ) {
    fprintf( stderr, "Error: Failed to initialize big float\n" );

    free(tmp);
    tmp = NULL;

    return (-1);
  }


  free(tmp);
  tmp = NULL;

  return 0;

}


int token_to_i32( const token_t *token, int32_t *value ) {

  mpz_t realval;
  uint32_t uval;
  int overflow;


  mpz_init( realval );

  if ( _token_to_mpz( token, realval ) != 0 ) {
    mpz_clear( realval );
    return (-1);
  }

  if ( (overflow = !mpz_fits_sint_p(realval)) != 0 ) {
    uval = mpz_fdiv_ui( realval, UINT32_MAX );
    *value = *((uint32_t *) &uval);
  }
  else {
    *value = mpz_get_si( realval );
  }

  mpz_clear( realval );

  return overflow;
}


int token_to_i64( const token_t *token, int64_t *value ) {

  mpz_t realval;
  uint64_t uval;
  int overflow;


  mpz_init( realval );

  if ( _token_to_mpz( token, realval ) != 0 ) {
    mpz_clear( realval );
    return (-1);
  }

  if ( (overflow = !mpz_fits_sint_p(realval)) != 0 ) {
    uval = mpz_fdiv_ui( realval, UINT64_MAX );
    *value = *((uint64_t *) &uval);
  }
  else {
    *value = mpz_get_si( realval );
  }

  mpz_clear( realval );

  return overflow;
}
