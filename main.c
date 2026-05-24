#include<stdbool.h>
#include<stdio.h>
#include<stdint.h>
#include<string.h>
#include<stdlib.h>
#include <time.h>

typedef struct CPU {
  uint8_t PC; // program counter
  uint8_t A; // accumulator register
  uint8_t ROM[256]; //program (main) memory
  uint8_t RAM[32]; // random access
  bool zero; // zero flag for branching
  bool stall_pipe; // to stall pipeline
  uint8_t fetch_ir; // for pipeline fetch
  uint8_t decode_ir; // for pipeline decode
  uint8_t execute_opc; // for pipeline execute before calling alu
  uint8_t execute_opr;
  uint8_t stk; // stack pointer
} CPU;

void assembler(char program[][32], CPU *cpu)
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
    char operand[10];
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

// only 15 general registers - i will set 0000 for use in operands
// 000 -> 111: ADD, SUB, NAND, LD, ST, PUSH, POP, BRZ 
// stack can be from 15 to 31
// registers will be from 0 to 14
void process(CPU cpu){
  struct timespec ts;
  ts.tv_sec = 0;           // 0 seconds
  ts.tv_nsec = 750000000;  // 100 million nanoseconds = 0.1 seconds
  while (true) {
    printf("\033[2J\033[H");
    printf("╔═════════════╦═════════════════╦════════════════╦════════════╗\n");
    printf("|  \033[36m[ACC: %02x]\033[0m  |", cpu.A);
    printf("\033[32m[EXECUTE: %02x %02x]\033[0m |", cpu.execute_opc, cpu.execute_opr);
    printf("  \033[33m[DECODE: %02x]\033[0m  |", cpu.decode_ir);
    printf("\033[31m[FETCH: %02x]\033[0m | \n", cpu.fetch_ir);
    printf("╚═════════════╩═════════════════╩════════════════╩════════════╝\n");
    printf("Registers - 12,13,14,15: [%02x][%02x][%02x][%02x]\n",cpu.RAM[11],cpu.RAM[12],cpu.RAM[13],cpu.RAM[14]);
    printf(" \033[1;37mDisplay Registers (12-15):\033[0m\n");
    printf(" ╔════╦════╦════╦════╗\n");
    printf(" ║ %02x ║ %02x ║ %02x ║ %02x ║\n", cpu.RAM[11], cpu.RAM[12], cpu.RAM[13], cpu.RAM[14]);
    printf(" ╚════╩════╩════╩════╝\n");
    
    printf("\n Zero Flag: [%c]   Stall: [%c]\n", cpu.zero ? '1' : '0', cpu.stall_pipe ? '1' : '0');
    
    fflush(stdout);
    nanosleep(&ts, NULL);
    // execute
    if(cpu.execute_opr>15) goto end_exec;
    switch (cpu.execute_opc){
      case 0:
        // add
        if(cpu.execute_opr!=0){
          cpu.A+=cpu.RAM[cpu.execute_opr-1];
        }
        break;
      case 1:
        // sub
        if(cpu.execute_opr!=0){
          cpu.A-=cpu.RAM[cpu.execute_opr-1];
        }
        cpu.zero = (cpu.A == 0);
        break;
      case 2:
        // nand
        if(cpu.execute_opr==0){
          cpu.A=~cpu.A;
        } else {
          cpu.A=~(cpu.A & cpu.RAM[cpu.execute_opr-1]);
        }
        cpu.zero = (cpu.A == 0);
        break;
      case 3:
        // load
        if(cpu.execute_opr==0) cpu.A = 0;
        else {
          cpu.A = cpu.RAM[cpu.execute_opr-1];
        }
        break;
      case 4:
        // store and stop
        if(cpu.execute_opr==0) goto end_process;
        else {
          cpu.RAM[cpu.execute_opr-1]=cpu.A;
        }
        break;
      case 5:
        // push
        if(cpu.execute_opr!=0&&cpu.stk<32){
          cpu.RAM[cpu.stk++]=cpu.A;
        }
        break;
      case 6:
        // pop
        if(cpu.execute_opr!=0&&cpu.stk>15){
          cpu.A=cpu.RAM[--cpu.stk];
        }
        break;
      default:
        break;
    }
    end_exec : ;
    if (cpu.PC == 255) goto end_process;
    // decode
    cpu.decode_ir = cpu.fetch_ir;
    cpu.execute_opr = cpu.fetch_ir & 0x1F;
    cpu.execute_opc = cpu.fetch_ir >> 5;
    if(cpu.execute_opc==7 && cpu.zero){
      cpu.PC -= cpu.execute_opr;
      cpu.stall_pipe=true;
    }
    // fetch
    if(!cpu.stall_pipe){
      cpu.fetch_ir = cpu.ROM[cpu.PC++];
    } else {cpu.stall_pipe=false; cpu.fetch_ir=0;}
  }
  end_process : ;
  printf("Program ended.\n");
}

int main() {
  CPU cpu = {.stk=15};

  printf("8 bit Virtual PC\n");
  printf("format: INSTRUCTION OPERAND eg: ADD 0x01\n");
  printf("Enter instructions (one per line, empty line to finish):\n");
  char program[256][32] = {0};
  int line = 0;
  while (line < 256 && fgets(program[line], 32, stdin)) {
    program[line][strcspn(program[line], "\r\n")] = '\0';
    
    if (program[line][0] == '\0')
      break;
    line++;
  }

  assembler(program, &cpu);

  process(cpu);

  printf("\nROM dump (hex):\n");
  for (int i = 0; i < 256; i++) {
    printf("%02x ", cpu.ROM[i]);
    if ((i + 1) % 16 == 0)
      putchar('\n');
  }
  putchar('\n');

  return 0;
}
