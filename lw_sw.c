#include <stdio.h>
int memory[1000] = {0}; // Initialize memory with all zeros
int lw(int *reg, int address) {
    *reg = memory[address]; // Load value from memory to register
    return 0;
}
int sw(int reg, int address) {
    memory[address] = reg; // Store value from register to memory
    return 0;
}

int main() {
    int reg = 0;
    int address = 100;

    // Load value from memory to register
    lw(&reg, address);
    printf("Value at memory address %d is %d\n", address, reg);
    // Store value from register to memory
    sw(reg, address);
    printf("Value at memory address %d is now %d\n", address, memory[address]);
    return 0;
}
