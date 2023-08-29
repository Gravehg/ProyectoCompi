#ifndef SEMANTICS_H
#define SEMANTICS_H

#include <stdio.h>
#include "structures.h"
#include "buffer.h"
#include "parser.h"

extern FILE *fptr;
extern FILE *fptrTemp;
extern int symbol_table_index;
extern struct variable_pointer symbol_table[1000];
void start(void);
void finish(void);
void assign(expr_rec target, expr_rec source);
void assign_if(expr_rec target, expr_rec source);
op_rec process_op(void);
expr_rec gen_infix(expr_rec e1,op_rec op,expr_rec e2);
expr_rec gen_infix_if(expr_rec e1,op_rec op,expr_rec e2);
void read_id(expr_rec in_var);
void read_id_if(expr_rec in_var);
expr_rec process_id(void);
expr_rec process_literal(void);
void write_expr(expr_rec out_expr);
void write_expr_if(expr_rec out_expr);
void generate(string op_code,string op1, string op2, string res_field);
void generate_header();
void generate_read_functs();
void generate_print_newline();
void generate_print_functs();
void generate_finish();
void generate_if(expr_rec out_expr);
void generate_tag(int tag_number);
void  generateEOIJmp(int return_number);
void writeOneFileToOther();
void generateIf(string op_code,string op1, string op2, string res_field);
void generate_jump(char *jump,int labelNumber);
char *extract(expr_rec expr);
char *get_temp(void);
char *extract_op(op_rec op);
void initialize_symbol_table();
void check_id(string s);
void enter(struct variable_pointer s);
int lookup(string s);
char* search_in_table(expr_rec target);
#endif