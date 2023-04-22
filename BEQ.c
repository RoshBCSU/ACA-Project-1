#include <stdio.h>
int beq(int reg1, int reg2, int offset) {
    if (reg1 == reg2) { // Compare the values of the two registers
        return offset; // If they are equal, return the offset to jump
    }
    return 0;
}

int main() {
    int reg1 = 10;
    int reg2 = 20;
    int offset = 5;

    // Compare the values of the two registers
    int jump = beq(reg1, reg2, offset);

    printf("Jump offset: %d\n", jump);

    // If the jump offset is non-zero, execute a no-op (do nothing)
    if (jump != 0) {
        printf("Executing no-op\n");
    }
    return 0;
}
