#include "scanner.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

FILE *microFile;

token scanner(){
    int in_char,c;
    clear_buffer();
    int counter_buffer = 0;
    if (feof(microFile)){
        return SCANEOF;
    }
    while((in_char=getc(microFile))!=EOF){
        if (isspace(in_char)){
            continue;
        }else if (isalpha(in_char)){
            /*
            ID:: = LETTER | ID LETTER
                          | ID DIGIT
                          | ID UNDERSCORE
            */
           buffer_char(in_char);
           counter_buffer++;
           for (c=getc(microFile);isalnum(c)||c=='_';c=getc(microFile)){
                buffer_char(c);
                counter_buffer++;
            }
            if(counter_buffer>33){
                ungetc(c,microFile);
                lexical_error("",2);
                fclose(microFile);
                exit(0);
            }
            ungetc(c,microFile);
            return check_reserved();
        }else if (isdigit(in_char)){
            /*
            INTLITERAL::=DIGIT | INTLITERAL DIGIT
            */
           buffer_char(in_char);
           for(c=getc(microFile);isdigit(c);c=getc(microFile)){
                buffer_char(c);
            }
            ungetc(c,microFile);
            return INTLITERAL;
        }else if (in_char == '(')
            return LPAREN;
        else if (in_char == ')')
            return RPAREN;
        else if (in_char==';')
            return SEMICOLON;
        else if (in_char==',')
            return COMMA;
        else if (in_char=='+')
            return PLUSOP;
        else if (in_char=='|')
            return CONDITIONAL;
        else if (in_char==':'){
            /*Looking for ';='*/
            c = getc(microFile);
            if(c=='=')
                return ASSIGNOP;
            else{
                lexical_error(c,0);
                ungetc(c,microFile);
                fclose(microFile);
                exit(0);
            }
        }else if (in_char == '-'){
            /*is it --, comment start*/
            c = getc(microFile);
            if(c=='-'){
                do
                {
                  in_char = getc(microFile);  /* code */
                } while (in_char!='\n' && in_char!=EOF);
            }else{
                ungetc(c,microFile);
                return MINUSOP;
            }
        }else{
            lexical_error(in_char,1);
            fclose(microFile);
            exit(0);
        }
    }
    if (feof(microFile)){
        return SCANEOF;
    }
}

