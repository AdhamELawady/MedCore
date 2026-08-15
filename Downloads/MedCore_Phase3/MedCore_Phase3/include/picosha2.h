// picosha2.h - public-domain single-header SHA256 implementation (compact)
// Minimal subset adapted for this project.
#ifndef PICOSHA2_H
#define PICOSHA2_H

#include <string>
#include <vector>
#include <sstream>
#include <iomanip>

namespace picosha2 {

inline std::string to_hex_string(const std::vector<unsigned char>& buf) {
    std::ostringstream oss;
    oss << std::hex << std::setfill('0');
    for (unsigned char c : buf) oss << std::setw(2) << (int)c;
    return oss.str();
}

// A very small SHA-256 implementation is placed here. For brevity and
// reliability include a compact reference implementation.
// Note: This is NOT optimized; suitable for demonstration and academic use.

inline unsigned int ch(unsigned int x, unsigned int y, unsigned int z) { return (x & y) ^ (~x & z); }
inline unsigned int maj(unsigned int x, unsigned int y, unsigned int z) { return (x & y) ^ (x & z) ^ (y & z); }
inline unsigned int rotr(unsigned int x, unsigned int n) { return (x >> n) | (x << (32 - n)); }
inline unsigned int bsig0(unsigned int x) { return rotr(x, 2) ^ rotr(x, 13) ^ rotr(x, 22); }
inline unsigned int bsig1(unsigned int x) { return rotr(x, 6) ^ rotr(x, 11) ^ rotr(x, 25); }
inline unsigned int ssig0(unsigned int x) { return rotr(x, 7) ^ rotr(x, 18) ^ (x >> 3); }
inline unsigned int ssig1(unsigned int x) { return rotr(x, 17) ^ rotr(x, 19) ^ (x >> 10); }

inline std::vector<unsigned char> sha256_raw(const std::string& msg) {
    static const unsigned int K[64] = {
        0x428a2f98ul,0x71374491ul,0xb5c0fbcful,0xe9b5dba5ul,0x3956c25bul,0x59f111f1ul,0x923f82a4ul,0xab1c5ed5ul,
        0xd807aa98ul,0x12835b01ul,0x243185beul,0x550c7dc3ul,0x72be5d74ul,0x80deb1feul,0x9bdc06a7ul,0xc19bf174ul,
        0xe49b69c1ul,0xefbe4786ul,0x0fc19dc6ul,0x240ca1ccul,0x2de92c6ful,0x4a7484aaul,0x5cb0a9dcul,0x76f988daul,
        0x983e5152ul,0xa831c66dul,0xb00327c8ul,0xbf597fc7ul,0xc6e00bf3ul,0xd5a79147ul,0x06ca6351ul,0x14292967ul,
        0x27b70a85ul,0x2e1b2138ul,0x4d2c6dfcul,0x53380d13ul,0x650a7354ul,0x766a0abbul,0x81c2c92eul,0x92722c85ul,
        0xa2bfe8a1ul,0xa81a664bul,0xc24b8b70ul,0xc76c51a3ul,0xd192e819ul,0xd6990624ul,0xf40e3585ul,0x106aa070ul,
        0x19a4c116ul,0x1e376c08ul,0x2748774cul,0x34b0bcb5ul,0x391c0cb3ul,0x4ed8aa4aul,0x5b9cca4ful,0x682e6ff3ul,
        0x748f82eeul,0x78a5636ful,0x84c87814ul,0x8cc70208ul,0x90befffaul,0xa4506cebul,0xbef9a3f7ul,0xc67178f2ul
    };
    std::vector<unsigned char> data(msg.begin(), msg.end());
    uint64_t bitlen = (uint64_t)data.size() * 8;
    // append 0x80
    data.push_back(0x80);
    // pad with zeros until length mod 512 == 448 bits (56 bytes)
    while ((data.size() % 64) != 56) data.push_back(0x00);
    // append bitlen as big-endian 64-bit
    for (int i = 7; i >= 0; --i) data.push_back(static_cast<unsigned char>((bitlen >> (i * 8)) & 0xff));

    unsigned int h[8] = {0x6a09e667ul,0xbb67ae85ul,0x3c6ef372ul,0xa54ff53aul,0x510e527ful,0x9b05688cul,0x1f83d9abul,0x5be0cd19ul};

    for (size_t chunk = 0; chunk < data.size(); chunk += 64) {
        unsigned int w[64];
        for (int i = 0; i < 16; ++i) {
            w[i] = (unsigned int)data[chunk + i*4] << 24 | (unsigned int)data[chunk + i*4 + 1] << 16 |
                   (unsigned int)data[chunk + i*4 + 2] << 8 | (unsigned int)data[chunk + i*4 + 3];
        }
        for (int i = 16; i < 64; ++i) w[i] = ssig1(w[i-2]) + w[i-7] + ssig0(w[i-15]) + w[i-16];

        unsigned int a = h[0], b = h[1], c = h[2], d = h[3], e = h[4], f = h[5], g = h[6], hh = h[7];
        for (int i = 0; i < 64; ++i) {
            unsigned int t1 = hh + bsig1(e) + ch(e,f,g) + K[i] + w[i];
            unsigned int t2 = bsig0(a) + maj(a,b,c);
            hh = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
        }
        h[0] += a; h[1] += b; h[2] += c; h[3] += d; h[4] += e; h[5] += f; h[6] += g; h[7] += hh;
    }
    std::vector<unsigned char> digest(32);
    for (int i = 0; i < 8; ++i) {
        digest[i*4] = (unsigned char)((h[i] >> 24) & 0xff);
        digest[i*4+1] = (unsigned char)((h[i] >> 16) & 0xff);
        digest[i*4+2] = (unsigned char)((h[i] >> 8) & 0xff);
        digest[i*4+3] = (unsigned char)(h[i] & 0xff);
    }
    return digest;
}

inline std::string hash256_hex_string(const std::string& src) {
    return to_hex_string(sha256_raw(src));
}

} // namespace picosha2

#endif // PICOSHA2_H
