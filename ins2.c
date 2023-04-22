#include <stdio.h>
int add(int a, int b) {
    return a + b;
}
int nand(int a, int b) {
    return ~(a & b);
}
int mult(int a, int b) {
    return a * b;
}
int main() {
    // test the functions
    printf("add(3, 5) = %d\n", add(3, 5));
    printf("nand(5, 6) = %d\n", nand(5, 6));
    printf("mult(4, 6) = %d\n", mult(4, 6));

    return 0;
}
