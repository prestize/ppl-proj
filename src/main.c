#include <stdio.h>
#include "include/lexer.h"
#include "include/token.h"

int main(int argc, char *argv[])
{
    lexer_T *lexer = init_lexer(
        "name1 = \"john\"\n"
        "name2 = \"mark\"\n"
        "display(name1)\n"
        "display(name2)\n");

    token_T *token = (void *)0;

    while ((token = lexer_get_next_token(lexer)) != (void *)0)
    {
        printf("TOKEN(%d, %s)\n", token->type, token->value);
    }

    return 0;
}