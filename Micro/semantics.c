#include "semantics.h"
#include <stdlib.h>
#include <string.h>

FILE *fptr;
FILE *fptrTemp;
struct variable_pointer symbol_table[1000];
int symbol_table_index = 0;
int rbp = 0;

//Semantic processing procs
void start(void){
    //SUpongo que aqui irian encabezados de x86 (?)
  generate_header();
}

void finish(void){
    generate_finish();
    fclose(fptr);
}

void generate_finish(){
    fclose(fptrTemp);
    fprintf(fptr,"%s\n","    mov rax,60");
    fprintf(fptr,"%s\n","    mov rdi, 0");
    fprintf(fptr,"%s\n","    syscall");
    writeOneFileToOther();
    generate_print_functs();
    generate_read_functs();
    generate_print_newline();
}
void writeOneFileToOther(){
    FILE *fptr1;
    fptr1 = fopen("labels.txt", "r");
    char c;
    c = fgetc(fptrTemp);
    while (c != EOF)
    {
        fputc(c, fptr);
        c = fgetc(fptrTemp);
    }
    fclose(fptrTemp);
}
void generate_read_functs(){
    fprintf(fptr,"%s\n","_generate_number:");
    fprintf(fptr,"%s\n","    xor rax,rax");
    fprintf(fptr,"%s\n","    xor rbx,rbx");
    fprintf(fptr,"%s\n","_get_next_digit:");
    fprintf(fptr,"%s\n","    movzx rcx,byte [rsi]");
    fprintf(fptr,"%s\n","    cmp rcx,10");
    fprintf(fptr,"%s\n","    je _return");
    fprintf(fptr,"%s\n","    sub rcx,48");
    fprintf(fptr,"%s\n","    imul rax,rax,10");
    fprintf(fptr,"%s\n","    add rax,rcx");
    fprintf(fptr,"%s\n","    inc rsi");
    fprintf(fptr,"%s\n","    inc rbx");
    fprintf(fptr,"%s\n","    cmp rbx,10");
    fprintf(fptr,"%s\n","    jl _get_next_digit");
    fprintf(fptr,"%s\n","_return:");
    fprintf(fptr,"%s\n","    ret");
}

void generate_print_newline(){
    fprintf(fptr,"%s\n","_print_newline:");
    fprintf(fptr,"%s\n","    mov rcx, 0x0a");
    fprintf(fptr,"%s\n","    mov rax,1");
    fprintf(fptr,"%s\n","    mov rdi,1");
    fprintf(fptr,"%s\n","    mov rsi,rcx");
    fprintf(fptr,"%s\n","    mov rdx,1");
    fprintf(fptr,"%s\n","    syscall");
    fprintf(fptr,"%s\n","    ret");
}

