#include <ctype.h>
#include <stdbool.h>
#include "assoc.h"
#include "precedence.h"

#define COMPOUND_ASSIGN (-1)

static bool _fp_flag;


static int _isbdigit( int c ) {

  return ( ( c == '0' ) || ( c == '1' ) );

}

static int _isodigit( int c ) {

  return ( ( '0' <= c ) && ( c <= '7' ) );

}


unsigned int match_arrow( const char *s ) {

  if ( s[0] == '-' && s[1] == '>' ) {
    return 2;
  }
  else {
    return 0;
  }

}

unsigned int match_colon( const char *s ) {

  if ( *s == ':' ) {
    return 1;
  }
  else {
    return 0;
  }

}

unsigned int match_comma( const char *s ) {

  if ( *s == ',' ) {
    return 1;
  }
  else {
    return 0;
  }

}

unsigned int match_dot( const char *s ) {

  if ( (*s) == '.' ) {
    return 1;
  }
  else {
    return 0;
  }

}

unsigned int match_lparen( const char *s ) {

  if ( *s == '(' ) {
    return 1;
  }
  else {
    return 0;
  }

}

unsigned int match_lsquare( const char *s ) {

  if ( *s == '[' ) {
    return 1;
  }
  else {
    return 0;
  }

}

unsigned int match_rparen( const char *s ) {

  if ( *s == ')' ) {
    return 1;
  }
  else {
    return 0;
  }

}

unsigned int match_rsquare( const char *s ) {

  if ( *s == ']' ) {
    return 1;
  }
  else {
    return 0;
  }

}


unsigned int match_ident( const char *s )
{
    int state, i;
    bool accept;

    state = 0;
    accept = true;
    i = 0;

    while ( accept == true ) {
        switch (state) {

            case 0:
                state = (isalpha(s[i]) || s[i] == '_') ? 1 : 2;
                break;

            case 1:
                accept = true;
                i++;
                state = (isalnum(s[i]) || s[i] == '_') ? 1 : 2;
                break;

            case 2:
                accept = false;
                break;

            /* Unknown State */
            default:
                return 0;

        }
    }

    return i;
}


static unsigned int _match_base_number( const char *s, int (*isbaseD)(int) ) {

  unsigned int count = 0; 
  int state = 1;
  _fp_flag = false;


  while ( state != 0 ) {

    switch( state ) {
      case 1:
  
        if ( isbaseD(*s) ) {
          count++;
          state = 2;
        }
        else {
          /* reject */
          count = 0;
          state = 0;
        }
        break;
  
  
      case 2:
  
        if ( isbaseD(*s) ) {
          count++;
        }
        else if ( *s == '.' ) {
          _fp_flag = true;
          count++;
          state = 3;
        }
        else {
          /* accept */
          state = 0;
        }
        break;
  
  
      case 3:
  
        if ( isbaseD(*s) ) {
          count++;
          state = 4;
        }
        else {
          /* Reject */
          _fp_flag = false;
          count = 0;
          state = 0;
        }
        break;


      case 4:

        if ( isbaseD(*s) ) {
          count++;
        }
        else {
          /* Accept */
          state = 0;
        }
        break;


    }


    s++;

  }


  return count;

}


unsigned int match_number( const char *s,
                           unsigned int *base,
                           bool *fp ) {

  unsigned int count;


  if ( s[0] == '0' && s[1] == 'x' ) {

    count = _match_base_number( s + 2, isxdigit );

    if ( count > 0 ) { 
      *base = 16;
      count += 2;
    }

  }
  else if ( s[0] == '0' && s[1] == 'b' ) {

    count = _match_base_number( s + 2, _isbdigit );

    if ( count > 0 ) {
      *base = 2;
      count += 2;
    }

  }
  else if ( s[0] == '0' && s[1] == 'o' ) {

    count = _match_base_number( s + 2, _isodigit );

    if ( count > 0 ) {
      *base = 8;
      count += 2;
    }

  }
  else {

    count = _match_base_number( s, isdigit );

    if ( count > 0 ) {
      *base = 10;
    }

  }


  *fp = _fp_flag;
  _fp_flag = false;


  return count;

}


