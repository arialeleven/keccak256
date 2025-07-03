#include <immintrin.h>
#include <iostream>
#include <iomanip>
#include <cstdint>
#include <chrono>

extern "C" void keccakf(uint64_t st[25]);

int main() {
    // Initialize state matrix
    alignas(64) uint64_t state[25] = {0};
    state[0] = 0x123456789ABCDEF;

    // First transformation
    keccakf(state);
    
    std::cout << "State after first keccakf:" << std::endl;
    for (int i = 0; i < 25; i++) {
        if (i > 0 && i % 5 == 0) std::cout << std::endl;
        std::cout << std::hex << std::setw(16) << std::setfill('0') << state[i] << " ";
    }
    std::cout << std::endl << std::endl;

    const int iterations = (1 << 25);
    uint64_t initial_state[25] = {0};
    initial_state[0] = 0x123456789ABCDEF;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < iterations; i++) {
        std::copy(initial_state, initial_state+25, state);
        keccakf(state);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    // Print benchmark results
    std::cout << "\nBenchmark results:" << std::endl;
    std::cout << "Iterations: " << std::dec << iterations << std::endl;
    std::cout << "Total time: " << duration << " ms" << std::endl;
    std::cout << "Time per iteration: " << (double)duration / iterations << " ms" << std::endl;
    std::cout << "Iterations per second: " << (iterations * 1000.0) / duration << std::endl;

    return 0;
}
