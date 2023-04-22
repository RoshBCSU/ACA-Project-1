#include <stdio.h>
#include <stdlib.h>

#define MEM_SIZE 1024

int speculationEnabled = 0; // flag to indicate if branch speculation is enabled
int speculationAddress = -1; // address of speculated instruction
int speculationResult = -1; // result of speculated instruction

int execute(int opcode, int rs1, int rs2, int imm, int rd) {
    int result = 0;
    if (opcode == 0x03) { // lw
        // check for data hazards
       int data_forwarding (rs1, 0, &forwardA, &forwardB);
        if (forwardA == 1) { // forward from EX/MEM
            rs1 = reg[33];
        } else if (forwardA == 2) { // forward from MEM/WB
            rs1 = memory[reg[32]];
        } else {
            rs1 = memory[reg[rs2] + imm];
        }
        // execute the lw instruction
        result = rs1;
    }
    else if (opcode == 0x23) { // beq
        // check for data hazards
        data_forwarding(rs1, rs2, &forwardA, &forwardB);
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
        // execute the beq instruction
        if (rs1 == rs2) { // Compare the values of the two registers
            speculationEnabled = 1;
            speculationAddress = reg[32] + imm; // calculate the target address of the branch
            speculationResult = speculationAddress + 4; // assume the branch is taken and calculate the result address
        }
        result = 0;
    }
    else if (opcode == 0x33) { // add
        // check for data hazards
        data_forwarding(rs1, rs2, &forwardA, &forwardB);
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
        // execute the add instruction
        result = rs1 + rs2;
    }
    else if (opcode == 0x63) { // branch speculation
        if (speculationEnabled && reg[34] == speculationAddress) { // check if the target address matches the speculated address
            result = speculationResult; // return the result of the speculated instruction
            speculationEnabled = 0; // disable branch speculation
        } else { // if the target address does not match the speculated address
// execute the branch instruction
if (reg[rs1] == reg[rs2]) { // Compare the values of the two registers
reg[34] = reg[34] + imm; // if the condition is true, update the PC to the target address
} else {
reg[34] += 4; // if the condition is false, update the PC to the next instruction
}
speculationEnabled = 0; // disable branch speculation
result = reg[34]; // return the updated PC
}
break;
default: // invalid opcode
printf("Invalid opcode: %d\n", opcode);
exit(1);
}
return result;
}
