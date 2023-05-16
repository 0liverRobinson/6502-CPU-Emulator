#include <bitset>
#include <string.h>
#include "cpu.h"
#define COLUMN_LEN 16
#define COLUMN_LEN_UNIT_TEST 55
#define MARGIN 8
#define TEST_ADDRESS ((Byte) 0xBB)
#define TEST_ADDRESS_FULL ((Byte) 0xBBBB)
#define TEST_RESULT ((Byte) 0xAAA)
#define BITSET_ARG 8

void insertIntoTable(string text, int column_padding)
{
    cout << "| " << text;
    for ( ; column_padding-- > 0; )
        cout << " ";
}

void insertIntoTable(bitset<BITSET_ARG> text, int column_padding)
{
    cout << "| " << text;
    for ( ; column_padding-- > 0; )
        cout << " ";
}

void CPU::unitTestOut(string current_instruction, Byte expected_result, Byte returned_result)
{
    string outcome = ( expected_result == returned_result ) ? "Success" : "Failed";

    int first_margin = COLUMN_LEN - outcome.length();
    int second_margin = COLUMN_LEN_UNIT_TEST - current_instruction.length();

    insertIntoTable(outcome, first_margin);

    insertIntoTable(bitset<BITSET_ARG>(expected_result), MARGIN);
    
    insertIntoTable(bitset<BITSET_ARG>(returned_result), MARGIN);

    insertIntoTable(current_instruction, second_margin);

    // New line for table.    
    cout <<"|" << endl;
}

void CPU::unitTestInAccumulator(Byte instruction, Byte expected_result, int clock_cycles, bool isAddrFetch)
{
    memory[program_counter] = instruction;
    if (isAddrFetch)
    {
        /// 0x100 is our test number
        memory[program_counter+1] = TEST_ADDRESS;
        memory[TEST_ADDRESS] = expected_result;
    }
    else 
        memory[program_counter+1] = expected_result;
    executeInstruction(clock_cycles);
    unitTestOut(current_instruction, expected_result, accumulator);
}

void CPU::unitTestInJumpRoutine(Byte instruction, Word expected_address, int clock_cycles)
{
    memory[program_counter] = instruction;
    memory[program_counter+1] = TEST_ADDRESS;
    memory[program_counter+2] = TEST_ADDRESS;

    // Passed Test 1
    executeInstruction(clock_cycles);
    unitTestOut(current_instruction, expected_address, program_counter);

}

void CPU::unitTest()
{
    cout << ":::    ::: ::::    ::: ::::::::::: :::::::::::      ::::::::::: :::::::::: :::::::: :::::::::::" << endl;
    cout << ":+:    :+: :+:+:   :+:     :+:         :+:              :+:     :+:       :+:    :+:    :+:" << endl;    
    cout << "+:+    +:+ :+:+:+  +:+     +:+         +:+              +:+     +:+       +:+           +:+" << endl;    
    cout << "+#+    +:+ +#+ +:+ +#+     +#+         +#+              +#+     +#++:++#  +#++:++#++    +#+" << endl;   
    cout << "+#+    +#+ +#+  +#+#+#     +#+         +#+              +#+     +#+              +#+    +#+" << endl;   
    cout << "#+#    #+# #+#   #+#+#     #+#         #+#              #+#     #+#       #+#    #+#    #+#" << endl;  
    cout << " ########  ###    #### ###########     ###              ###     ########## ########     ###" << endl << endl << endl;


    cout << "+-----------------+-----------------+-----------------+--------------------------------------------------------+" << endl;
    cout << "| Result          | Expected        | Received        | Unit test                                              |" << endl;
    cout << "+-----------------+-----------------+-----------------+--------------------------------------------------------+" << endl;

    
    Byte expected_result = TEST_RESULT;
    Word expected_address = TEST_ADDRESS;

    unitTestInJumpRoutine(JUMP_TO_SUBROUTINE, expected_address, 6);

    unitTestInAccumulator(LOAD_ACCUMULATOR_IMMEDIATE, expected_result, 2, false);    
    unitTestInAccumulator(LOAD_ACCUMULATOR_ZERO_PAGE, expected_result, 3, true);
    unitTestInAccumulator(LOAD_ACCUMULATOR_ZERO_PAGE_X, expected_result, 3, true);
}

int main()
{
    CPU cpu = CPU();
    cpu.unitTest();
}