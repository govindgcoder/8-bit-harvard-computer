#include<stdbool.h>
#include<stdint.h>
#include<string.h>
#include<stdlib.h>

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

void main()
{
  char program[256][10];

  int i = 0;
  while (program[i]!=NULL && i<256) {
    char *p = program[i];
    char ir[];
    int j = 0;
    while(*p!=' '){
      ir[j]=*p;
      *p+=1;j++;
    }
    uint8_t opcode = 0;
    if(strcmp(ir,"ADD")){
    } else if(strcmp(ir,"SUB")){
      opcode = opcode | 0x1;
    } else if(strcmp(ir,"NAND")){
      opcode = opcode | 0x2;
    } else if(strcmp(ir,"LD")){
      opcode = opcode | 0x3;
    } else if(strcmp(ir,"ST")){
      opcode = opcode | 0x4;
    } else if(strcmp(ir,"PUSH")){
      opcode = opcode | 0x5;
    } else if(strcmp(ir,"POP")){
      opcode = opcode | 0x6;
    } else if(strcmp(ir, "BRZ")){
      opcode = opcode | 0x7;
    } else {
      printf("Bad instruction at %d\n",i+1);
    }
    opcode = opcode << 5;
    

    i++;
  }

}
