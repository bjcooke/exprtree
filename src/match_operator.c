#include <stdbool.h>
#include "assoc.h"
#include "precedence.h"

#define COMPOUND_ASSIGN (-1)


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
