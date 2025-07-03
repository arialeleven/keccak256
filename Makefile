# Компиляторы
CC := gcc
CXX := g++
ASM := nasm

# Флаги
CFLAGS := -Wall -Wextra -O3 -march=native -fPIC
CXXFLAGS := -Wall -Wextra -O3 -std=c++11 -fPIC

# Исходные файлы
SRC_DIR := src
C_SRCS := $(SRC_DIR)/keccak.c $(SRC_DIR)/keccakf.c
CPP_SRCS := $(SRC_DIR)/benchmark.cpp

# Объектные файлы
C_OBJS := $(C_SRCS:.c=.o)
CPP_OBJS := $(CPP_SRCS:.cpp=.o)

# Целевой файл
TARGET := keccak_benchmark

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(C_OBJS) $(CPP_OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ -no-pie

# Правило для C файлов
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@


# Правило для C++ файлов
$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(C_OBJS) $(CPP_OBJS) $(TARGET)