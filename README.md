# Operating Systems Project 1  
**CS3113 – Introduction to Operating Systems - Spring 2025**

## Project Description  
This project involves the implementation of a simplified simulation of operating system process management. The objective is to emulate key operating system functionalities such as process control, memory allocation, instruction execution, and job scheduling. The program reads structured process data, manages memory allocation dynamically, and simulates instruction execution for each process through a basic CPU model.

## Core Functionalities  
- Construction and management of Process Control Blocks (PCBs)  
- Logical to physical memory mapping and allocation  
- Implementation of job and ready queues  
- CPU instruction decoding and execution  
- Logging of memory content, instruction activity, and process termination details  

## Files Included  
- `CS3113_Project1 copy.cpp`: Source code implementing the simulation  
- `CS3113-Spring-2025-ProjectOne.pdf`: Official project specification document  
- `README.md`: Documentation and usage instructions for the project  

## Input Specification  
The program reads from standard input (redirected from a file). The input must follow the format outlined below:
1. An integer indicating the total size of main memory  
2. An integer specifying the number of processes  
3. For each process:
   - `processID maxMemoryNeeded numInstructions`
   - A sequence of instructions encoded as integers. Instruction formats are:
     - `1 <iterations> <cycles>` — Compute  
     - `2 <cycles>` — Print  
     - `3 <value> <address>` — Store  
     - `4 <address>` — Load  

## Instruction Set Summary  
| Opcode | Instruction | Parameters                  | Description                     |
|--------|-------------|-----------------------------|---------------------------------|
| 1      | Compute     | iterations, cycles          | Increments CPU cycle count      |
| 2      | Print       | cycles                      | Simulates output and uses time  |
| 3      | Store       | value, memory address       | Stores value at given location  |
| 4      | Load        | memory address              | Loads value into register       |

## Compilation and Execution  
To compile and run the program using a C++ compiler:
```bash
g++ CS3113_Project1\ copy.cpp -o os_project1
./os_project1 < input.txt
