#include "../cpu.h"

void CPU::loadAccumulatorImmediate(int &clock_cycles)
{
    current_instruction = "Load Accumulator Immediate";
    accumulator = memory[program_counter++];
    status_register.fields.zero_flag = (accumulator == 0);
    status_register.fields.negative_flag = ( (accumulator & 0x01000000) > 0 );
    clock_cycles--;
}
void CPU::loadAccumulatorZeroPage(int &clock_cycles)
{
    current_instruction = "Load Accumulator Zero Page";
    Byte zero_page_address = memory[program_counter++];
    clock_cycles--;
    accumulator = memory[zero_page_address];
    clock_cycles--;
    status_register.fields.zero_flag = (accumulator == 0);
    status_register.fields.negative_flag = ( (accumulator & 0x01000000) > 0 );
}
void CPU::loadAccumulatorZeroPageX(int &clock_cycles)
{
    current_instruction = "Load Accumulator Zero Page X";
    Byte zero_page_address = memory[program_counter++];
    clock_cycles--;
    zero_page_address += x;
    clock_cycles--;
    accumulator = memory[zero_page_address];
    clock_cycles--;
    status_register.fields.zero_flag = (accumulator == 0);
    status_register.fields.negative_flag = ( (accumulator & 0x01000000) > 0 );
}