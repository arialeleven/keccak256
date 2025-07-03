// SPDX-License-Identifier: Apache-2.0

#include <stdint.h>
#include <stddef.h>
#include <string.h>

#define ROUNDS 24
#define KECCAKF_ROUNDS ROUNDS
#define SHA3_256_RATE 136

//intrinsic
#if defined(_MSC_VER) // MSVC
#include <intrin.h>
#define ROTL64(value, shift) _rotl64(value, shift)
#define ANDN64(l, r) _andn_u64(l, r)

#else // Fallback
#define ROTL64(value, shift) (((value) << (shift)) | ((value) >> (64 - (shift))))
#define ANDN64(l, r) (~(l) & (r))

#endif

static const uint64_t keccakf_rndc[24] = {
    0x0000000000000001ULL, 0x0000000000008082ULL,
    0x800000000000808aULL, 0x8000000080008000ULL,
    0x000000000000808bULL, 0x0000000080000001ULL,
    0x8000000080008081ULL, 0x8000000000008009ULL,
    0x000000000000008aULL, 0x0000000000000088ULL,
    0x0000000080008009ULL, 0x000000008000000aULL,
    0x000000008000808bULL, 0x800000000000008bULL,
    0x8000000000008089ULL, 0x8000000000008003ULL,
    0x8000000000008002ULL, 0x8000000000000080ULL,
    0x000000000000800aULL, 0x800000008000000aULL,
    0x8000000080008081ULL, 0x8000000000008080ULL,
    0x0000000080000001ULL, 0x8000000080008008ULL
};

