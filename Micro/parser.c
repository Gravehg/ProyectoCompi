#include <string.h>
#include "parser.h"
#include "scanner.h"

#define MAX_STRING_NUMBER 1000
#define MAX_STRING_SIZE 33


token current_token;

int label_number = 0;
int return_number = 0;

void system_goal(void){
   /*<system goal> :: = <program> SCANEOF*/
   program();
   match(SCANEOF);
   finish();
}

void program(void){
    /*<program> :: BEGIN <statement_list> END*/
    match(BEGIN);
    statement_list();
    match(END);
}

void match(token tok){
    //receive a token  
    if( current_token == tok){
        clear_previous_buffer();
        strcpy(previous_token_buffer,token_buffer);
        current_token = next_token();
    }else{
        syntax_error(tok);
    }
}

token next_token(){
    current_token = scanner();
    return current_token;
}

void statement_list(void){
    /*<statement list>::= <statement> {<statement>}*/
    statement();
    while(1){
        switch (current_token)
        {
        case ID:
        case READ:
        case IF:
        case WRITE:
            statement();
            break;
        default:
            return;
        }
    }
}

void statement_list_if(void){
    /*<statement list>::= <statement> {<statement>}*/
    statement_if();
    while(1){
        switch (current_token)
        {
        case ID:
        case READ:
        case WRITE:
            statement_if();
            break;
        default:
            return;
        }
    }
}

void statement(void){
    switch (current_token) {
        case ID:
            match(ID);
            expr_rec id_rec,expr;
            id_rec = process_id();
            match(ASSIGNOP);
            expression(& expr);
            assign(id_rec,expr);
            match(SEMICOLON);
            break;
        
        case READ:
            match(READ);
            match(LPAREN);
            id_list();
            match(RPAREN);
            match(SEMICOLON);
            break;

        case WRITE:
            match(WRITE);
            match(LPAREN);
            expr_list();
            match(RPAREN);
            match(SEMICOLON);
            break;
        
        case IF:
            match(IF);
            match(LPAREN);
            expr_rec ifexpr;
            expression(&ifexpr);
            generate_if(ifexpr);
            match(THEN);
            generate_tag(label_number);
            char * label = (char *)malloc(100);
            strcpy(label,"jne");
            generate_jump(label,label_number);
            label_number++;
            statement_list_if();
            generateEOI(return_number);
            match(ELSE);
            char * labelNe = (char *)malloc(100);
            strcpy(labelNe,"je");
            generate_jump(labelNe,label_number);
            generate_tag(label_number);
            label_number++;
            statement_list_if();
            generateEOI(return_number);
            match(RPAREN);
            match(SEMICOLON);
            generateEOIJmp(return_number);
            return_number++;
            break;
        case END:
            break;
        default:
            syntax_error(current_token);
            break;
    }
}

void statement_if(void){
    switch (current_token) {
        case ID:
            match(ID);
            expr_rec id_rec,expr;
            id_rec = process_id();
            match(ASSIGNOP);
            expression_if(& expr);
            assign_if(id_rec,expr);
            match(SEMICOLON);
            break;
        
        case READ:
            match(READ);
            match(LPAREN);
            id_list_if();
            match(RPAREN);
            match(SEMICOLON);
            break;

        case WRITE:
            match(WRITE);
            match(LPAREN);
            expr_list_if();
            match(RPAREN);
            match(SEMICOLON);
            break;

        default:
            syntax_error(current_token);
            break;
    }
}


void expression_if(expr_rec *result){
    expr_rec left_operand, right_operand;
    op_rec op;
    primary_if(&left_operand);
    while(current_token==PLUSOP||current_token==MINUSOP){
        add_op(& op);
        primary_if(& right_operand);
        left_operand = gen_infix_if(left_operand,op,right_operand);
    }
    *result = left_operand;
}


void expression(expr_rec *result){
    expr_rec left_operand, right_operand;
    op_rec op;
    primary(&left_operand);
    while(current_token==PLUSOP||current_token==MINUSOP){
        add_op(& op);
        primary(& right_operand);
        left_operand = gen_infix(left_operand,op,right_operand);
    }
    *result = left_operand;
}

void add_op(op_rec *result){
    *result = process_op();
    if(current_token == PLUSOP || current_token == MINUSOP){
        match(current_token);
    }else
        syntax_error(current_token);
}


void primary(expr_rec *result){
    
    expr_rec expr;
    switch(current_token){
        case LPAREN:
            match(LPAREN);
            expression(& expr);
            match(RPAREN);
            break;

        case ID:
            match(ID);
            expr = process_id();
            break;

        case INTLITERAL:
            match(INTLITERAL);
            expr = 
            process_literal();
            break;

        default:
            syntax_error(current_token);
            break;
    }
    *result = expr;


}


void primary_if(expr_rec *result){
    
    expr_rec expr;
    switch(current_token){
        case LPAREN:
            match(LPAREN);
            expression_if(& expr);
            match(RPAREN);
            break;

        case ID:
            match(ID);
            expr = process_id();
            break;

        case INTLITERAL:
            match(INTLITERAL);
            expr = process_literal();
            break;

        default:
            syntax_error(current_token);
            break;
    }
    *result = expr;

}

