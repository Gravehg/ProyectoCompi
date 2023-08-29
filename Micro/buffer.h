#ifndef BUFFER_H
#define BUFFER_H

#include "structures.h"

extern char token_buffer[];
extern char previous_token_buffer[];
extern int buffer_pos;

void clear_buffer();
void buffer_char(char ch);
void lexical_error(char ch,int type);
token check_reserved();
void clear_previous_buffer();

#endif