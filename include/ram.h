#ifndef RAM_DEFINITION
#define RAM_DEFINITION

// 8 Bit
using Byte = unsigned char;
// 16 Bit
using Word = unsigned short;

class RAM
{
    
    public:
        
        static const int memory_size = 65536;

        Byte memory[memory_size];
        
        RAM () { clearMemory(); }

        void clearMemory() 
        {
            for (int i = 0; i < memory_size; i++)
               memory[i] = (Byte) 0x1;
        }

        Byte getByte()
        {
            return 0x0;
        }
        Word getWord()
        {
            return 0x0;
        }

};
#endif