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
  uint8_t fetch_instruc; // for pipeline fetch
  uint8_t decode_instruc; // for pipeline decode
  uint8_t execute_opc; // for pipeline execute before calling alu
  uint8_t execute_opr;
  uint8_t stk; // stack pointer
} CPU;

void assembler(char program[][10], CPU *cpu)
{

  int i = 0;
  while (program[i][0]!='\0' && i<256) {
    char *p = program[i];
    char instruc[10];
    int j = 0;
    while(*p!=' ' && *p!='\0'){
      instruc[j]=*p++;j++;
    }
    instruc[j]='\0';
    uint8_t opcode = 0;
    if(!strcmp(instruc,"ADD")){ //strcmp returns 0 for true
    } else if(!strcmp(instruc,"SUB")){
      opcode = opcode | 0x1;
    } else if(!strcmp(instruc,"NAND")){
      opcode = opcode | 0x2;
    } else if(!strcmp(instruc,"LD")){
      opcode = opcode | 0x3;
    } else if(!strcmp(instruc,"ST")){
      opcode = opcode | 0x4;
    } else if(!strcmp(instruc,"PUSH")){
      opcode = opcode | 0x5;
    } else if(!strcmp(instruc,"POP")){
      opcode = opcode | 0x6;
    } else if(!strcmp(instruc, "BRZ")){
      opcode = opcode | 0x7;
    } else {
      printf("Bad instruction at %d\n",i+1);
      return;
    }
    opcode = opcode << 5;
    while(*p==' '&&*p!='\0') p++;
    char operand[4];
    int k = 0;
    while(*p!=' '&& *p!='\0'){
      operand[k]=*p++;k++;
    }
    operand[k]='\0';
    long operandVal = strtol(operand, NULL, 16);
    if(operandVal<0 || operandVal > 31) {printf("invalid operand!!! at %d\n",i+1); return;}
    opcode = opcode | ((uint8_t) operandVal);

    cpu->ROM[i]=opcode;
    i++;
  }

}

int main() {
  CPU cpu = {0};
  printf("8 bit Virtual PC\n");
  printf("format: INSTRUCTION OPERAND eg: ADD 0x01\n");
  printf("Enter instructions (one per line, empty line to finish):\n");
  char program[256][10] = {0};
  int line = 0;
  while (line < 256 && fgets(program[line], 10, stdin)) {
    size_t len = strlen(program[line]);
    if (len > 0 && program[line][len-1] == '\n')
      program[line][len-1] = '\0';
    if (program[line][0] == '\0')
      break;
    line++;
  }

  assembler(program, &cpu);

  printf("\nROM dump (hex):\n");
  for (int i = 0; i < 256; i++) {
    printf("%02x ", cpu.ROM[i]);
    if ((i + 1) % 16 == 0)
      putchar('\n');
  }
  putchar('\n');

  return 0;
}
