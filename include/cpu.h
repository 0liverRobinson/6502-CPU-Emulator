#include <iostream>
#include <stdlib.h>
#include <bitset>
#include "ram.h"


#ifndef CPU_DEFINITION

#define CPU_DEFINITION

using namespace std;


class Instruction
{




};


class CPU : RAM 
{

    public: 

        static const Byte LOAD_ACCUMULATOR_IMMEDIATE = 0XA9,
                            LOAD_ACCUMULATOR_ZERO_PAGE = 0XA5,
                            LOAD_ACCUMULATOR_ZERO_PAGE_X = 0XB5,
                            LOAD_ACCUMULATOR_ABSOLUTE = 0XAD,
                            LOAD_ACCUMULATOR_ABSOLUTE_X = 0XBD,
                            LOAD_ACCUMULATOR_ABSOLUTE_Y = 0XB9,
                            LOAD_ACCUMULATOR_X = 0XA1,
                            LOAD_ACCUMULATOR_Y = 0XB1,
                            
                            LOAD_X_IMMEDIATE = 0XA2,
                            LOAD_X_ZERO_PAGE = 0XA6,
                            LOAD_X_ZERO_PAGE_Y = 0XB6,
                            LOAD_X_ABSOLUTE = 0XAE,
                            LOAD_X_ABSOLUTE_Y = 0XBE,
                            
                            LOAD_Y_IMMEDIATE = 0XA0,
                            LOAD_Y_ZERO_PAGE = 0XA4,
                            LOAD_Y_ZERO_PAGE_X = 0XB4,
                            LOAD_Y_ABSOLUTE = 0XAC,
                            LOAD_Y_ABSOLUTE_X = 0XBC,
                            
                            STORE_ACCUMULATOR_ZERO_PAGE = 0X85,
                            STORE_ACCUMULATOR_ZERO_PAGE_X = 0X95,
                            STORE_ACCUMULATOR_ABSOLUTE = 0X8D,
                            STORE_ACCUMULATOR_ABSOLUTE_X = 0X9D,
                            STORE_ACCUMULATOR_ABSOLUTE_Y = 0X99,
                            STORE_ACCUMULATOR_X = 0X81,
                            STORE_ACCUMULATOR_Y = 0X91,
                            
                            JUMP_TO_SUBROUTINE = 0X20
                    
                            ;

        void unitTest();
        void unitTestOut(string current_instruction, Byte expected_result, Byte returned_result);
        void unitTestInAccumulator(Byte instruction, Byte expected_result, int clock_cycles, bool isAddrFetch);
        void unitTestInJumpRoutine(Byte instruction, Word expected_address, int clock_cycles);

        void loadAccumulatorImmediate(int &clock_cycles);                       // Load / Store instructions
        void loadAccumulatorZeroPage(int &clock_cycles);
        void loadAccumulatorZeroPageX(int &clock_cycles);
        

        void jumpToSubRoutine(int &clock_cycles);

 

        void noOperation(int&clock_cycles);


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
#endif