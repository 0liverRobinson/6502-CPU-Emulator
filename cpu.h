#include <iostream>
#include <stdlib.h>
#include <bitset>
#include "ram.h"

using namespace std;

class CPU : RAM {

    public: 

        // Operation codes
        static const Byte LOAD_ACCUMULATOR_IMMEDIATE = 0XA9,
                            LOAD_ACCUMULATOR_ZERO_PAGE = 0XA5,
                            LOAD_ACCUMULATOR_ZERO_PAGE_X = 0XB5,
                            LOAD_ACCUMULATOR_ABSOLUTE = 0XAD,
                            LOAD_ACCUMULATOR_ABSOLUTE_X = 0XBD,
                            LOAD_ACCUMULATOR_ABSOLUTE_Y = 0XB9,
                            LOAD_ACCUMULATOR_X = 0XA1,
                            LOAD_ACCUMULATOR_Y = 0XB1,
                            JUMP_TO_SUBROUTINE = 0X20
                            ;

        // Function definitions
        void unitTest();
        void unitTestOut(string current_instruction, Byte expected_result, Byte returned_result);
        void unitTestInAccumulator(Byte instruction, Byte expected_result, int clock_cycles, bool isAddrFetch);
        void unitTestInJumpRoutine(Byte instruction, Word expected_address, int clock_cycles);
        void loadAccumulatorImmediate(int &clock_cycles);
        void loadAccumulatorZeroPage(int &clock_cycles);
        void loadAccumulatorZeroPageX(int &clock_cycles);
        void jumpToSubRoutine(int &clock_cycles);


        // Status register bit field union
        typedef union
        {   
            Byte status;
            struct
            {
                Byte carry_flag: 1;
                Byte zero_flag: 1;
                Byte interrupt_disable: 1;
                Byte decimal_flag: 1;
                Byte break_cmd: 1;
                Byte overflow_flag: 1;
                Byte negative_flag: 1;
            } fields;

        } Status_register;

        Word program_counter;

        Byte stack_pointer,  accumulator, x, y;

        Status_register status_register;

        // Debug stuff
        string current_instruction;

        Byte unit_test_expected_out;


        CPU()
        {
            // Set defaults
            resetCPU();

            // Reset memory
            clearMemory();  

        }

        void dumpMemory()
        {
            for (int i = 0; i < memory_size; i++)
                printf("%c", memory[i]);
            printf("\n");
        }
        
        void startExecuting()
        {
            executeInstruction(3);
        }

    ;


    private:

        void executeInstruction(int clock_cycles)
        {
            for ( ; clock_cycles > 0; clock_cycles-- ) 
            {   
                Byte operation_code = memory[program_counter++];
                
//                cout << "Instruction: " << bitset<8>(operation_code) << " Program Counter: " << program_counter << endl;

                switch (operation_code)
                {
                    case LOAD_ACCUMULATOR_IMMEDIATE:
                        loadAccumulatorImmediate(clock_cycles);
                    break;

                    case LOAD_ACCUMULATOR_ZERO_PAGE:
                        loadAccumulatorZeroPage(clock_cycles);
                    break;

                    case LOAD_ACCUMULATOR_ZERO_PAGE_X:
                        loadAccumulatorZeroPageX(clock_cycles);
                    break;

                    case JUMP_TO_SUBROUTINE:
                        jumpToSubRoutine(clock_cycles);
                    break;

                    default:
                        cout << "[ERROR] No instruction found | Opcode: " << bitset<16>(operation_code) << endl;
                    break;

//                cout << "Instruction: " << bitset<8>(operation_code) << " Program Counter: " << program_counter << endl;

                }
            }
        }

        void resetCPU()
        {
            // Reset prorgam counter position 
            program_counter = 0xFFFC;

            // Reset stack pointer position
            stack_pointer = (Byte) 0x0100;
            
            // Reset x and y registers
            x = 0x0;
            y = 0x0;

            // Reset status register
            status_register.status = 0x0;

            // Reset memory
            clearMemory();

        }   

    ;        

};
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