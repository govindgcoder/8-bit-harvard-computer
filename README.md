# 8 bit Harvard Computer 
An 8 bit Virtual CPU in C that executes a custom bytecode instruction set

## 🎯 Purpose & Learning Goals
I built this mini project to put what i learned in computer architecture to use and to practise low level programming in C.

## ✨ Features
* Custom assembler that converts simple assembly mnemonics to machine code
* 8 bit Harvard architecture with separate program and data memory
* Pipelined execution with fetch decode execute stages
* Branching with zero flag and pipeline stall handling
* Stack implementation for push and pop operations
* Register and memory visualization in terminal
* ROM dump after execution

## 🧰 Tech Stack
* Language: C (standard library only)
* No external frameworks or libraries
* Built and run with any standard C compiler


## ⚙️ Setup & Installation
1. Clone the repo: git clone https://github.com/yourusername/8-bit-harvard-computer.git
2. Compile the code: gcc main.c -o cpu
3. Run the program: ./cpu
4. Enter instructions one per line (empty line to finish) using format INSTRUCTION OPERAND eg ADD 0x01
5. Watch the CPU state update in real time
