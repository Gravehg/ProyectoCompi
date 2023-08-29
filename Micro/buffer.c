#include "buffer.h"
#include <string.h>
#include <stdio.h>

char token_buffer[MAXIDLEN];
char previous_token_buffer[MAXIDLEN];
int buffer_pos = 0;

void clear_buffer(){
    //restart the buffer
    for(int i = 0; i<33; i++){
        token_buffer[i] = '\0';
    }
    buffer_pos = 0;
}

void buffer_char(char ch){ 
    //insert the received token in the buffer
    token_buffer[buffer_pos] = ch;
    buffer_pos++;
}

void lexical_error(char ch,int type){
    //return the lexical error
    switch (type)
    {
        case 0: /* constant-expression */
            printf("Lexical error, expected (=), got: %c\n",ch);
            break;
        case 1: /* constant-expression */
            printf("Lexical error, char error is: %c\n",ch);
            break;
        case 2:
            printf("Lexical error, id is too long: %s\n",token_buffer);
            break;
        default:
            break;
    }
}

token check_reserved(){
    //Compare token_buffer 
    //Is ID? Return ID
    //Is it reserved word? RETURN THE APPROPIATE ONE
    //get the string from the buffer, compare it
    // if string is begin return BEGIN
    //else if string is end return END
    //else if string is read return READ
    //else if string is write return WRITE
    char wordBegin[] = "begin";
    char wordEnd[] = "end";
    char wordRead[] = "read";
    char wordWrite[] = "write";
    char wordThen[] = "then";
    char wordElse[] = "else";
    if(strcmp(wordBegin,token_buffer)==0) return BEGIN;
    if(strcmp(wordEnd,token_buffer)==0) return END;
    if(strcmp(wordRead,token_buffer)==0) return READ;
    if(strcmp(wordWrite,token_buffer)==0) return WRITE;
    else return ID; 
}

/*void removeTrailingSpaces(char* str) {
    int len = strlen(str);
    while (len > 0 && isspace(str[len - 1])) {
        len--;
    }
    str[len] = '\0';
}*/

void clear_previous_buffer(){
    //restart the buffer
    for(int i = 0; i<33; i++){
        previous_token_buffer[i] = '\0';
    }
}