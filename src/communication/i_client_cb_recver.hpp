/**
 * @file i_client_cb_recver.hpp
 * @author mayfa
 * @date Jun 25, 2018
 */

#ifndef I_CLIENT_CB_RECVER_HPP_
#define I_CLIENT_CB_RECVER_HPP_

#include "interval_list.hpp"

/**
 * @brief Interface for client callbacks receivers.
 *
 * @note The callback functions are not pure virtual.
 */
class IClientCbRecver {
public:
    virtual void connectedCb(uint32_t server_real_time) {}
    virtual void tempSentCb() {}
    virtual void intervalsSentCb() {}
    virtual void intervalsRecvCb(const IntervalList &interval_list) {}
};



#endif /* I_CLIENT_CB_RECVER_HPP_ */
