#ifndef __HEADSET_DDS_REQUEST_GENERICSENDER_HPP__
#define __HEADSET_DDS_REQUEST_GENERICSENDER_HPP__

#include <dds/core/QosProvider.hpp>
#include <dds/core/BuiltinTopicTypes.hpp>
#include <dds/domain/DomainPacipant.hpp>
#include <dds/pub/find.hpp>
#include <dds/pub/DataWriter.hpp>
#include "request/detail/Common.hpp"
#include "request/detail/EntityParams.hpp"

namespace headunit {
namespace dds {
namespace request {
namespace detail {

// ::dds::pub::qos::DataWriterQos default_writer_qos();

template <typename T>
class GenericSender {
public:
    typedef T DataType;

    GenericSender(
            const EntityParams& params,
            ::dds::topic::Topic<T> topic,
            const std::string& role_name,
            ::dds::pub::DataWriterListener<T>* listener = NULL,
            const ::dds::core::status::StatusMask& mask =
                ::dds::core::status::StatusMask::none())
        : writer_(::dds::core::null)
    {
        // Get customized Qos, if specified
        ::dds::pub::qos::DataWriterQos qos;
        if (params.datawriter_qos().is_set()) {
            qos = params.datawriter_qos().get();
        }
        // } else {
        //     qos = default_writer_qos();
        // }

        // Get Publisher
        ::dds::pub::Publisher publisher = params.publisher();
        if (publisher == ::dds::core::null) {
            publisher = ::dds::pub::Publisher(params.participant());
        }

        if (listener != NULL) {
            writer_ = ::dds::pub::DataWriter<T>(publisher, topic, qos, listener, mask);
        } else {
            writer_ = ::dds::pub::DataWriter<T>(publisher, topic, qos);
        }
    }

    bool closed()
    {
        return writer_->closed();
    }

    void close()
    {
        writer_.close();
    }

    void write(const T& data)
    {
        writer_.write(data);
    }

    ::dds::pub::DataWriter<T> writer() const
    {
        return writer_;
    }

private:
    ::dds::pub::DataWriter<T> writer_;
};

} // detail 
} // request
} // dds 
} // headunit

#endif // __HEADSET_DDS_REQUEST_GENERICSENDER_HPP__
