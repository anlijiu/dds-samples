#ifndef SYNC_REPLIER_LISTENER_H
#define SYNC_REPLIER_LISTENER_H

#include <dds/core/ddscore.hpp>
#include <dds/pub/ddspub.hpp>

#include <rti/request/ReplierListener.hpp>

namespace dashboard { namespace dds {

template <typename T, typename T2>
class SyncReplierListener : public rti::request::ReplierListener< T, T2 > {
public:
    /**
     * @brief Constructor
     */
    SyncReplierListener()
    {
    }

    /**
     * @brief Constructor
     *
     * @param on_connect on connect callback
     * @param on_disconnect on disconnect callback
     */
    SyncReplierListener(
            const std::function<T2(const T &)> &on_request)
            : on_request_(on_request)
    {
    }

    /**
     * @brief On request available
     *
     * @param replier that did match
     */
    virtual void on_request_available(
            rti::request::Replier< T, T2 > & replier)
    {
        /*
        * Receive requests and process them
        */
        const auto MAX_WAIT = ::dds::core::Duration::from_secs(5);

        requests_ = replier.receive_requests(MAX_WAIT);
        while (requests_.length() > 0) {
            for (unsigned int i=0 ; i< requests_.length(); i++) {
                if (requests_[i].info().valid()) {
                    T2 reply = on_request_(requests_[i].data());
                    replier.send_reply(reply, requests_[i].info());
                }
            }
            requests_ = replier.receive_requests(MAX_WAIT);
        }
    }

private:
    std::function<T2(const T &)> on_request_;
    ::dds::sub::LoanedSamples<T> requests_;
};

}  // namespace dds
}  // namespace dashboard

#endif  // SYNC_REPLIER_LISTENER_H
