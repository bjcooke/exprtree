#include <stdbool.h>
#include <ctype.h>

static bool _fp_flag;


static int _isbdigit( int c ) {

  return ( ( c == '0' ) || ( c == '1' ) );

}

static int _isodigit( int c ) {

  return ( ( '0' <= c ) && ( c <= '7' ) );

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
