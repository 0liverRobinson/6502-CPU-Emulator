#include "../cpu.h"

void CPU::jumpToSubRoutine(int &clock_cycles)
{
    current_instruction = "Jump to Sub Routine";
    Word target_address = (memory[program_counter+1] | ( memory[program_counter+2] << 8 ));
    program_counter+=2;
    memory[stack_pointer] = ( (program_counter - 1) & 0xFF);
    memory[stack_pointer+1] = ( (program_counter - 1) >> 8);
    program_counter = target_address;
    clock_cycles-=6;
}