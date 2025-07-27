#pragma once

// Проверить алгоритм кодирования/декодирования - 100% где то не то

#include <string>
#include <vector>
#include <stdint.h>

#include <iostream>

class BCD{
public:
    static std::vector<uint8_t> encode_imsi(const std::string& imsi);
    static std::string decode_imsi(const std::vector<uint8_t>& bytes);
};