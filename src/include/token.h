#ifndef TOKEN_H
#define TOKEN_H
typedef struct TOKEN_STRUCT
{
    enum
    {
        TOKEN_ID,
        TOKEN_EQUALS,
        TOKEN_WORD,
        TOKEN_LPAREN,
        TOKEN_RPAREN,
        TOKEN_KEYWORD

    } type;

    char *value;
} token_T;

token_T *init_token(int type, char *value);
#endif // !TOKEN_H