void generate_print_functs(){
    fprintf(fptr,"%s\n","_print_number:");
    fprintf(fptr,"%s\n","    mov rbx,rax");
    fprintf(fptr,"%s\n","    push rbx");
    fprintf(fptr,"%s\n","    test rax,rax");
    fprintf(fptr,"%s\n","    js _is_negative");
    fprintf(fptr,"%s\n","    _jmp_back:");
    fprintf(fptr,"%s\n","    mov rcx,digit");
    fprintf(fptr,"%s\n","    mov rbx,10");
    fprintf(fptr,"%s\n","    mov [rcx],rbx");
    fprintf(fptr,"%s\n","    inc rcx");
    fprintf(fptr,"%s\n","    mov [digitPos],rcx");
    fprintf(fptr,"%s\n","_get_number_loop:");
    fprintf(fptr,"%s\n","    mov rdx,0");
    fprintf(fptr,"%s\n","    mov rbx,10");
    fprintf(fptr,"%s\n","    div rbx");
    fprintf(fptr,"%s\n","    push rax");
    fprintf(fptr,"%s\n","    add rdx,48");
    fprintf(fptr,"%s\n","    mov rcx,[digitPos]");
    fprintf(fptr,"%s\n","    mov [rcx],dl");
    fprintf(fptr,"%s\n","    inc rcx");
    fprintf(fptr,"%s\n","    mov [digitPos],rcx");
    fprintf(fptr,"%s\n","    pop rax");
    fprintf(fptr,"%s\n","    cmp rax,0");
    fprintf(fptr,"%s\n","    jne _get_number_loop");
    fprintf(fptr,"%s\n","_print_number_loop:");
    fprintf(fptr,"%s\n","    mov rcx, [digitPos]");
    fprintf(fptr,"%s\n","    mov rax,1");
    fprintf(fptr,"%s\n","    mov rdi,1");
    fprintf(fptr,"%s\n","    mov rsi,rcx");
    fprintf(fptr,"%s\n","    mov rdx,1");
    fprintf(fptr,"%s\n","    syscall");
    fprintf(fptr,"%s\n","    mov rcx, [digitPos]");
    fprintf(fptr,"%s\n","    dec rcx");
    fprintf(fptr,"%s\n","    mov [digitPos],rcx");
    fprintf(fptr,"%s\n","    cmp rcx,digit");
    fprintf(fptr,"%s\n","    jge _print_number_loop");
    fprintf(fptr,"%s\n","    pop rbx");
    fprintf(fptr,"%s\n","    mov rax,rbx");
    fprintf(fptr,"%s\n","    ret");
    fprintf(fptr,"%s\n","_is_negative:");
    fprintf(fptr,"%s\n","    push rax");
    fprintf(fptr,"%s\n","    mov rcx, [digitPos]");
    fprintf(fptr,"%s\n","    mov rax, 1");          
    fprintf(fptr,"%s\n", "    mov rdi, 1");
    fprintf(fptr,"%s\n", "    mov rsi, minus_sign");
    fprintf(fptr,"%s\n", "    mov rdx, 1");
    fprintf(fptr,"%s\n", "    syscall");
    fprintf(fptr,"%s\n","    pop rax");
    fprintf(fptr,"%s\n","    neg rax");
    fprintf(fptr,"%s\n","    jmp _jmp_back");
}

void generate_header(){
    fprintf(fptr,"%s\n","section .data");
    fprintf(fptr,"%s\n", "minus_sign db '-'");
    fprintf(fptr,"%s\n","section .bss");
    fprintf(fptr,"%s\n","    digit resb 100");
    fprintf(fptr,"%s\n","    digitPos resb 8");
    fprintf(fptr,"%s\n","section .text");
    fprintf(fptr,"%s\n","    global _start","","","");
    fprintf(fptr,"%s\n","_start:","","","");
    fprintf(fptr,"    %s\n","push rbp");
    fprintf(fptr,"    %s\n","mov rbp,rsp");


}

void assign(expr_rec target,expr_rec source){
    char *result = (char *)malloc(100);
    char* destination = search_in_table(target);
    if(source.kind==IDEXPR || source.kind==TEMPEXPR){
        result = search_in_table(source);
        generate("mov","rax",result,"");
        generate("mov",destination,"rax","");
    }else{
        result = extract(source);
        generate("mov qword",destination,result,"");
    }
}

void assign_if(expr_rec target,expr_rec source){
    char *result = (char *)malloc(100);
    char* destination = search_in_table(target);
    if(source.kind==IDEXPR || source.kind==TEMPEXPR){
        result = search_in_table(source);
        generateIf("mov","rax",result,"");
        generateIf("mov",destination,"rax","");
    }else{
        result = extract(source);
        generateIf("mov qword",destination,result,"");
    }
}

char* search_in_table(expr_rec target){
    char *result = (char *)malloc(100);
    strcpy(result, "[rbp+");
    char pointer_pos[100];
    for(int i = 0;i<1000;i++){
        if(strcmp(symbol_table[i].id,target.name)==0){
            sprintf(pointer_pos,"%i",symbol_table[i].pointer_pos);
            strcat(pointer_pos,"]");
            break;
        }
    }
    strcat(result,pointer_pos);
    return result;
}

op_rec process_op(void){
    op_rec o;
    if(current_token == PLUSOP)
        o.operator = PLUS;
    else
        o.operator = MINUS;
    return o;
}

