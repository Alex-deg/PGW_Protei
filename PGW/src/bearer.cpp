#include "bearer.h"

#include "pdn_connection.h"

bearer::bearer(uint32_t dp_teid, std::shared_ptr<pdn_connection> pdn) : _dp_teid(dp_teid), _pdn(pdn) {}

bearer::bearer(uint32_t dp_teid, std::shared_ptr<pdn_connection> pdn, 
           uint32_t uplink_rate, uint32_t downlink_rate)
        : _dp_teid(dp_teid),
          _pdn(pdn),
          up(uplink_rate),
          down(downlink_rate) {}

uint32_t bearer::get_sgw_dp_teid() const { return _sgw_dp_teid; }

void bearer::set_sgw_dp_teid(uint32_t sgw_cp_teid) { _sgw_dp_teid = sgw_cp_teid; }

uint32_t bearer::get_dp_teid() const { return _dp_teid; }

std::shared_ptr<pdn_connection> bearer::get_pdn_connection() const { return _pdn.lock(); }

bool bearer::check_uplink(uint32_t packet_size) {
    return up.consume(packet_size * 8); 
}

bool bearer::check_downlink(uint32_t packet_size) {
    return down.consume(packet_size * 8);
}

void bearer::set_rates(uint32_t uplink_bps, uint32_t downlink_bps) {
    up.set_rate(uplink_bps);
    down.set_rate(downlink_bps);
}