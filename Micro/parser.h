#ifndef PARSER_H
#define PARSER_H

#include "structures.h"
#include "semantics.h"

extern token current_token;

void system_goal(); 
void program();
void expression(expr_rec *result); 
void expr_list();
void expr_list_if();
void expression_if(expr_rec *result);
void statement(); 
void statement_if();
void statement_list();
void statement_list_if();
void id_list();
void id_list_if();
void primary(expr_rec *result);
void primary_if(expr_rec *result);
void add_op(op_rec *result);
void match(token tok); 
token next_token();
void syntax_error(token tok);

#endif