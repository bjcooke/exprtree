#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "assoc.h"
#include "parse.h"
#include "precedence.h"
#include "syntree.h"
#include "token.h"


/* Non-Terminals */
static syntax_tree_t *_unary_expression(token_t **tokens);
static syntax_tree_t *_binary_expression(token_t **tokens, unsigned int prec);
static syntax_tree_t *_ternary_expression(token_t **tokens);
static syntax_tree_t *_assignment_expression(token_t **tokens);
static syntax_tree_t *_compound_expression(token_t **tokens);


static syntax_tree_t *_unary_expression( token_t **tokens ) {

  syntax_tree_t *tree, *branch;
  operator_token_t *op;


  /* rewrite minus operator to be unary */
  if ( (*tokens)->token.id == OPERATOR_TOKEN &&
       (*tokens)->token.size == 1 &&
       (*tokens)->token.location[0] == '-' ) {
    (*tokens)->operator.arity = 1;
    (*tokens)->operator.prec = NEG_PREC;
    (*tokens)->operator.asv = RIGHT_ASSOC;
  }
  /* rewrite plus operator to be unary */
  else if ( (*tokens)->token.id == OPERATOR_TOKEN &&
       (*tokens)->token.size == 1 &&
       (*tokens)->token.location[0] == '+' ) {
    (*tokens)->operator.arity = 1;
    (*tokens)->operator.prec = POS_PREC;
    (*tokens)->operator.asv = RIGHT_ASSOC;
  }
  /* rewrite (de)reference operator to be unary */
  else if ( (*tokens)->token.id == OPERATOR_TOKEN &&
            (*tokens)->token.size == 1 &&
            ( (*tokens)->token.location[0] == '&' ||
              (*tokens)->token.location[0] == '*' ) ) {
    (*tokens)->operator.arity = 1;
    (*tokens)->operator.prec = REF_PREC;
    (*tokens)->operator.asv = RIGHT_ASSOC;
  }


  if ( (*tokens)->token.id == OPERATOR_TOKEN &&
       (*tokens)->operator.arity == 1 ) {

    op = (operator_token_t *) (*tokens)++;
    branch = _binary_expression( tokens, op->prec );

    if ( branch == NULL ) {
      fprintf( stderr, "Error: Operand not found\n" );
      parse_throw( (const token_t *) *tokens );
    }

    /* rewrite postfix operator as prefix */
    op->post = false;

    tree = ST_mknode( (token_t *) op, branch );

  }
  else if ( (*tokens)->token.id == LPAREN_TOKEN ) {

    (*tokens)++;
    tree = _compound_expression( tokens );

    expect( (const token_t **) tokens, RPAREN_TOKEN );

  }
  else if ( (*tokens)->token.id == NUMBER_TOKEN ||
            (*tokens)->token.id == IDENT_TOKEN ) {

    tree = ST_mknode( *tokens );
    (*tokens)++;

  }
  else {
    tree = NULL;
  }


  return tree;

}


static syntax_tree_t *_binary_expression( token_t **tokens, unsigned int prec ){

  syntax_tree_t *lbranch, *rbranch;
  operator_token_t *op;
  token_t *square_bracket, *accessor, *member;


  lbranch = _unary_expression(tokens);

  if ( lbranch == NULL ) {
    return NULL;
  }


  /* Handle postfix operators */
  if ( (*tokens)->token.id == OPERATOR_TOKEN &&
       (*tokens)->operator.arity == 1 &&
       (*tokens)->operator.post == true ) {

    op = (operator_token_t *) (*tokens)++;
    lbranch = ST_mknode( (token_t *) op, lbranch );

  }
  /* Handle array subscript */
  else if ( (*tokens)->token.id == LSQUARE_TOKEN ) {
    square_bracket = (*tokens)++;
    lbranch = ST_mknode( square_bracket, lbranch, _compound_expression(tokens) );

    if ( lbranch->subtree[1] == NULL ) {
      fprintf( stderr, "Error: Empty array subscript\n" );
      parse_throw( (const token_t *) *tokens );
    }

    expect( (const token_t **) tokens, RSQUARE_TOKEN );
  }
  /* Handle accessors */
  else if ( (*tokens)->token.id == ARROW_TOKEN ||
            (*tokens)->token.id == DOT_TOKEN ) {

    accessor = (*tokens)++;

    member = *tokens;
    expect( (const token_t **) tokens, IDENT_TOKEN );

    lbranch = ST_mknode( accessor, lbranch, ST_mknode( member ) );

  }


  while ( (*tokens)->token.id == OPERATOR_TOKEN && 
          (*tokens)->operator.arity == 2 && 
          (*tokens)->operator.prec >= prec &&
          (*tokens)->operator.asv != NON_ASSOC ) {

    op = (operator_token_t *) (*tokens)++;

    rbranch = _binary_expression( tokens, op->asv == RIGHT_ASSOC ?
                                          op->prec : op->prec + 1 );

    if ( rbranch == NULL ) {
      fprintf( stderr, "Error: Operand not found\n" );
      ST_delete( lbranch );
      parse_throw( (const token_t *) *tokens );
    }

    lbranch = ST_mknode( (token_t *) op, lbranch, rbranch );

  }


  return lbranch;

}


