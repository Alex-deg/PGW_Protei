#include "rate_limiter.h"

RateLimiter::RateLimiter() {
    _rate_bps = 10000000; // 10 Мбит/с
    _tokens = 0; 
    _last_update = std::chrono::steady_clock::now();
}

RateLimiter::RateLimiter(uint32_t rate_bps) :
    _rate_bps(rate_bps), _tokens(0), _last_update(std::chrono::steady_clock::now()) {}


bool RateLimiter::consume(uint32_t packet_size) {
    usleep(50000); // задержка для реалистичности
    const auto now = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_ms = now - _last_update;
    
    _tokens += (elapsed_ms.count() * _rate_bps) / 1000;
    if (_tokens > _rate_bps) {
        _tokens = _rate_bps; 
    }
    _last_update = now;

    std::cout << "tokens = " << _tokens    << " packet = "  << packet_size
              << " time = "   << elapsed_ms.count() << " bitrate = " << _rate_bps << std::endl;

    if (packet_size > _tokens) {
        return false;
    }
    _tokens -= packet_size;
    return true;
}

void RateLimiter::set_rate(uint32_t rate_bps) {
    _rate_bps = rate_bps;
}