unsigned int match_operator( char *s,
                             unsigned int *arity,
                             int *prec,
                             assoc_t *asv,
                             bool *post ) {

  unsigned int count = 0;
  int state = 1;


  /* Most operators are prefix, and I'm lazy */
  *post = false;


  while ( state != 0 ) {

    switch (state) {

      case 1:

        if ( *s == '=' ) {
          count++;
          state = 2;
        }
        else if ( *s == '!' ) {
          count++;
          state = 3;
        }
        else if ( *s == '<' || *s == '>' ) {
          count++;
          state = 4;
        }
        else if ( *s == '&' ) {
          count++;
          state = 5;
        }
        else if ( *s == '|' ) {
          count++;
           state = 6;
        }
        else if ( *s == '+' || *s == '-' ) {
          count++;
          state = 7;
        }
        else if ( *s == '^' ) {
          *arity = 2;
          *prec = BXOR_PREC;
          *asv = LEFT_ASSOC;

          count++;
          state = COMPOUND_ASSIGN;
        }
        else if ( *s == '~' ) {
          *arity = 1;
          *prec = BNOT_PREC;
          *asv = RIGHT_ASSOC;

          count++;
          state = COMPOUND_ASSIGN;
        }
        else if ( *s == '*' ||  *s == '/' || *s == '%' ) {
          *arity = 2;
          *prec = PROD_PREC;
          *asv = LEFT_ASSOC;

          /* accept */
          count++;
          state = COMPOUND_ASSIGN;
        }
        else if ( *s == '?' ) {
          *arity = 3;
          *prec = TERN_PREC;
          *asv = RIGHT_ASSOC;

          /* accept */
          count++;
          state = 0;
        }
        else {
          /* reject */
          count = 0;
          state = 0;
        }
        break;


      case 2:

        if ( *s == '=' ) {
          *arity = 2;
          *prec = EQ_PREC;
          *asv = RIGHT_ASSOC;

          /* accept */
          count++;
          state = 0;
        }
        else {
          *arity = 2;
          *prec = ASSIGN_PREC;
          *asv = NON_ASSOC;

          /* accept */
          state = 0;
        }
        break;


      case 3:

        if ( *s == '=' ) {
          *arity = 2;
          *prec = EQ_PREC;
          *asv = RIGHT_ASSOC;

          /* accept */
          count++;
          state = 0;
        }
        else {
          *arity = 1;
          *prec = LNOT_PREC;
          *asv = RIGHT_ASSOC;

          /* accept */
          state = 0;
        }
        break;


      case 4:

         if ( *s == '=' ) {
           *arity = 2;
           *prec = REL_PREC;
           *asv = RIGHT_ASSOC;

           /* accept */
           count++;
           state = 0;
         }
         /* Now you fucked up */
         else if ( *(s - 1) == *s ) {
           *arity = 2;
           *prec = SHIFT_PREC;
           *asv = LEFT_ASSOC;

           count++;
           state = COMPOUND_ASSIGN;
         }
         else {
           *arity = 2;
           *prec = REL_PREC;
           *asv = RIGHT_ASSOC;

           /* accept */
           state = 0;
         }
         break;


       case 5:

        if ( *s == '&' ) {
          *arity = 2;
          *prec = LAND_PREC;
          *asv = LEFT_ASSOC;

          /* accept */
          count++;
          state = 0;
        }
        else {
          *arity = 2;
          *prec = BAND_PREC;
          *asv = LEFT_ASSOC;

          s--;
          state = COMPOUND_ASSIGN;
        }
        break;


      case 6:

        if ( *s == '|' ) {
          *arity = 2;
          *prec = LOR_PREC;
          *asv = LEFT_ASSOC;

          /* accept */
          count++;
          state = 0;
        }
        else {
          *arity = 2;
          *prec = BOR_PREC;
          *asv = LEFT_ASSOC;

          s--;
          state = COMPOUND_ASSIGN;
        }
        break;


      case 7:

        /* You have fucked up now */
        if ( *(s - 1) == *s ) {
          *arity = 1;
          *prec = INC_PREC;
          *asv = RIGHT_ASSOC;
          *post = true;

          /* accept */
          count++;
          state = 0;
        }
        else {
          *arity = 2;
          *prec = SUM_PREC;
          *asv = LEFT_ASSOC;

          s--;
          state = COMPOUND_ASSIGN;
        }

        break;


      case COMPOUND_ASSIGN:

        if ( *s == '=' ) {
          *arity = 2;
          *prec = ASSIGN_PREC;
          *asv = NON_ASSOC;

          /* accept */
          count++;
          state = 0;
        }
        else {
          /* accept */
          state = 0;
        }

        break;
      
    }

    s++;

  }


  return count;

}
