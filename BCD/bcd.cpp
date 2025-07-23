#include "bcd.h"

std::vector<uint8_t> BCD::encode_imsi(const std::string& imsi) {
    std::vector<uint8_t> bytes;

    for (size_t i = 0; i < imsi.size(); i += 2) {
        uint8_t digit1 = imsi[i] - '0';
        uint8_t digit2 = (i + 1 < imsi.size()) ? imsi[i+1] - '0' : 0xF; // Filler
        bytes.push_back((digit2 << 4) | digit1); // Важно: digit2 — старшие биты!
    }
    return bytes;
}

std::string BCD::decode_imsi(const std::vector<uint8_t>& bytes) {
    std::string imsi;
    for (uint8_t byte : bytes) {
        uint8_t digit1 = byte & 0x0F;  // Младшие 4 бита
        uint8_t digit2 = (byte >> 4) & 0x0F;  // Старшие 4 бита
        if (digit1 != 0xF) imsi += std::to_string(digit1);
        if (digit2 != 0xF) imsi += std::to_string(digit2);
    }
    return imsi;
}

