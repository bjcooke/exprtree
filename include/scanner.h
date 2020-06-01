#ifndef _SCANNER_H
#define _SCANNER_H

#include <stdio.h>
#include "token.h"


extern char *lazy_read( FILE *stream );
extern token_t *scan_tokens( const char *s, unsigned int *n );

#endif