char *extract_op(op_rec op){
    switch (op.operator)
    {
    case PLUS:
        return "add";
        break;
    case MINUS:
        return "sub";
        break;
    default:
        break;
    }
}

void initialize_symbol_table(){
    for(int i = 0;i <1000;i++){
      struct variable_pointer v1;
      v1.id = "\0";
      v1.pointer_pos = 0;
      symbol_table[i] = v1;
    }
}


expr_rec gen_infix(expr_rec e1,op_rec op, expr_rec e2){
    expr_rec e_rec;
    /*AN expr_rec with temp variant set*/
    e_rec.kind = TEMPEXPR;

    /*Generate code for infix operation
    Get result temp and set up semantic record for result
    */
   char *result = (char *)malloc(100);
   strcpy(e_rec.name,get_temp());
   strcpy(result, "[rbp+");

   char *destination = search_in_table(e_rec);

   int eOne,eTwo;
   //Si los dos son literales se suman y se guardan en el temporal
   //Asi se hace un poco el constant folding supongo (?)
   if(e1.kind == LITERALEXPR && e2.kind == LITERALEXPR){
        if(op.operator==PLUS){
            int sum = e2.val+e1.val;
            char* sumChar[100];
            sprintf(sumChar,"%i",sum);
            generate("mov qword",destination,sumChar,"");
        }else{
            int diff = e1.val-e2.val;
            char* sumChar[100];
            sprintf(sumChar,"%i",diff);
            generate("mov qword",destination,sumChar,"");
        }
        return e_rec;
   }
   
   if(e1.kind==LITERALEXPR){
         char *number[100];
         sprintf(number,"%i",e1.val);
         char* e2_direction = search_in_table(e2);
         generate("mov rax,",number,"","");
         if(op.operator==PLUS){
            generate("add","rax",e2_direction,"");
        }else{
            generate("sub","rax",e2_direction,"");
        }
        generate("mov",destination,"rax","");
        return e_rec;
   }

   if(e2.kind==LITERALEXPR){
    if(e2.val!=0){
         char *number[100];
         sprintf(number,"%i",e2.val);
         char* e1_direction = search_in_table(e1);
         generate("mov rax,",number,"","");
         if(op.operator==PLUS){
            generate("add","rax",e1_direction,"");
        }else{
             generate("sub",e1_direction,"rax","");
             generate("mov","rax",e1_direction,"");
        }
        generate("mov",destination,"rax","");
        return e_rec;
    }else{
        char* e1_direction = search_in_table(e1);
        generate("mov","rax",e1_direction,"");
        generate("mov",destination,"rax","");
        return e_rec;
    }
   }

   
   char* e1_direction = search_in_table(e1);
   char* e2_direction = search_in_table(e2);

   generate("mov","rax",e1_direction,"");
   if(op.operator==PLUS){
        generate("add","rax",e2_direction,"");
   }else if(op.operator==MINUS){
        generate("sub","rax",e2_direction,"");
   }
   generate("mov",destination,"rax","");
   return e_rec;
}