static syntax_tree_t *_ternary_expression( token_t **tokens ) {

  syntax_tree_t *tree, *branch[3];
  operator_token_t *op;


  branch[0] = _binary_expression( tokens, TERN_PREC );

  if ( branch[0] == NULL ) {
    return NULL;
  }


  if ( (*tokens)->token.id == OPERATOR_TOKEN ) {
    op = (operator_token_t *) *(tokens);
  }
  else {
    return branch[0];
  }


  if ( op->arity == 3 && op->asv == RIGHT_ASSOC ) {

    (*tokens)++;

    branch[1] = _assignment_expression( tokens );

    if ( branch[1] == NULL ) {
      fprintf( stderr, "Error: Expression not found\n" );
      parse_throw( (const token_t *) *tokens );
    }

    expect( (const token_t **) tokens, COLON_TOKEN );

    branch[2] = _ternary_expression( tokens );

    if ( branch[2] == NULL ) {
      fprintf( stderr, "Error: Expression not found\n" );
      parse_throw( (const token_t *) *tokens );
    }


    tree = ST_mknode( (token_t *) op, branch[0], branch[1], branch[2] );

  }
  else {

    tree = branch[0];

  }


  return tree;

}


static syntax_tree_t *_assignment_expression( token_t **tokens ) {

  syntax_tree_t *tree, *lbranch, *rbranch;
  operator_token_t *op;

  tree = NULL;


  /* rvalues are allowed on lhs during syntax analysis */
  lbranch = _ternary_expression( tokens );

  if ( lbranch == NULL ) {
    return NULL;
  }


  if ( (*tokens)->token.id == OPERATOR_TOKEN ) {
    op = (operator_token_t *) (*tokens);
  }
  else {
    return lbranch;
  }


  if ( op->prec == ASSIGN_PREC && op->arity == 2 ) {

    (*tokens)++;
    rbranch = _ternary_expression( tokens );

    if ( rbranch == NULL ) {
      fprintf( stderr, "Error: rvalue not found\n" );
      ST_delete( lbranch );
      parse_throw( (const token_t *) *tokens );
    }


    tree = ST_mknode( (token_t *) op, lbranch, rbranch );

  }
  else {

    fprintf( stderr, "Error: Unexpected operator " );
    fprint_token( stderr, (const token_t *) op );
    fputc( '\n', stderr );

    ST_delete( lbranch );
    parse_throw( (const token_t *) *tokens );

  }


  return tree;

}


static syntax_tree_t *_compound_expression( token_t **tokens ) {

  syntax_tree_t *tree;
  token_t *comma;

  tree = _assignment_expression( tokens );


  if ( tree != NULL ) {
    while ( (*tokens)->token.id == COMMA_TOKEN ) {
      comma = (*tokens)++;
      tree = ST_mknode( comma, tree, _assignment_expression(tokens) );
    }
  }


  return tree;

}


syntax_tree_t *expression( token_t **tokens ) {

  syntax_tree_t *tree;


  tree = _compound_expression(tokens);


  return tree;

}
