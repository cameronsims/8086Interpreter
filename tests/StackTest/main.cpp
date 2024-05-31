#include "../include/cpu/Stack.h"

int main() {

    constexpr uint16_t SIZE = 0xFFFF;

    int nums[SIZE];

    // Get input
    for (uint16_t i = 0x0000; i < SIZE; i++) {
        cpu.stack.push(i);
        nums[i] = i;
    }
    for (uint16_t i = 0x0000; i < SIZE; i++) {
        printf("0x%04x 0x%04x\n", cpu.stack.pop(), nums[(SIZE-1) - i]);
    }
   
    // Print 
    for (uint16_t i = 0x0000; i < SIZE; i++) {
        cpu.stack.push(i);
        cpu.stack.pop();
    }
}