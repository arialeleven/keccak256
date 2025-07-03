#!/bin/bash

gcc -c keccak.c -O3 -o keccak.o
g++ -c main.cpp -O3 -o main.o
g++ main.o keccak.o -O3 -o a.out
./a.out
rm -f keccak.o main.o a.out