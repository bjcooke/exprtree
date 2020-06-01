#include <ctype.h>
#include <stdbool.h>

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
                return (-1);

        }
    }

    return i;
}
