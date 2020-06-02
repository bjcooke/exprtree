#ifndef _PARSE_H
#define _PARSE_H

#include "syntree.h"
#include "token.h"


extern syntax_tree_t *parse_start( const char *s );
extern void parse_throw( const token_t *t );
extern void parse_clear( void );
extern void expect( const token_t **tokens, tokenid_t id );

extern syntax_tree_t *expression( token_t **tokens );

#endif