expr_rec gen_infix_if(expr_rec e1,op_rec op, expr_rec e2){
    expr_rec e_rec;
    /*AN expr_rec with temp variant set*/
    e_rec.kind = TEMPEXPR;

    /*Generate code for infix operation
    Get result temp and set up semantic record for result
    */
   char *result = (char *)malloc(100);
   strcpy(e_rec.name,get_temp());
   strcpy(result, "[rbp+");

   char *destination = search_in_table(e_rec);

   int eOne,eTwo;
   //Si los dos son literales se suman y se guardan en el temporal
   //Asi se hace un poco el constant folding supongo (?)
   if(e1.kind == LITERALEXPR && e2.kind == LITERALEXPR){
        if(op.operator==PLUS){
            int sum = e2.val+e1.val;
            char* sumChar[100];
            sprintf(sumChar,"%i",sum);
            generateIf("mov qword",destination,sumChar,"");
        }else{
            int diff = e1.val-e2.val;
            char* sumChar[100];
            sprintf(sumChar,"%i",diff);
            generateIf("mov qword",destination,sumChar,"");
        }
        return e_rec;
   }
   
   if(e1.kind==LITERALEXPR){
         char *number[100];
         sprintf(number,"%i",e1.val);
         char* e2_direction = search_in_table(e2);
         generateIf("mov rax,",number,"","");
         if(op.operator==PLUS){
            generateIf("add","rax",e2_direction,"");
        }else{
            generateIf("sub","rax",e2_direction,"");
        }
        generateIf("mov",destination,"rax","");
        return e_rec;
   }

   if(e2.kind==LITERALEXPR){
         char *number[100];
         sprintf(number,"%i",e2.val);
         char* e1_direction = search_in_table(e1);
         generateIf("mov rax,",number,"","");
         if(op.operator==PLUS){
            generateIf("add","rax",e1_direction,"");
        }else{
             generateIf("sub","rax",e1_direction,"");
        }
        generateIf("mov",destination,"rax","");
        return e_rec;
   }

   
   char* e1_direction = search_in_table(e1);
   char* e2_direction = search_in_table(e2);

   generateIf("mov","rax",e1_direction,"");
   if(op.operator==PLUS){
        generateIf("add","rax",e2_direction,"");
   }else if(op.operator==MINUS){
        generateIf("sub","rax",e2_direction,"");
   }
   generateIf("mov",destination,"rax","");
   return e_rec;
}

void generateIf(string op_code, string op1, string op2, string res_field){
    fprintf(fptrTemp, "    %s %s,%s\n", op_code, op1, op2);
}

char *get_temp(void){
    /*Max temporary allocated so far*/
    static int max_temp = 0;
    static char tempname[MAXIDLEN];

    max_temp++;
    sprintf(tempname,"Temp&%d",max_temp);
    check_id(tempname);
    return tempname;
}


void read_id(expr_rec in_var){
    /*Generate code for read*/

    char* to_read = search_in_table(in_var);
    char *instruction =  (char *)malloc(100);
    strcpy(instruction,"    lea rsi,");
    strcat(instruction,to_read);
    fprintf(fptr,"%s\n","    mov rsi,0");
    fprintf(fptr,"%s\n",instruction);
    fprintf(fptr,"%s\n","    mov rdx,10");
    fprintf(fptr,"%s\n","    mov rax,0");
    fprintf(fptr,"%s\n","    syscall");
    fprintf(fptr,"%s\n","    xor rax,rax");
    fprintf(fptr,"%s\n","    mov rbx,0");
    fprintf(fptr,"%s\n",instruction);
    fprintf(fptr,"%s\n","    call _generate_number");
    fprintf(fptr,"%s%s%s\n","    mov ",to_read,",rax");
    fprintf(fptr,"%s\n","    call _print_newline");

}

void read_id_if(expr_rec in_var){
    /*Generate code for read*/

    char* to_read = search_in_table(in_var);
    char *instruction =  (char *)malloc(100);
    strcpy(instruction,"    lea rsi,");
    strcat(instruction,to_read);
    fprintf(fptrTemp,"%s\n","    mov rsi,0");
    fprintf(fptrTemp,"%s\n",instruction);
    fprintf(fptrTemp,"%s\n","    mov rdx,10");
    fprintf(fptrTemp,"%s\n","    mov rax,0");
    fprintf(fptrTemp,"%s\n","    syscall");
    fprintf(fptrTemp,"%s\n","    xor rax,rax");
    fprintf(fptrTemp,"%s\n","    mov rbx,0");
    fprintf(fptrTemp,"%s\n",instruction);
    fprintf(fptrTemp,"%s\n","    call _generate_number");
    fprintf(fptrTemp,"%s%s%s\n","    mov ",to_read,",rax");
    fprintf(fptrTemp,"%s\n","   call _print_newline");
}


expr_rec process_id(void){
    expr_rec t;
    
    check_id(previous_token_buffer);
    t.kind = IDEXPR;
    strcpy(t.name,previous_token_buffer);
    return t;
}

expr_rec process_literal(void){
    expr_rec t;

    /*COnvert literal to a numeric representation and build
    semantic record*/
    t.kind = LITERALEXPR;
    (void) sscanf(previous_token_buffer,"%d",&t.val);// LO que hace es poner el numero
    return t;                                //que esta en el token_buffer en el t.val
}

