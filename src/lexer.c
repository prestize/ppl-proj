#include "include/lexer.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

lexer_T *init_lexer(char *contents)
{
    lexer_T *lexer = calloc(1, sizeof(struct LEXER_STRUCT));
    lexer->contents = contents;
    lexer->i = 0;
    lexer->c = contents[lexer->i];

    return lexer;
}

void lexer_advance(lexer_T *lexer)
{
    if (lexer->c != '\0' && lexer->i < strlen(lexer->contents))
    {
        lexer->i += 1;
        lexer->c = lexer->contents[lexer->i];
    }
}

void lexer_skip_whitespace(lexer_T *lexer)
{
    while (lexer->c == ' ' || lexer->c == 10)
    {
        lexer_advance(lexer);
    }
}

token_T *lexer_get_next_token(lexer_T *lexer)
{
    while (lexer->c != '\0' && lexer->i < strlen(lexer->contents))
    {
        if (lexer->c == ' ' || lexer->c == 10)
            lexer_skip_whitespace(lexer);

        if (isalnum(lexer->c))
            return lexer_collect_id(lexer);

        if (lexer->c == '"')
            return lexer_collect_string(lexer);

        if (lexer->c == '#')
            return lexer_collect_comment(lexer);

        switch (lexer->c)
        {
        case '=':
            return lexer_advance_with_token(lexer, init_token(TOKEN_EQUALS, lexer_get_current_char_as_string(lexer)));
            break;
        case '(':
            return lexer_advance_with_token(lexer, init_token(TOKEN_LPAREN, lexer_get_current_char_as_string(lexer)));
            break;
        case ')':
            return lexer_advance_with_token(lexer, init_token(TOKEN_RPAREN, lexer_get_current_char_as_string(lexer)));
            break;
        case '+':
            return lexer_advance_with_token(lexer, init_token(TOKEN_OPERATOR, lexer_get_current_char_as_string(lexer)));
            break;
        case '-':
            return lexer_advance_with_token(lexer, init_token(TOKEN_OPERATOR, lexer_get_current_char_as_string(lexer)));
            break;
        case '/':
            return lexer_advance_with_token(lexer, init_token(TOKEN_OPERATOR, lexer_get_current_char_as_string(lexer)));
            break;
        case '*':
            return lexer_advance_with_token(lexer, init_token(TOKEN_OPERATOR, lexer_get_current_char_as_string(lexer)));
            break;
        case '%':
            return lexer_advance_with_token(lexer, init_token(TOKEN_OPERATOR, lexer_get_current_char_as_string(lexer)));
            break;
        }
    }

    return (void *)0;
}

token_T *lexer_collect_string(lexer_T *lexer)
{
    lexer_advance(lexer);

    char *value = calloc(1, sizeof(char));
    value[0] = '\0';

    while (lexer->c != '"')
    {
        char *s = lexer_get_current_char_as_string(lexer);
        value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
        strcat(value, s);

        lexer_advance(lexer);
    }

    lexer_advance(lexer);

    return init_token(TOKEN_WORD, value);
}

//this is for identifying comments
token_T *lexer_collect_comment(lexer_T *lexer)
{
   
    char *value = calloc(1, sizeof(char));
    value[0] = '\0';
 
    //saves the first '#'
    char *s = lexer_get_current_char_as_string(lexer);
    value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
    strcat(value, s);

    lexer_advance(lexer);
    //if next character is not #, it is a single line comment
    if(lexer->c!='#')
    {   
        while (lexer->c != '\n' && lexer->c !='\0') 
        {
            s = lexer_get_current_char_as_string(lexer);
            value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
            strcat(value, s);

            lexer_advance(lexer);
        } 

        return init_token(TOKEN_SLCOMMENT, value);   
    }
    
    //this is for multi-line comment
    else
    {   
        int found =0; // to know if we have closing ## 

        //s holds the current character and nextcharacter holds the next character
        char *nextcharacter;
        s = lexer_get_current_char_as_string(lexer); //saves the second #
            value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
            strcat(value, s);

        lexer_advance(lexer);
        while (lexer->c!='\0')
        {
            s = lexer_get_current_char_as_string(lexer);
            lexer_advance(lexer);
            nextcharacter = lexer_get_current_char_as_string(lexer);
            if((*s=='#') && (*nextcharacter=='#')) 
            {   
               
                found=1;
                value = realloc(value, (strlen(value) + strlen(s) + 2) * sizeof(char));
                strcat(value, s);
                strcat(value, nextcharacter);
                lexer_advance(lexer);
                break;
            }
            else
            {   
                value = realloc(value, (strlen(value) + strlen(s) + 2) * sizeof(char));
                strcat(value, s);
                strcat(value, nextcharacter);
                lexer_advance(lexer);
            }  
        }

        if (found==1)
        {
            return init_token(TOKEN_MLCOMMENT, value);
        }

        else{
            return init_token(TOKEN_INVALID, value);
        }
    }
}

token_T *lexer_collect_id(lexer_T *lexer)
{
    char *value = calloc(1, sizeof(char));
    value[0] = '\0';
    int count = 0;

    while (isalnum(lexer->c))
    {
        char *s = lexer_get_current_char_as_string(lexer);
        value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
        strcat(value, s);

        lexer_advance(lexer);
    }

    if (identify_if_keyword(value) == 1)
        return init_token(TOKEN_KEYWORD, value);

    if (count <= 30)
        return init_token(TOKEN_ID, value);
}

int identify_if_keyword(char string[])
{

    int isKeyword(char buffer[])
    {
        char keywords[20][10] = {
            "powerOf",
            "if",
            "else",
            "then",
            "elseif",
            "foreach",
            "in",
            "do",
            "match",
            "return",
            "continue",
            "true",
            "false",
            "list",
            "while",
            "display",
            "input",
            "case",
            "end",
            "noreturn"};

        int i, flag = 0;
        for (i = 0; i < 20; ++i)
        {
            if (strcmp(keywords[i], buffer) == 0)
            {
                flag = 1;
                break;
            }
        }
        return flag;
    }

    int isOperator(char buffer[])
    {
        char operators[4][10] = {"and",
                                 "powerOf",
                                 "or",
                                 "not"};

        int i, flag = 0;
        for (i = 0; i < 20; ++i)
        {
            if (strcmp(operators[i], buffer) == 0)
            {
                flag = 1;
                break;
            }
        }
        return flag;
    }

    int isReservedWord(char buffer[])
    {
        char reservedWords[20][10] = {
            "num",
            "decimal",
            "word",
            "letter",
            "boolean"};

        int i, flag = 0;
        for (i = 0; i < 20; ++i)
        {
            if (strcmp(reservedWords[i], buffer) == 0)
            {
                flag = 1;
                break;
            }
        }
        return flag;
    }

    if (isKeyword(string) == 1)
    {
        return 1;
    }
    else if (isOperator(string) == 1)
    {
        return 1;
    }
    else if (isReservedWord(string) == 1)
    {
        return 1;
    }
}

token_T *lexer_advance_with_token(lexer_T *lexer, token_T *token)
{
    lexer_advance(lexer);

    return token;
}

char *lexer_get_current_char_as_string(lexer_T *lexer)
{
    char *str = calloc(2, sizeof(char));
    str[0] = lexer->c;
    str[1] = '\0';

    return str;
}