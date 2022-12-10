#ifndef TOKEN_H
#define TOKEN_H
typedef struct TOKEN_STRUCT
{
    enum
    {
        TOKEN_ID,      // 0
        TOKEN_EQUALS,  // 1
        TOKEN_WORD,    // 2
        TOKEN_LPAREN,  // 3
        TOKEN_RPAREN,  // 4
        TOKEN_KEYWORD, // 5
        TOKEN_OPERATOR // 6

    } type;

    char *value;
} token_T;

token_T *init_token(int type, char *value);
#endif // !TOKEN_H
