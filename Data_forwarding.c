#include <stdio.h>
#include <stdlib.h>
#define HALT 0
#define NOOP 1


int add(int a, int b) {
    return a + b;
}
int nand(int a, int b) {
    return ~(a & b);
}
int mult(int a, int b) {
    return a * b;
}
int memory[1000] = {0}; // Initialize memory with all zeros
int lw(int *reg, int address) {
    *reg = memory[address]; // Load value from memory to register
    return 0;
}
int sw(int reg, int address) {
    memory[address] = reg; // Store value from register to memory
    return 0;
}

int beq(int reg1, int reg2, int offset) {
    if (reg1 == reg2) { // Compare the values of the two registers
        return offset; // If they are equal, return the offset to jump
    }
    return 0;
}
// global variables for registers and memory
int reg[32] = {0};
// function to forward data from the Execute stage to the Decode stage
void data_forwarding(int rs1, int rs2, int *forwardA, int *forwardB) {
    // check for hazards and set forwarding signals accordingly
    if (rs1 == reg[32] && rs1 != 0) { // forwarding from MEM/WB
        *forwardA = 2;
    } else if (rs1 == reg[33] && rs1 != 0) { // forwarding from EX/MEM
        *forwardA = 1;
    } else {
        *forwardA = 0;
    }
    if (rs2 == reg[32] && rs2 != 0) { // forwarding from MEM/WB
        *forwardB = 2;
    } else if (rs2 == reg[33] && rs2 != 0) { // forwarding from EX/MEM
        *forwardB = 1;
    } else {
        *forwardB = 0;
    }
}

int main() {
    int instr = 0x0080006B; // "lw x1, 0(x2)"
    int opcode = instr & 0x7F; // extract the opcode
    int rs1 = (instr >> 15) & 0x1F; // extract the rs1 register number
    int rs2 = (instr >> 20) & 0x1F; // extract the rs2 register number
    int imm = (instr >> 20); // extract the immediate value
    int forwardA, forwardB; // variables to hold forwarding signals

    // execute the instruction with data forwarding
    if (instr == HALT) {
        printf("Halting...\n");
        exit(0); // exit the program
    } else if (instr == NOOP) {
        printf("No operation...\n");
        // continue with the next instruction
    } else if (opcode == 0x03) { // lw
        data_forwarding(rs1, 0, &forwardA, &forwardB); // check for data hazards
        if (forwardA == 1) { // forward from EX/MEM
            reg[rs1] = reg[33];
        } else if (forwardA == 2) { // forward from MEM/WB
            reg[rs1] = memory[reg[32]];
        } else {
            reg[rs1] = memory[reg[rs2] + imm];
        }
    } else if (opcode == 0x33) { // add
data_forwarding(rs1, rs2, &forwardA, &forwardB); // check for data hazards
if (forwardA == 1) { // forward from EX/MEM
rs1 = reg[33];
} else if (forwardA == 2) { // forward from MEM/WB
rs1 = memory[reg[32]];
} else {
rs1 = reg[rs1];
}
    if (forwardB == 1) { // forward from EX/MEM
        rs2 = reg[33];
    } else if (forwardB == 2) { // forward from MEM/WB
        rs2 = memory[reg[32]];
    } else {
        rs2 = reg[rs2];
    }
    reg[32] = rs1 + rs2; // execute the add instruction
}

// print the contents of the registers
printf("Contents of the registers:\n");
for (int i = 0; i < 32; i++) {
    printf("x%d: %d\n", i, reg[i]);
}
return 0;
}
