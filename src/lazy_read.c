#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BASE_CAPACITY 4096
#define MEM_OFFSET 32


char *lazy_read( FILE *stream ) {

  char *buf, *tmp;
  size_t capacity, memdiff, bcount;


  capacity = BASE_CAPACITY - MEM_OFFSET;
  memdiff = capacity;
  buf = (char *) malloc( sizeof(char) * capacity );
  tmp = NULL;

  if ( buf == NULL ) {
    return NULL;
  }

  while ( (bcount=fread( buf, memdiff , sizeof(char), stream )) == memdiff ) {

    tmp = buf;

    capacity += BASE_CAPACITY;
    memdiff = BASE_CAPACITY;

    buf = (char *) malloc( sizeof(char) * capacity );

    memcpy( buf, tmp, sizeof(char) * (capacity - memdiff) );

    free(tmp);
    tmp = NULL;

  }


  if ( ferror( stream ) ) {
    perror( "Error while reading input" );
    free( buf);
    buf = NULL;
  }
  else if ( bcount == 0 ) {

    tmp = buf;

    buf = (char *) malloc( sizeof(char) * (capacity + BASE_CAPACITY) );

    memcpy( buf, tmp, sizeof(char) * capacity );

    buf[ capacity + 1 ] = '\0';

  }
  else {

    buf[ (capacity - memdiff) + bcount ] = '\0';

  }


  return buf;

}
