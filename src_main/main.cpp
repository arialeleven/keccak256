#include <iostream>
#include <cstdint>
#include <chrono>
#include <cstring>
#include <iomanip>

extern "C" {
    void keccak256(uint8_t *output, const uint8_t *input, const size_t input_length);
    void keccak256_(uint8_t *output, const uint8_t *input, const size_t input_length);
}

const int ITERATIONS = 10000000;

void print_hex(const uint8_t *data, size_t length) {
    for (size_t i = 0; i < length; ++i) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)data[i];
    }
    std::cout << std::dec << std::endl;
}

void run_test(const char* name, void (*func)(uint8_t*, const uint8_t*, const size_t)) {
    uint8_t output[32] = {0};
    const uint8_t empty_input[1] = {0}; // Пустой ввод
    
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < ITERATIONS; ++i) {
        func(output, empty_input, 0); // Длина 0
    }
    auto end = std::chrono::high_resolution_clock::now();
    
    std::chrono::duration<double> diff = end - start;
    std::cout << name << " result: ";
    print_hex(output, 32);
    std::cout << name << " time: " << diff.count() << " sec (" 
              << ITERATIONS / diff.count() << " ops/sec)\n";
}

int main() {
    std::cout << "Keccak-256 Empty Input Test (" << (ITERATIONS/1000) << "K iterations)\n";
    std::cout << "=============================================\n";
    
    // Сравниваем результаты
    uint8_t result1[32];
    keccak256(result1, nullptr, 0);
    
    std::cout << "keccak256 : ";
    print_hex(result1, 32);
    
    // Тестируем производительность
    std::cout << "\nPerformance test:\n";
    run_test("keccak256 ", keccak256);
    
    return 0;
}