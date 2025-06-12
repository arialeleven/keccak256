// SPDX-License-Identifier: Apache-2.0

#include <stdint.h>
#include <stddef.h>
#include <string.h>

#define ROUNDS 24
#define KECCAKF_ROUNDS ROUNDS
#define SHA3_256_RATE 136

//extern round funroll function
void keccakf(uint64_t st[25]);

static void keccak_absorb(uint64_t state[25], const uint8_t *in, size_t inlen) {
    while (inlen >= SHA3_256_RATE)
    {
        for (size_t i = 0; i < SHA3_256_RATE / 8; i++)
        {
            uint64_t t = 0;
            for (size_t b = 0; b < 8; b++)
                t |= (uint64_t)in[i * 8 + b] << (8 * b);
            state[i] ^= t;
        }
        keccakf(state);
        in += SHA3_256_RATE;
        inlen -= SHA3_256_RATE;
    }

    uint8_t temp[SHA3_256_RATE] = {0};
    memcpy(temp, in, inlen);
    temp[inlen] = 0x01;
    temp[SHA3_256_RATE - 1] |= 0x80;

    for (size_t i = 0; i < SHA3_256_RATE / 8; i++)
    {
        uint64_t t = 0;
        for (size_t b = 0; b < 8; b++)
            t |= (uint64_t)temp[i * 8 + b] << (8 * b);
        state[i] ^= t;
    }
}

static void keccak_squeeze(uint64_t state[25], uint8_t *out, size_t outlen)
{
    while (outlen > 0)
    {
        size_t block_size = (outlen < SHA3_256_RATE) ? outlen : SHA3_256_RATE;
        for (size_t i = 0; i < block_size; i++)
        {
            out[i] = (state[i / 8] >> (8 * (i % 8))) & 0xFF;
        }
        out += block_size;
        outlen -= block_size;

        if (outlen > 0)
        {
            keccakf(state);
        }
    }
}

void keccak256(uint8_t *output, const uint8_t *input, const size_t input_length)
{
    uint64_t state[25] = {0};
    keccak_absorb(state, input, input_length);
    keccakf(state);
    keccak_squeeze(state, output, 32);
}

