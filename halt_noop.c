#include <stdio.h>
#include <stdlib.h>
#define HALT 0
#define NOOP 1

              int main() {
 int instr = HALT; // set the instruction to halt
   // execute the instruction
    if (instr == HALT) {
        printf("Halting...\n");
        exit(0); // exit the program
    } else if (instr == NOOP) {
        printf("No operation...\n");
        // continue with the next instruction
    } else {
        printf("Unknown instruction...\n");
        // continue with the next instruction
    }
    return 0;
}