void write_expr(expr_rec out_expr){

    if (out_expr.kind == LITERALEXPR){
        fprintf(fptr,"    mov rax, %d\n ",out_expr.val);
        fprintf(fptr, "   call _print_number\n");
        fprintf(fptr, "    call _print_newline\n");
    } else {
        char *toprint = search_in_table(out_expr);
        fprintf(fptr,"    mov rax, qword %s\n ",toprint);
        fprintf(fptr, "   call _print_number\n");
        fprintf(fptr, "    call _print_newline\n");
    }
    
}

void write_expr_if(expr_rec out_expr){

    if (out_expr.kind == LITERALEXPR){
        fprintf(fptrTemp,"    mov rax, %d\n ",out_expr.val);
        fprintf(fptrTemp, "   call _print_number\n");
        fprintf(fptrTemp, "    call _print_newline\n");
    } else {
        char *toprint = search_in_table(out_expr);
        fprintf(fptrTemp,"    mov rax, qword %s\n ",toprint);
        fprintf(fptrTemp, "   call _print_number\n");
        fprintf(fptrTemp, "    call _print_newline\n");
    }
}


void generate_tag(int tag_number){
    char *result = (char *)malloc(100);
    strcpy(result, "_L");
    char* tag[100];
    sprintf(tag,"%i",tag_number);
    strcat(result,tag);
    fprintf(fptrTemp,"%s:\n",result);
}

void generate_if(expr_rec out_expr){
    if(out_expr.kind == LITERALEXPR){
        fprintf(fptr,"    mov rax, %d\n ", out_expr.val); 
        fprintf(fptr,"   cmp rax,0\n");
    }else{
        char *direction = search_in_table(out_expr);
        fprintf(fptr,"    mov rax, qword %s\n ",direction); 
        fprintf(fptr,"   cmp rax,0\n");
    }
}

void generate_jump(char*jump,int labelNumber){
    char *result = (char *)malloc(100);
    strcpy(result, " _L");
    strcat(jump,result);
    char* tag[100];
    sprintf(tag,"%i",labelNumber);
    strcat(jump,tag);
    fprintf(fptr,"    %s\n",jump);
}

void generateEOIJmp(int return_number){
    char *result = (char *)malloc(100);
    strcpy(result, "_r");
    char* tag[100];
    sprintf(tag,"%i",return_number);
    strcat(result,tag);
    strcat(result,":\n");
    fprintf(fptr,"    %s\n",result);
}

void generateEOI(int return_number){
    char *result = (char *)malloc(100);
    strcpy(result, "_r");
    char* tag[100];
    sprintf(tag,"%i",return_number);
    strcat(result,tag);
    fprintf(fptrTemp,"    jmp %s\n",result);
}


void generate(string op_code, string op1, string op2, string res_field){
    fprintf(fptr,"    %s",op_code);
    fprintf(fptr,"%s"," ");
    fprintf(fptr,"%s",op1);
    fprintf(fptr,"%s",",");
    fprintf(fptr,"%s",op2);
    fprintf(fptr,"\n");
}

char *extract(expr_rec expr){
    char *result = (char *)malloc(33);

    switch (expr.kind) {
    case IDEXPR:
        strcpy(result, expr.name);
        break;
    case LITERALEXPR:
        sprintf(result, "%d", expr.val);
        break;
    case TEMPEXPR:
        strcpy(result, expr.name);
        break;
    default:
        strcpy(result, ""); 
        break;
    }

    return result;

}

void check_id(string s){
    if(!lookup(s)){
        rbp+=8;
        struct variable_pointer v1;
        v1.id = strdup(s);;
        v1.pointer_pos = rbp;
        enter(v1);
    }
}

void enter(struct variable_pointer s){
    //Meter en la tabla
    symbol_table[symbol_table_index++] = s;
}


int lookup(string s){
    //Revisar si esta en la tabla
    for(int i = 0;i<1000;i++){
        if(strcmp(symbol_table[i].id,s)==0){
            return 1;
        }
    }
    return 0;
}