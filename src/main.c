#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <term.h>
#include "assoc.h"
#include "parse.h"
#include "scanner.h"
#include "syntree.h"
#include "token.h"

#define TBUFSIZE 2048

int TERM_WIDTH;
int TERM_HEIGHT;


static void plot_tree( syntax_tree_t *node,
                char ** const scrbuf,
                int width,
                int x_offset,
                int y_offset ) {

  syntax_tree_t **tpit;
  int left_offset, ncount;
  char *token_string;


  sprint_token( &token_string, node->t );
  left_offset = x_offset - strlen(token_string)/2;


  if ( TERM_WIDTH <= (strlen(token_string) + left_offset + 1) ||
       x_offset < 0 ||
       TERM_HEIGHT <= y_offset ||
       left_offset < 0 ) {
    free( token_string );
    return;
  }

  memset( scrbuf[y_offset], ' ', sizeof(char) * left_offset );
  strcpy( scrbuf[y_offset] + left_offset, token_string );
  scrbuf[y_offset][left_offset - 1] = '(';
  *(scrbuf[y_offset++] + strlen(token_string) + left_offset ) = ')';


  ncount = 0;
  for ( tpit = node->subtree; *tpit != NULL; tpit++ ) {
    ncount++;
  }


  if ( ncount == 1 ) {


    if ( TERM_WIDTH <= (x_offset + 1) || TERM_HEIGHT <= y_offset ) {
      free( token_string );
      return;
    }
  
    memset( scrbuf[y_offset], ' ', sizeof(char) * x_offset );
    scrbuf[y_offset++][x_offset] = '|';

    plot_tree( node->subtree[0], scrbuf, width, x_offset, y_offset++ );

  }
  else if ( ncount == 2 || ncount == 3 ) {

    if ( TERM_WIDTH <= (x_offset + 1) || TERM_HEIGHT <= y_offset ) {
      free( token_string );
      return;
    }


    if ( width >= 8 ) {

      width = width/2;
      width & 1 ? width++: 0;

    }
    else {

      if ( *scrbuf[y_offset] != '\0' ) {
        width *= 2;
      }

      while ( *scrbuf[y_offset] != '\0' ) {

        if ( scrbuf[y_offset][x_offset] == ' ' ) {
          scrbuf[y_offset][x_offset] = '|';
        }

        if ( TERM_HEIGHT <= ++y_offset ) {
          free( token_string );
          return;
        }
      }

    }
 
 
    memset( scrbuf[y_offset], ' ', sizeof(char) * x_offset );
    scrbuf[y_offset++][x_offset] = '|';


    if ( TERM_WIDTH <= (x_offset + x_offset/4 + 1) ||
         x_offset - width/2 < 0 ||
         TERM_HEIGHT <= y_offset ) {
      free( token_string );
      return;
    }

    memset( scrbuf[y_offset], ' ', sizeof(char) * (x_offset - width/2) );
    memset( scrbuf[y_offset++] + x_offset - width/2, '-', sizeof(char) * width );


    if ( TERM_HEIGHT <= y_offset ) {
      free( token_string );
      return;
    }

    memset( scrbuf[y_offset], ' ', sizeof(char) * (x_offset + width/2 - 1) );

    scrbuf[y_offset][x_offset - width/2] = '|';

    if ( ncount == 3 ) {
      scrbuf[y_offset][x_offset] = '|';
    }

    scrbuf[y_offset++][x_offset + width/2 - 1] = '|';


    if ( ncount == 3 ) {
      plot_tree( node->subtree[2], scrbuf, width, x_offset + width/2 - 1, y_offset );
      plot_tree( node->subtree[1], scrbuf, width, x_offset, y_offset );
    }
    else {  
      plot_tree( node->subtree[1], scrbuf, width, x_offset + width/2 - 1, y_offset );
    }

    plot_tree( node->subtree[0], scrbuf, width, x_offset - width/2, y_offset );

  }


  free( token_string );

}


static void print_paren( syntax_tree_t *node ) {

  int i;


  if ( node == NULL ) {
    return;
  }


  if ( ST_isleaf(node) ) {

    print_token( node->t );

  }
  else if ( node->t->token.id == OPERATOR_TOKEN ) {

    putchar('(');

    if ( node->t->operator.arity == 1  && node->t->operator.post == false ) {
      print_token(node->t);
    }

    for ( i = 0; i < node->t->operator.arity; i++ ) {

      if ( i == 1 ) {
        print_token(node->t);
      }
      else if ( i == 2 ) {
        putchar(':');
      }

      print_paren(node->subtree[i]);

    }

    if ( node->t->operator.arity == 1  && node->t->operator.post == true ) {
      print_token(node->t);
    }

    putchar(')');

  }

}


