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

        // delimiters, boolean operators, arithmetic operators (logical operators not yet included in here)
        switch (lexer->c)
        {
        case '=':; //  equals or is equal to

            char *value = calloc(1, sizeof(char));
            value[0] = '\0';

            char *s = lexer_get_current_char_as_string(lexer);
            value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
            strcat(value, s);

            lexer_advance(lexer);

            if (lexer->c == '=')
            {
                s = lexer_get_current_char_as_string(lexer);
                value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
                strcat(value, s);

                return lexer_advance_with_token(lexer, init_token(TOKEN_ISEQUALTO, value));
            }
            return lexer_advance_with_token(lexer, init_token(TOKEN_EQUALS, value));
            break;
        case '>':; // greater than || greater than or equals to
            value = calloc(1, sizeof(char));
            value[0] = '\0';

            s = lexer_get_current_char_as_string(lexer);
            value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
            strcat(value, s);

            lexer_advance(lexer);

            if (lexer->c == '=')
            {
                s = lexer_get_current_char_as_string(lexer);
                value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
                strcat(value, s);

                return lexer_advance_with_token(lexer, init_token(TOKEN_GREATERTHANOREQUALS, value));
            }
            return lexer_advance_with_token(lexer, init_token(TOKEN_GREATERTHAN, value));
            break;
        case '<':; // less thann || less than or equals to
            value = calloc(1, sizeof(char));
            value[0] = '\0';

            s = lexer_get_current_char_as_string(lexer);
            value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
            strcat(value, s);

            lexer_advance(lexer);

            if (lexer->c == '=')
            {
                s = lexer_get_current_char_as_string(lexer);
                value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
                strcat(value, s);

                return lexer_advance_with_token(lexer, init_token(TOKEN_LESSTHANOREQUALS, value));
            }
            return lexer_advance_with_token(lexer, init_token(TOKEN_LESSTHAN, value));
            break;
        case '!':; // not equal to
            value = calloc(1, sizeof(char));
            value[0] = '\0';

            s = lexer_get_current_char_as_string(lexer);
            value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
            strcat(value, s);

            lexer_advance(lexer);

            if (lexer->c == '=')
            {
                s = lexer_get_current_char_as_string(lexer);
                value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
                strcat(value, s);

                return lexer_advance_with_token(lexer, init_token(TOKEN_NOTEQUAL, value));
            }
            return lexer_advance_with_token(lexer, init_token(TOKEN_INVALID, value));
            break;
        case '(':
            return lexer_advance_with_token(lexer, init_token(TOKEN_LPAREN, lexer_get_current_char_as_string(lexer)));
            break;
        case ')':
            return lexer_advance_with_token(lexer, init_token(TOKEN_RPAREN, lexer_get_current_char_as_string(lexer)));
            break;
        case '[':
            return lexer_advance_with_token(lexer, init_token(TOKEN_LBRACKET, lexer_get_current_char_as_string(lexer)));
            break;
        case ']':
            return lexer_advance_with_token(lexer, init_token(TOKEN_RBRACKET, lexer_get_current_char_as_string(lexer)));
            break;
        case '{':
            return lexer_advance_with_token(lexer, init_token(TOKEN_LBRACE, lexer_get_current_char_as_string(lexer)));
            break;
        case '}':
            return lexer_advance_with_token(lexer, init_token(TOKEN_RBRACE, lexer_get_current_char_as_string(lexer)));
            break;
        case '+':
            return lexer_advance_with_token(lexer, init_token(TOKEN_ADDITION, lexer_get_current_char_as_string(lexer)));
            break;
        case '-':
            return lexer_advance_with_token(lexer, init_token(TOKEN_SUBTRACTION, lexer_get_current_char_as_string(lexer)));
            break;
        case '/':
            return lexer_advance_with_token(lexer, init_token(TOKEN_DIVISION, lexer_get_current_char_as_string(lexer)));
            break;
        case '*':
            return lexer_advance_with_token(lexer, init_token(TOKEN_MULTIPLICATION, lexer_get_current_char_as_string(lexer)));
            break;
        case '%':
            return lexer_advance_with_token(lexer, init_token(TOKEN_MODULUS, lexer_get_current_char_as_string(lexer)));
            break;
        case '@':
            return lexer_advance_with_token(lexer, init_token(TOKEN_INTDIV, lexer_get_current_char_as_string(lexer)));
            break;
        case '^':
            return lexer_advance_with_token(lexer, init_token(TOKEN_EXPONENT, lexer_get_current_char_as_string(lexer)));
            break;
        case ',':
            return lexer_advance_with_token(lexer, init_token(TOKEN_COMMA, lexer_get_current_char_as_string(lexer)));
            break;
        case ';':
            return lexer_advance_with_token(lexer, init_token(TOKEN_SEMICOLON, lexer_get_current_char_as_string(lexer)));
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

// this is for identifying comments
token_T *lexer_collect_comment(lexer_T *lexer)
{

    char *value = calloc(1, sizeof(char));
    value[0] = '\0';

    // saves the first '#'
    char *s = lexer_get_current_char_as_string(lexer);
    value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
    strcat(value, s);

    lexer_advance(lexer);
    // if next character is not #, it is a single line comment
    if (lexer->c != '#')
    {
        while (lexer->c != '\n' && lexer->c != '\0')
        {
            s = lexer_get_current_char_as_string(lexer);
            value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
            strcat(value, s);

            lexer_advance(lexer);
        }

        return init_token(TOKEN_SLCOMMENT, value);
    }

    // this is for multi-line comment
    else
    {
        int found = 0; // to know if we have closing ##

        // s holds the current character and nextcharacter holds the next character
        char *nextcharacter;
        s = lexer_get_current_char_as_string(lexer); // saves the second #
        value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
        strcat(value, s);

        lexer_advance(lexer);
        while (lexer->c != '\0')
        {
            s = lexer_get_current_char_as_string(lexer);
            lexer_advance(lexer);
            nextcharacter = lexer_get_current_char_as_string(lexer);
            if ((*s == '#') && (*nextcharacter == '#'))
            {

                found = 1;
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

        if (found == 1)
        {
            return init_token(TOKEN_MLCOMMENT, value);
        }

        else
        {
            return init_token(TOKEN_INVALID, value);
        }
    }
}

token_T *lexer_collect_id(lexer_T *lexer)
{

    char *nextcharacter;

    char *value = calloc(1, sizeof(char));
    value[0] = '\0';
    int count = 0;

    // gets the first character
    char *s = lexer_get_current_char_as_string(lexer);
    value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
    strcat(value, s);

    // if first character is a digit, possibilities: digit, float, or invalid
    if (isdigit(lexer->c))
    {
        int flag = 0; // 0 if lexeme is number, 1 if lexeme is decimal, 2 if invalid
        lexer_advance(lexer);

        // this is for one-digit num (Ex. 3)
        if ((lexer->c) == ' ' || (lexer->c) == '\0' || (lexer->c) == '\n' || (lexer->c) == '#')
            return init_token(TOKEN_NUM, value);

        // else continue reading
        while (isdigit(lexer->c))
        {
            // if current character is digit, store it to the string
            s = lexer_get_current_char_as_string(lexer);
            value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
            strcat(value, s);

            lexer_advance(lexer);

            // Ex: 23423 <-- next character is whitespace or '#' which implies comment
            if ((lexer->c) == ' ' || (lexer->c) == '\0' || (lexer->c) == '\n' || (lexer->c) == '#')
                return init_token(TOKEN_NUM, value);
        }

        // if current character is not a digit, exit loop then proceed here to check if decimal or invalid or next character is an operator (ex. 23+)
        s = lexer_get_current_char_as_string(lexer);

        // Ex. 323a <-- a is an alphabet AND not decimal point (.)
        if (isalnum(*s) && (*s != '.'))
        {
            lexer_advance(lexer);
            value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
            strcat(value, s);
            flag = 2; // 323a cannot be an identifier or keyword, so this has invalid token

            // keep adding character to string if alnum or decimal point(.) Ex. 323a.323ab, 323a.3.2.3.2
            while ((isalnum(lexer->c) || (lexer->c) == '.') && (lexer->c) != '\n' && (lexer->c) != '\0' && (lexer->c) != ' ')
            {

                // add ko na yung character after ng perod (.)
                s = lexer_get_current_char_as_string(lexer);
                value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
                strcat(value, s);

                lexer_advance(lexer);
            }
        }

        // Ex. 323. <-- current character is decimal point (.)
        else if (*s == '.')
        {
            lexer_advance(lexer);
            nextcharacter = lexer_get_current_char_as_string(lexer);

            // Ex. 323.123 <-- next character after decimal point (.) which is 1 is not whitespace or comment symbol
            if (*nextcharacter != '\0' && *nextcharacter != ' ' && *nextcharacter != '#')
            {
                // if current chracter is (.) dot, and next character is number (Ex. 323.2) <-- possibilities: decimal or invalid
                if (isdigit(*nextcharacter))
                {
                    flag = 1;
                    // add current character to string which is decimal point (.)
                    value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
                    strcat(value, s);

                    // lexer came from nextcharacter from line 246. Then, check the next character after the character after decimal point (Ex. 323.23<-)
                    while ((isalnum(lexer->c) && (lexer->c) != '\n' && (lexer->c) != '\0' && (lexer->c) != ' ') || (lexer->c) == '.')
                    {
                        // Ex. 323.233a <-- a is not a digit, so token is invalid
                        if (!isdigit(lexer->c))
                        {
                            flag = 2;
                        }

                        // add character to string
                        s = lexer_get_current_char_as_string(lexer);
                        value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
                        strcat(value, s);

                        lexer_advance(lexer);
                    }
                }

                // this is if current character is a DOT or is a letter (Ex.23a) <-- after 3, a followed; (Ex. 12.21.1<--decimal point occured more than once)
                else if (isalpha(*nextcharacter) || *nextcharacter == '.')
                {
                    flag = 2; // invalid

                    // add letter or decimal point(.)
                    value = realloc(value, (strlen(value) + strlen(s) + 2) * sizeof(char));
                    strcat(value, s);
                    strcat(value, nextcharacter);
                    lexer_advance(lexer);

                    // continue adding to the string if an operator (or delimiter) is not yet encountered
                    while ((isalnum(lexer->c) || (lexer->c) == '.') && (lexer->c) != '\n' && (lexer->c) != ' ' && (lexer->c) != '\0')
                    {
                        s = lexer_get_current_char_as_string(lexer);
                        value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
                        strcat(value, s);

                        lexer_advance(lexer);
                    }
                }

                else
                {
                    value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
                    strcat(value, s);
                    flag = 2;
                }
            }

            else
            {
                value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
                strcat(value, s);
                flag = 2;
            }
        }

        // Ex: 4+ <-- + is an operator
        else
        {
            flag = 0;
        }

        if (flag == 0)
            return init_token(TOKEN_NUM, value);
        else if (flag == 1)
            return init_token(TOKEN_DECIMAL, value);
        else if (flag == 2)
            return init_token(TOKEN_INVALID, value);
    }

    // if passed character is an alphabet, possibilities: identifier, keyword, reserve word, noise word, or invalid
    else
    {
        while (isalnum(lexer->c))
        {

            if (lexer->c == 'n')
            {
                lexer_advance(lexer);

                if (lexer->c == 'u')
                {
                    s = lexer_get_current_char_as_string(lexer);
                    value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
                    strcat(value, s);

                    lexer_advance(lexer);

                    if (lexer->c == 'm')
                    {
                        s = lexer_get_current_char_as_string(lexer);
                        value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
                        strcat(value, s);

                        lexer_advance(lexer);

                        if (lexer->c == ' ' || lexer->c == '\0' || lexer->c == '\n')
                            return lexer_advance_with_token(lexer, init_token(TOKEN_RESVWORD, value));
                        else
                        {
                            while ((isalnum(lexer->c) || (lexer->c)=='_') && (lexer->c) != ' ' && (lexer->c) != '\n' && (lexer->c) != '\0')
                            {
                                s = lexer_get_current_char_as_string(lexer);
                                value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
                                strcat(value, s);

                                lexer_advance(lexer);
                            }

                            if ((lexer->c) == ' ' && (lexer->c) == '\n' && (lexer->c) == '\0')
                                lexer_advance(lexer);

                            return init_token(TOKEN_ID, value);
                        }
                    }
                    else
                    {
                        return lexer_advance_with_token(lexer, init_token(TOKEN_ID, value));
                    }
                }
                else
                {
                    return lexer_advance_with_token(lexer, init_token(TOKEN_ID, value));
                }
            }
            else if (lexer->c == 'b')
            {
                lexer_advance(lexer);

                if (lexer->c == 'o')
                {
                    s = lexer_get_current_char_as_string(lexer);
                    value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
                    strcat(value, s);

                    lexer_advance(lexer);

                    if (lexer->c == 'o')
                    {
                        s = lexer_get_current_char_as_string(lexer);
                        value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
                        strcat(value, s);

                        lexer_advance(lexer);

                        if (lexer->c == 'l')
                        {
                            s = lexer_get_current_char_as_string(lexer);
                            value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
                            strcat(value, s);

                            lexer_advance(lexer);

                            if (lexer->c == 'e')
                            {
                                s = lexer_get_current_char_as_string(lexer);
                                value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
                                strcat(value, s);

                                lexer_advance(lexer);

                                if (lexer->c == 'a')
                                {
                                    s = lexer_get_current_char_as_string(lexer);
                                    value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
                                    strcat(value, s);

                                    lexer_advance(lexer);

                                    if (lexer->c == 'n')
                                    {
                                        s = lexer_get_current_char_as_string(lexer);
                                        value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
                                        strcat(value, s);

                                        if (lexer->c == ' ' || lexer->c == '\0' || lexer->c == '\n')
                                            return lexer_advance_with_token(lexer, init_token(TOKEN_RESVWORD, value));
                                        else
                                        {
                                            while ((isalnum(lexer->c) || (lexer->c)=='_') && (lexer->c) != ' ' && (lexer->c) != '\n' && (lexer->c) != '\0')
                                            {
                                                s = lexer_get_current_char_as_string(lexer);
                                                value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
                                                strcat(value, s);

                                                lexer_advance(lexer);
                                            }

                                            if ((lexer->c) == ' ' && (lexer->c) == '\n' && (lexer->c) == '\0')
                                                lexer_advance(lexer);

                                            return init_token(TOKEN_ID, value);
                                        }
                                    }
                                    else
                                    {
                                        return lexer_advance_with_token(lexer, init_token(TOKEN_ID, value));
                                    }
                                }
                                else
                                {
                                    return lexer_advance_with_token(lexer, init_token(TOKEN_ID, value));
                                }
                            }
                            else
                            {
                                return lexer_advance_with_token(lexer, init_token(TOKEN_ID, value));
                            }
                        }
                        else
                        {
                            return lexer_advance_with_token(lexer, init_token(TOKEN_ID, value));
                        }
                    }
                    else
                    {
                        return lexer_advance_with_token(lexer, init_token(TOKEN_ID, value));
                    }
                }
                else
                {
                    return lexer_advance_with_token(lexer, init_token(TOKEN_ID, value));
                }
            }
            else if (lexer->c == 'a')
            {
                lexer_advance(lexer);

                if (lexer->c == 'n')
                {
                    s = lexer_get_current_char_as_string(lexer);
                    value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
                    strcat(value, s);

                    return lexer_advance_with_token(lexer, init_token(TOKEN_NOISEWORD, value));
                }
                else
                {
                    return lexer_advance_with_token(lexer, init_token(TOKEN_ID, value));
                }
            }
            else if (lexer->c == 'm')
            {
                lexer_advance(lexer);

                if (lexer->c == 'a')
                {
                    s = lexer_get_current_char_as_string(lexer);
                    value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
                    strcat(value, s);

                    lexer_advance(lexer);

                    if (lexer->c == 't')
                    {
                        s = lexer_get_current_char_as_string(lexer);
                        value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
                        strcat(value, s);

                        lexer_advance(lexer);

                        if (lexer->c == 'c')
                        {
                            s = lexer_get_current_char_as_string(lexer);
                            value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
                            strcat(value, s);

                            lexer_advance(lexer);
                            if (lexer->c == 'h')
                            {
                                s = lexer_get_current_char_as_string(lexer);
                                value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
                                strcat(value, s);

                                return lexer_advance_with_token(lexer, init_token(TOKEN_KEYWORD, value));
                            }
                            else
                            {
                                return lexer_advance_with_token(lexer, init_token(TOKEN_ID, value));
                            }
                        }
                        else
                        {
                            return lexer_advance_with_token(lexer, init_token(TOKEN_ID, value));
                        }
                    }
                    else
                    {
                        return lexer_advance_with_token(lexer, init_token(TOKEN_ID, value));
                    }
                }
                else
                {
                    return lexer_advance_with_token(lexer, init_token(TOKEN_ID, value));
                }
            }
            else if (lexer->c == 'c')
            {
            }
            else if (lexer->c == 'r')
            {
            }
            else if (lexer->c == 'l')
            {
            }
            else if (lexer->c == 'w')
            {
            }
            else if (lexer->c == 'f')
            {
            }
            else if (lexer->c == 'e')
            {
            }
            else if (lexer->c == 'i')
            {
            }
            else if (lexer->c == 't')
            {
            }
            else if (lexer->c == 'd')
            {
            }

            // // -- this part will be replaced
            // int flag = identify_if_keyword(value);

            // if (flag == 1)
            //     return init_token(TOKEN_KEYWORD, value);
            // else if (flag == 2)
            //     return init_token(TOKEN_OPERATOR, value);
            // else if (flag == 3)
            //     return init_token(TOKEN_RESVWORD, value);
            // // -- up to here

            // if (flag == 0 && count < 30)
            //     return init_token(TOKEN_ID, value);
            // else
            //     return init_token(TOKEN_INVALID, value);
        }
    }
}

int identify_if_keyword(char string[])
{

    int isKeyword(char buffer[])
    {
        char keywords[20][10] = {
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
                                 "or",
                                 "not"};

        int i, flag = 0;
        for (i = 0; i < 4; ++i)
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
        char reservedWords[5][10] = {
            "num",
            "decimal",
            "word",
            "letter",
            "boolean"};

        int i, flag = 0;
        for (i = 0; i < 5; ++i)
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
        return 1;
    else if (isOperator(string) == 1)
        return 2;
    else if (isReservedWord(string) == 1)
        return 3;

    return 0;
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
