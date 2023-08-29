#ifndef SCANNER_H
#define SCANNER_H
#define MAXIDLEN 33

#include "structures.h"
#include "buffer.h"
#include <stdio.h>
#include <stdlib.h>

extern token scanner();
extern FILE *microFile;

token scanner();

#endif