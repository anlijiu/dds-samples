#ifndef __ASYNC_REPLIER_LISTENER_H__
#define __ASYNC_REPLIER_LISTENER_H__

#include <dds/core/ddscore.hpp>
#include <dds/pub/ddspub.hpp>
#include <dds/sub/LoanedSamples.hpp>
#include <dds/sub/SampleInfo.hpp>

#include <rti/request/ReplierListener.hpp>

namespace dashboard {
namespace dds {

template <typename T, typename T2>
class AsyncReplierListener : public rti::request::ReplierListener< T, T2 > {
public:
    AsyncReplierListener()
    {
    }

    AsyncReplierListener(
            const std::function<void(const T &, const ::dds::sub::SampleInfo &)> &on_request)
            : on_request_(on_request)
    {
    }

    virtual void on_request_available(
            ::rti::request::Replier< T, T2 > & replier)
    {
        const auto MAX_WAIT = ::dds::core::Duration::from_secs(5);

        requests_ = replier.receive_requests(MAX_WAIT);
        while (requests_.length() > 0) {
            for (unsigned int i=0 ; i< requests_.length(); i++) {
                if (requests_[i].info().valid()) {
                    // T2 reply = on_request_(requests_[i].data());
                    on_request_(requests_[i].data(), requests_[i].info());
                    // replier.send_reply(reply, requests_[i].info());
                }
            }
            requests_ = replier.receive_requests(MAX_WAIT);
        }
    }

private:
    std::function<void(const T &, const ::dds::sub::SampleInfo&)> on_request_;
    ::dds::sub::LoanedSamples<T> requests_;
};

}  // namespace dds
}  // namespace dashboard

#endif  // __ASYNC_REPLIER_LISTENER_H__
