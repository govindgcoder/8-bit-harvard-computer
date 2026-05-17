#include<stdbool.h>
#include<stdint.h>

typedef struct CPU {
  uint8_t PC; // program counter
  uint8_t A; // accumulator register
  uint8_t ROM[256]; //program (main) memory
  uint8_t RAM[32]; // random access
  bool zero; // zero flag for branching
  bool stall_pipe; // to stall pipeline
  uint8_t fetch_IR; // for pipeline fetch
  uint8_t decode_IR; // for pipeline decode
  uint8_t execute_opc; // for pipeline execute before calling alu
  uint8_t execute_opr;
  uint8_t stk; // stack pointer
} CPU;

// no operand NAND means NANDing the accumulator, no operand ST can serve as stop

// I'm thinking of commands: LD, ST, LDI, BRZ, NAND, ADD, PUSH, POP


