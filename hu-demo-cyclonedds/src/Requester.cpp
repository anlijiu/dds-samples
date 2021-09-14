
#include "Requester.hpp"

namespace dds {
namespace request {

Requester<RequestType, ReplyType>::Requester
{
}

void Requester<RequestType, ReplyType>::send_request(const RequestType& data)
{
    request_writer->write(hello);
}

void receive_replies(const ::dds::core::Duration&)
{
    bool timeout = cv_.wait_for(lock, seconds, [&]() -> bool
                    {
                        return current_number_ == number_received_;
                    });
}

void Requester<RequestType, ReplyType>::block(
        const std::chrono::seconds& seconds)
{
    std::unique_lock<std::mutex> lock(mutex_);

    bool timeout = cv_.wait_for(lock, seconds, [&]() -> bool
                    {
                        return current_number_ == number_received_;
                    });
}

void Requester<RequestType, ReplyType>::ReplyListener::on_data_available(
        ::dds::sub::DataReader<ReplyType>& reader) {

    dds::sub::LoanedSamples<ReplyType> samples;
    samples = reader.take();
    for (auto sample : samples) { // process the data
        if (sample.info().valid()) {
            requester_.newSample(sample.data());
        }
    }
}

void Requester<RequestType, ReplyType>::newSample(ReplyType data) {
}

} /* request  */     
} /* dds  */ 
