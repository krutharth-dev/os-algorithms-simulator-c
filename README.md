# Operating System Algorithms Simulator in C

A menu-driven C program that demonstrates core Operating System concepts including page replacement, memory allocation, disk scheduling, and inter-process communication.

## Features

### Page Replacement Algorithms
- FIFO
- LRU

### Memory Allocation Algorithms
- First Fit
- Best Fit
- Worst Fit

### Disk Scheduling Algorithms
- FCFS
- SSTF
- SCAN
- C-SCAN

### Inter-Process Communication
- Pipe communication
- Producer-consumer using semaphores
- Shared memory writer
- Shared memory reader

## Technologies Used

- C Programming
- GCC Compiler
- POSIX Threads
- Semaphores
- Shared Memory
- Unix/Linux system calls

## Project Structure

.
├── include/
│   ├── diskscheduling.h
│   ├── ipc.h
│   ├── memoryallocation.h
│   └── pagereplacement.h
├── main.c
├── README.md
├── Makefile
└── .gitignore

## How to Compile

gcc -Wall -Wextra -std=c11 main.c -o os_simulator -pthread

## How to Run

./os_simulator

## Purpose

This project was developed to simulate and understand important Operating System algorithms through an interactive command-line interface.

It is useful for learning:
- Memory management
- Page replacement
- Disk scheduling
- Inter-process communication
- Synchronization concepts
