#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BASE_CAPACITY 4096
#define MEM_OFFSET 32


static int resize( char **buf, size_t *cap ) {

  char *tmp;

  tmp = malloc( (*cap+MEM_OFFSET)*2 - MEM_OFFSET );

  if ( tmp == NULL ) {
    return 1;
  }

  memcpy( tmp, *buf, *cap );
  *cap = (*cap+MEM_OFFSET)*2 - MEM_OFFSET;

  free(*buf);
  *buf = tmp;


  return 0;

}


char *lazy_read( FILE *stream ) {

  char *buf;
  size_t cap;
  int c, i;


  cap = BASE_CAPACITY - MEM_OFFSET;
  buf = malloc( cap );

  for ( i = 0; ; i++ ) {

    c = getc( stream );

    if ( i >= cap ) {
      if ( resize( &buf, &cap ) != 0 ) {
        free( buf );
        return NULL;
      }
    }

    if ( c != EOF ) {
      buf[i] = c;
    }
    else {
      buf[i] = '\0';
      break;
    }

  }


  if ( ferror( stream ) ) {
    perror( "Error while reading input" );
    free( buf );
    buf = NULL;
  }

  return buf;

}
