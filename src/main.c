#include <stdio.h>
#include <string.h>
#include "include/lexer.h"
#include "include/token.h"

int main(int argc, char *argv[])
{
    lexer_T *lexer = init_lexer(
       "32=3b.a 2.(3.s23thisisinvalid*0.233 100 10a+23. 123b.3\n"
        "word name1 = \"john\"\n"
        "word x name2 = \"mark\"\n"
        "display(name1)\n"
        "display(name2)\n"
        "num d = 323.2 + 5+\n"
        "##this is a MULTI\nLINE COMMENT##\n"
        "#this is a single line comment\n"
        "##this is an invalid comment");

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