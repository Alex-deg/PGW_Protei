#pragma once
#include <chrono>
#include <iostream>
#include <unistd.h>

class RateLimiter {
public:
    RateLimiter();
    RateLimiter(uint32_t rate_bps);

    bool consume(uint32_t packet_size);

    void set_rate(uint32_t rate_bps);
private:
    uint32_t _rate_bps; 
    uint32_t _tokens;  
    std::chrono::steady_clock::time_point _last_update;
};