static void keccakf(uint64_t st[25])
{
    uint64_t bc[5], t;
    for (int round = 0; round < 24; round++)
    {
        // theta step
        bc[0] = st[0] ^ st[5] ^ st[10] ^ st[15] ^ st[20];
        bc[1] = st[1] ^ st[6] ^ st[11] ^ st[16] ^ st[21];
        bc[2] = st[2] ^ st[7] ^ st[12] ^ st[17] ^ st[22];
        bc[3] = st[3] ^ st[8] ^ st[13] ^ st[18] ^ st[23];
        bc[4] = st[4] ^ st[9] ^ st[14] ^ st[19] ^ st[24];

        t = bc[4] ^ ROTL64(bc[1], 1);
        st[0] ^= t;
        st[5] ^= t;
        st[10] ^= t;
        st[15] ^= t;
        st[20] ^= t;
        t = bc[0] ^ ROTL64(bc[2], 1);
        st[1] ^= t;
        st[6] ^= t;
        st[11] ^= t;
        st[16] ^= t;
        st[21] ^= t;
        t = bc[1] ^ ROTL64(bc[3], 1);
        st[2] ^= t;
        st[7] ^= t;
        st[12] ^= t;
        st[17] ^= t;
        st[22] ^= t;
        t = bc[2] ^ ROTL64(bc[4], 1);
        st[3] ^= t;
        st[8] ^= t;
        st[13] ^= t;
        st[18] ^= t;
        st[23] ^= t;
        t = bc[3] ^ ROTL64(bc[0], 1);
        st[4] ^= t;
        st[9] ^= t;
        st[14] ^= t;
        st[19] ^= t;
        st[24] ^= t;

        // rho and pi steps
        t = st[1];
        bc[0] = st[10];
        st[10] = ROTL64(t, 1);
        t = bc[0];
        bc[0] = st[7];
        st[7] = ROTL64(t, 3);
        t = bc[0];
        bc[0] = st[11];
        st[11] = ROTL64(t, 6);
        t = bc[0];
        bc[0] = st[17];
        st[17] = ROTL64(t, 10);
        t = bc[0];
        bc[0] = st[18];
        st[18] = ROTL64(t, 15);
        t = bc[0];
        bc[0] = st[3];
        st[3] = ROTL64(t, 21);
        t = bc[0];
        bc[0] = st[5];
        st[5] = ROTL64(t, 28);
        t = bc[0];
        bc[0] = st[16];
        st[16] = ROTL64(t, 36);
        t = bc[0];
        bc[0] = st[8];
        st[8] = ROTL64(t, 45);
        t = bc[0];
        bc[0] = st[21];
        st[21] = ROTL64(t, 55);
        t = bc[0];
        bc[0] = st[24];
        st[24] = ROTL64(t, 2);
        t = bc[0];
        bc[0] = st[4];
        st[4] = ROTL64(t, 14);
        t = bc[0];
        bc[0] = st[15];
        st[15] = ROTL64(t, 27);
        t = bc[0];
        bc[0] = st[23];
        st[23] = ROTL64(t, 41);
        t = bc[0];
        bc[0] = st[19];
        st[19] = ROTL64(t, 56);
        t = bc[0];
        bc[0] = st[13];
        st[13] = ROTL64(t, 8);
        t = bc[0];
        bc[0] = st[12];
        st[12] = ROTL64(t, 25);
        t = bc[0];
        bc[0] = st[2];
        st[2] = ROTL64(t, 43);
        t = bc[0];
        bc[0] = st[20];
        st[20] = ROTL64(t, 62);
        t = bc[0];
        bc[0] = st[14];
        st[14] = ROTL64(t, 18);
        t = bc[0];
        bc[0] = st[22];
        st[22] = ROTL64(t, 39);
        t = bc[0];
        bc[0] = st[9];
        st[9] = ROTL64(t, 61);
        t = bc[0];
        bc[0] = st[6];
        st[6] = ROTL64(t, 20);
        t = bc[0];
        bc[0] = st[1];
        st[1] = ROTL64(t, 44);
        t = bc[0];

        // chi step
        bc[0] = st[0];
        bc[1] = st[1];
        bc[2] = st[2];
        bc[3] = st[3];
        bc[4] = st[4];
        st[0] ^= ANDN64(bc[1], bc[2]);
        st[1] ^= ANDN64(bc[2], bc[3]);
        st[2] ^= ANDN64(bc[3], bc[4]);
        st[3] ^= ANDN64(bc[4], bc[0]);
        st[4] ^= ANDN64(bc[0], bc[1]);
        bc[0] = st[5];
        bc[1] = st[6];
        bc[2] = st[7];
        bc[3] = st[8];
        bc[4] = st[9];
        st[5] ^= ANDN64(bc[1], bc[2]);
        st[6] ^= ANDN64(bc[2], bc[3]);
        st[7] ^= ANDN64(bc[3], bc[4]);
        st[8] ^= ANDN64(bc[4], bc[0]);
        st[9] ^= ANDN64(bc[0], bc[1]);
        bc[0] = st[10];
        bc[1] = st[11];
        bc[2] = st[12];
        bc[3] = st[13];
        bc[4] = st[14];
        st[10] ^= ANDN64(bc[1], bc[2]);
        st[11] ^= ANDN64(bc[2], bc[3]);
        st[12] ^= ANDN64(bc[3], bc[4]);
        st[13] ^= ANDN64(bc[4], bc[0]);
        st[14] ^= ANDN64(bc[0], bc[1]);
        bc[0] = st[15];
        bc[1] = st[16];
        bc[2] = st[17];
        bc[3] = st[18];
        bc[4] = st[19];
        st[15] ^= ANDN64(bc[1], bc[2]);
        st[16] ^= ANDN64(bc[2], bc[3]);
        st[17] ^= ANDN64(bc[3], bc[4]);
        st[18] ^= ANDN64(bc[4], bc[0]);
        st[19] ^= ANDN64(bc[0], bc[1]);
        bc[0] = st[20];
        bc[1] = st[21];
        bc[2] = st[22];
        bc[3] = st[23];
        bc[4] = st[24];
        st[20] ^= ANDN64(bc[1], bc[2]);
        st[21] ^= ANDN64(bc[2], bc[3]);
        st[22] ^= ANDN64(bc[3], bc[4]);
        st[23] ^= ANDN64(bc[4], bc[0]);
        st[24] ^= ANDN64(bc[0], bc[1]);

        // iota step
        st[0] ^= keccakf_rndc[round];
    }
}

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