static int evalopts( int argc, char * const argv[], char **e, char **fn, bool *visual ) {

  struct option optv[3];
  char *expopt_name = "expression";
  char *fileopt_name = "file";
	char *nvopt_name = "paren";
  char *optstring = "+e:f:p";
  char *usage_fmt = "Usage: %s [-p|--paren] [[-e|--expression] exp] [[-f|--file] file]\n";
  int ch;


  optv[0].name = expopt_name;
  optv[0].has_arg = 1;
  optv[0].flag = NULL;
  optv[0].val = 'e';

  optv[1].name = fileopt_name;
  optv[1].has_arg = 1;
  optv[1].flag = NULL;
  optv[1].val = 'f';

	optv[2].name = nvopt_name;
	optv[2].has_arg = 0;
	optv[2].flag = NULL;
	optv[2].val = 'p';


	*visual = true;

  while ( (ch = getopt_long( argc, argv, optstring, optv, NULL )) > 0 ) {

    switch ( ch ) {
      case 'e':
        *e = malloc( sizeof(char) * (strlen(optarg) + 1) );
        memcpy( *e, optarg, sizeof(char) * (strlen(optarg) + 1) );
        break;

      case 'f':
        *fn = optarg;
        break;

			case 'p':
				*visual = false;
				break;

      case '?':
        fprintf(stderr, usage_fmt, argv[0]);
        return 1;

      default:
        break;
    }

  }


  if ( argc > optind ) {
    fprintf( stderr, usage_fmt, argv[0] );
    return 1;
  }


  return 0;

}


int main( int argc, char **argv ) {

  syntax_tree_t *tree;
  FILE *input_stream;
  char *buf, *filename, *term_name, term_data[TBUFSIZE], **screen_buffer, *ch;
	bool visual;
  int i;


  buf = NULL;
  filename = NULL;

  if ( evalopts( argc, argv, &buf, &filename, &visual ) != 0 ) {
    return 1;
  }

  if ( filename == NULL ) {
    input_stream = stdin;
  }
  else {
    if ( (input_stream=fopen( filename, "r" )) == NULL ) {
      perror("Error opening file");
      return 1;
    }
  }


  if ( buf == NULL ) {
    buf = lazy_read( input_stream );
    fclose( input_stream );
  }

  for ( ch = buf; *ch != ';' && *ch != '\0'; ch++ );
  *ch = '\0';

  tree = parse_start( buf );

  if ( tree == NULL ) {
    return 1;
  }


  term_name = getenv("TERM");

  if ( term_name == NULL ||
       tgetent( term_data, term_name ) != 1 ||
       ( TERM_WIDTH = tgetnum("co") ) == -1 ||
       ( TERM_HEIGHT = tgetnum("li") ) == -1 ) {
    return 1;
  }


	if ( visual == true ) {
	  screen_buffer = (char **) malloc( sizeof(char *) * TERM_HEIGHT );
	
	  for ( i = 0; i < TERM_HEIGHT; i++ ) {
	    screen_buffer[i] = (char *) malloc( sizeof(char) * TERM_WIDTH );
	    screen_buffer[i][0] = '\0';
	    memset( screen_buffer[i] + 1, 0x3, sizeof(char) * (TERM_WIDTH - 1) );
	  }
	
	
	  plot_tree(tree, screen_buffer, TERM_WIDTH, TERM_WIDTH/2, 0);
	
	  putchar('\n');
	
	  for ( i = 0; i < TERM_HEIGHT; i++ ) {
	    for ( ch = screen_buffer[i] + TERM_WIDTH; *(ch - 1) == 0x3; ch-- );
	    ch[0] = '\n';
	    ch[1] = '\0';
	    printf("%s", screen_buffer[i]);
	  }
	
	  putchar('\n');
	
	
	  for ( i = 0; i < TERM_HEIGHT; i++ ) {
	    free( screen_buffer[i] );
	    screen_buffer[i] = NULL;
	  }
	
	  free( screen_buffer );
	  screen_buffer = NULL;
	}
	else {
		print_paren( tree );
	}


  parse_clear();
  tree = NULL;
  free(buf);
  buf = NULL;


  return 0;

}
