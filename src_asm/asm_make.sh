#!/bin/bash

nasm -f elf64 keccakf.asm -o keccakf.o
g++ -c main.cpp -o main.o
g++ -O3 main.o keccakf.o -o a.out -no-pie
./a.out
rm -f keccakf.o main.o a.out