This project is based on the chapter 2 of the book, "Crating a Compiler with C" by Charles N. Fischer & Richard Joseph LeBlanc.
It developed a MICRO compiler written in C that produces assembler x86. In this case the used assembler is NASM.
MICRO rules (defined on the book):
	1. The only type is integer
	2. All identifiers are implicitly declared and are no longer than 32 characters. Identifiers must begin with a letter and are 
	composed of letters, digits, and underscores
	3. Literals are strings of digits
	4. Comments begin with -- and end at the end of the current line
	5. Statements types are:
		Assignment:
			ID := Expression;
			(where expression can be constructed of literals, identifiers and +,- operators)
		Input/Output:
			read
			write
	6. begin, end, read, write are reserved words.
	7. Each statement is terminated by a semicolon (;)
	8. A blank is appended to the right end of each source line
Note: in this project is implemented the exercise 8 and 10(.a, .c) described and the in the end of the chapter.
      So, "if", "then" and "else" are also reserved words.
      To get more details about the rules its recomended to check the book.
The proyect is divided in some source files and its implementations, in order to separate the diffents funcionalities, those are:
	scanner.c - scanner.h
	buffer.c  - buffer.h
	parser.c  - parser.h
	semantincs.c - semantics.h
	structures.h
	main.c
Where mostly all the files contains code taken from the mentioned book and chapter, with some changes.
The objective for each source (and header) is:
	scanner: contains the method that goes throught the MICRO file separating the different elements in TOKENS
	buffer: contains the buffers functions, to help storing some needed data (saved by the scanner)
	parser: contains functions in charge of check that the receiving TOKENS are gramatically correct
	semantics: contains methods that separates the expressions in order to write the assembler output
	structures: contains the structs, enums and similars, to express the MICRO definition
	main: contains the main program. It starts the compiler and run the remaining .asm file
To run the compiler:
	1. Open the folder in terminal
	2. Write & run make clean
	3. Write & run make
	4. Write & run ./compileMicro < MICRO-code.txt
To change the MICRO source file:
	1. Open the MICRO-code.txt on a text editor
	2. Make the wanted changes (following MICRO rules or not)
	3. Run the compiler
	
	
Project Status:
	Finished
Members:
	Jeffrey Daniel Leiva Cascante
	Lucas Alexander Oviedo Castro

