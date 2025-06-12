#include <iostream>
#include <chrono>
#include <string>

// External C function declaration for Keccak-256 hash
extern "C" void keccak256(uint8_t* output, const uint8_t* input, const size_t input_length);

// Simple usage example
int main()
{
    // Input (single zero byte)
    uint8_t in = 0;

    // Buffer for hash output (32 bytes for Keccak-256)
    uint8_t hash[32];

    // Start timing
    auto start = std::chrono::high_resolution_clock::now();

    // Benchmark loop - run hash 2^24 million times
    for (size_t i = 0; i < 16777216; i++) keccak256(hash, &in, 0);

    // End timing
    auto end = std::chrono::high_resolution_clock::now();

    // Calculate and print hashes per second
    std::cout << (16777216 / 1000000.0) / std::chrono::duration<double>(end - start).count() << " MH/s\n";

    // Print resulting hash in hexadecimal
    for (size_t i = 0; i < 32; i++) std::cout << std::hex << (hash[i] >> 4) << (hash[i] & 0b00001111);
    std::cout << '\n';
}