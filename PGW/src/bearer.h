#pragma once

#include <boost/asio/ip/address_v4.hpp>

#include "rate_limiter.h"

class pdn_connection;

class bearer {
public:
    bearer(uint32_t dp_teid, std::shared_ptr<pdn_connection> pdn);
    bearer(uint32_t dp_teid, std::shared_ptr<pdn_connection> pdn, uint32_t uplink_rate, uint32_t downlink_rate);

    [[nodiscard]] uint32_t get_sgw_dp_teid() const;
    void set_sgw_dp_teid(uint32_t sgw_cp_teid);

    [[nodiscard]] uint32_t get_dp_teid() const;

    [[nodiscard]] std::shared_ptr<pdn_connection> get_pdn_connection() const;

    bool check_uplink(uint32_t packet_size);
    bool check_downlink(uint32_t packet_size);
    void set_rates(uint32_t uplink_bps, uint32_t downlink_bps);

private:
    uint32_t _sgw_dp_teid{};
    uint32_t _dp_teid{};
    std::weak_ptr<pdn_connection> _pdn;

    RateLimiter up;
    RateLimiter down;

};