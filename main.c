#include<stdbool.h>
#include<stdio.h>
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

  CPU mycpu;

  char program[256][10];

  int i = 0;
  while (program[i]!=NULL && i<256) {
    char *p = program[i];
    char ir[10];
    int j = 0;
    while(*p!=' '){
      ir[j]=*p++;j++;
    }
    ir[j]='\0';
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
      return;
    }
    opcode = opcode << 5;
    char operand[4];
    int k = 0;
    while(*p!=' '||*p!='\0'){
      operand[k]=*p+=1;k++;
    }
    operand[k]='\0';
    long operandVal = strtol(operand, NULL, 16);
    if(operandVal<0 || operandVal > 31) return;
    opcode = opcode | ((uint8_t) operandVal);

    mycpu.ROM[i]=opcode;
    i++;
  }

}