void id_list(void){
    expr_rec expr;
    match(ID);
    expr = process_id();
    read_id(expr);
    while(current_token == COMMA){
        match(COMMA);
        match(ID);
        expr = process_id();
        read_id(expr);
    }
}

void id_list_if(void){
    expr_rec expr;
    match(ID);
    expr = process_id();
    read_id_if(expr);
    while(current_token == COMMA){
        match(COMMA);
        match(ID);
        expr = process_id();
        read_id_if(expr);
    }
}

void expr_list(void){
    expr_rec expr;
    expression(& expr);
    write_expr(expr);
    while(current_token == COMMA) {
        match(COMMA);
        expression(&expr);
        write_expr(expr);
    }
}


void expr_list_if(void){
    expr_rec expr;
    expression_if(& expr);
    write_expr_if(expr);
    while(current_token == COMMA) {
        match(COMMA);
        expression_if(&expr);
        write_expr_if(expr);
    }
}

void syntax_error(token tok){
    char *result = (char *)malloc(100);
    switch (current_token){
        case 0:/* constant-expression */
            result = "BEGIN";
            break;
        case 1:
            result = "END";
            break;
        case 2:
            result = "READ";
            break;
        case 3:
            result = "WRITE";
            break;
        case 4:
            result = "ID";
            break;
        case 5:
            result = "INTLITERAL";
            break;
        case 6:
            result = "LPAREN";
            break;
        case 7:
            result = "RPAREN";
            break;
        case 8:
            result = "SEMICOLON";
            break;
        case 9:
            result = "COMMA";
            break;
        case 10:
            result = "ASSIGNOP";
            break;
        case 11:
            result = "PLUSOP";
            break;
        case 12:
            result = "MINUSOP";
            break;
        case 13:
            result = "IF";
            break;
        case 14:
            result = "THEN";
            break;
        case 15:
            result = "ELSE";
            break;
        case 16:
            result = "SCANEOF";
            break;
        default:
            result = "";
            break;
    }
    switch (tok){
        case 0:/* constant-expression */
            printf("Error, expected BEGIN {begin}, got: %s\n",result);
            printf("Last read lexeme is: %s\n",token_buffer);
            break;
        case 1:
            printf("Error, expected END {end}, got: %s\n",result);
            printf("Last read lexeme is: %s\n",token_buffer);
            break;
        case 2:
            printf("Error, expected READ {read(<ID_LIST>)}, got: %s\n",result);
            printf("Last read lexeme is: %s\n",token_buffer);
            break;
        case 3:
            printf("Error, expected WRITE {write(<expression_list>)}, got: %s\n",result);
            printf("Last read lexeme is: %s\n",token_buffer);
            break;
        case 4:
            printf("Error, expected ID, got: %s\n",result);
            printf("Last read lexeme is: %s\n",token_buffer);
            break;
        case 5:
            printf("Error, expected INTLITERAL, got: %s\n",result);
            printf("Last read lexeme is: %s\n",token_buffer);
            break;
        case 6:
            printf("Error, expected LPAREN {(}, got: %s\n",result);
            printf("Last read lexeme is: %s\n",token_buffer);
            break;
        case 7:
            printf("Error, expected RPAREN {)}, got: %s\n",result);
            printf("Last read lexeme is: %s\n",token_buffer);
            break;
        case 8:
            printf("Error, expected SEMICOLON {;}, got: %s\n",result);
            printf("Last read lexeme is: %s\n",token_buffer);
            break;
        case 9:
            printf("Error, expected COMMA {,}, got: %s\n",result);
            printf("Last read lexeme is: %s\n",token_buffer);
            break;
        case 10:
            printf("Error, expected ASSIGNOP {:=}, got: %s\n",result);
            printf("Last read lexeme is: %s\n",token_buffer);
            break;
        case 11:
            printf("Error, expected PLUSOP {+}, got: %s\n",result);
            printf("Last read lexeme is: %s\n",token_buffer);
            break;
        case 12:
            printf("Error, expected MINUSOP {-}, got: %s\n",result);
            printf("Last read lexeme is: %s\n",token_buffer);
            break;
        case 13:
            printf("Error, expected IF {if}, got: %s\n",result);
            printf("Last read lexeme is: %s\n",token_buffer);
            break;
        case 14:
            printf("Error, expected THEN {then}, got: %s\n",result);
            printf("Last read lexeme is: %s\n",token_buffer);
            break;
        case 15:
            printf("Error, expected ELSE {else}, got: %s\n",result);
            printf("Last read lexeme is: %s\n",token_buffer);
            break;
        case 16:
            printf("Error, expected SCANEOF , got: %s\n",result);
            printf("Last read lexeme is: %s\n",token_buffer);
            break;
        default:
            break;
    }
    fclose(microFile);
    fclose(fptr);
    fclose(fptrTemp);
    exit(0);
}
