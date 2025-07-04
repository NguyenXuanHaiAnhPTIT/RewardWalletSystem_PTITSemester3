﻿// sha256.cpp
#include "Sha256.h"
#include <cstring>
#include <sstream>
#include <iomanip>

typedef unsigned char uint8;
typedef unsigned int uint32;
typedef unsigned long long uint64;

namespace
{
    inline uint32 rotr(uint32 x, uint32 n) { return (x >> n) | (x << (32 - n)); }

    void Sha256_transform(const uint8 data[], uint32 state[])
    {
        static const uint32 k[64] = {
            0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b,
            0x59f111f1, 0x923f82a4, 0xab1c5ed5, 0xd807aa98, 0x12835b01,
            0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7,
            0xc19bf174, 0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
            0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da, 0x983e5152,
            0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147,
            0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc,
            0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
            0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819,
            0xd6990624, 0xf40e3585, 0x106aa070, 0x19a4c116, 0x1e376c08,
            0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f,
            0x682e6ff3, 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
            0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
        };
        uint32 w[64];
        for (int i = 0; i < 16; ++i)
            w[i] = (data[i * 4] << 24) | (data[i * 4 + 1] << 16) |
            (data[i * 4 + 2] << 8) | (data[i * 4 + 3]);
        for (int i = 16; i < 64; ++i)
            w[i] = w[i - 16] + (rotr(w[i - 15], 7) ^ rotr(w[i - 15], 18) ^ (w[i - 15] >> 3)) +
            w[i - 7] + (rotr(w[i - 2], 17) ^ rotr(w[i - 2], 19) ^ (w[i - 2] >> 10));
        uint32 a = state[0], b = state[1], c = state[2], d = state[3];
        uint32 e = state[4], f = state[5], g = state[6], h = state[7];
        for (int i = 0; i < 64; ++i) {
            uint32 temp1 = h + (rotr(e, 6) ^ rotr(e, 11) ^ rotr(e, 25)) +
                ((e & f) ^ (~e & g)) + k[i] + w[i];
            uint32 temp2 = (rotr(a, 2) ^ rotr(a, 13) ^ rotr(a, 22)) +
                ((a & b) ^ (a & c) ^ (b & c));
            h = g; g = f; f = e; e = d + temp1;
            d = c; c = b; b = a; a = temp1 + temp2;
        }
        state[0] += a; state[1] += b; state[2] += c; state[3] += d;
        state[4] += e; state[5] += f; state[6] += g; state[7] += h;
    }
}

std::string Sha256(const std::string& input) {
    uint32 state[8] = {
        0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
        0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
    };

    uint64 bitlen = input.size() * 8;
    std::string data = input;
    data += static_cast<char>(0x80);
    while ((data.size() % 64) != 56)
        data += static_cast<char>(0x00);
    for (int i = 7; i >= 0; --i)
        data += static_cast<char>((bitlen >> (i * 8)) & 0xff);

    for (size_t i = 0; i < data.size(); i += 64)
        Sha256_transform(reinterpret_cast<const uint8*>(data.c_str() + i), state);

    std::ostringstream result;
    for (int i = 0; i < 8; ++i)
        result << std::hex << std::setw(8) << std::setfill('0') << state[i];
    return result.str();
}
