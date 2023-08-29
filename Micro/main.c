#include <stdio.h>
#include "parser.h"
#include "semantics.h"
#include "scanner.h"

int main(int argc, char *argv[]) {
    fptr = fopen("micro-x86.asm","w");
    fptrTemp = fopen("labels.txt","w+");
    microFile = fopen(argv[1],"a+");
    initialize_symbol_table();
    next_token();
    start();
    system_goal();
    compileNASM();
    remove("labels.txt");
}



void compileNASM(){
    system("nasm -f elf64 -o micro-x86.o micro-x86.asm");
    system("ld micro-x86.o -o micro-x86");
    system("./micro-x86");
}


