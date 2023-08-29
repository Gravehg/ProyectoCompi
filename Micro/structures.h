#ifndef STRUCTURES_H
#define STRUCTURES_H
#define MAXIDLEN 33

typedef enum token_types {
    BEGIN, END, READ, WRITE, ID, INTLITERAL, LPAREN,
    RPAREN, SEMICOLON, COMMA, ASSIGNOP, PLUSOP, MINUSOP,CONDITIONAL,SCANEOF
} token;

typedef char string[MAXIDLEN];

typedef struct operator {
    enum op { PLUS, MINUS,DESITION} operator;
} op_rec;

typedef struct variable_pointer{
    char* id;
    int pointer_pos;
};

enum expr { IDEXPR, LITERALEXPR, TEMPEXPR };

typedef struct expression {
    enum expr kind;
    union {
        string name;
        int val;
    };
} expr_rec;

#endif