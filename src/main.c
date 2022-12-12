#include <stdio.h>
#include <string.h>
#include "include/lexer.h"
#include "include/token.h"
#include "include/io.h"

int main(int argc, char *argv[])
{
    lexer_T *lexer = init_lexer(get_file_contents(argv[1]));

    token_T *token = (void *)0;

    while ((token = lexer_get_next_token(lexer)) != (void *)0)
    {
        char tokenName[20];
        switch (token->type)
        {
        case 0:
            strcpy(tokenName, "TOKEN_IDENTIFIER");
            break;
        case 1:
            strcpy(tokenName, "TOKEN_EQUALS");
            break;
        case 2:
            strcpy(tokenName, "TOKEN_WORD");
            break;
        case 3:
            strcpy(tokenName, "TOKEN_LPAREN");
            break;
        case 4:
            strcpy(tokenName, "TOKEN_RPAREN");
            break;
        case 5:
            strcpy(tokenName, "TOKEN_KEYWORD");
            break;
        case 6:
            strcpy(tokenName, "TOKEN_OPERATOR");
            break;
        case 7:
            strcpy(tokenName, "TOKEN_SLCOMMENT");
            break;
        case 8:
            strcpy(tokenName, "TOKEN_MLCOMMENT");
            break;
        case 9:
            strcpy(tokenName, "TOKEN_INVALID");
            break;
        case 10:
            strcpy(tokenName, "TOKEN_NUM");
            break;
        case 11:
            strcpy(tokenName, "TOKEN_DECIMAL");
            break;
        }
        printf("%s = %s\n", token->value, tokenName);
    }

    return 0;
}