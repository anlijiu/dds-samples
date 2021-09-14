/*
(c) Copyright, Real-Time Innovations, 2016-2017.
All rights reserved.

No duplications, whole or partial, manual or electronic, may be made
without express written permission. Any such copies, or
revisions thereof, must display this notice unaltered.
This code contains trade secrets of Real-Time Innovations, Inc.

 */

#ifndef __HEADSET_DDS_REQUEST_DETAIL_GENERICRECEIVER_HPP__
#define __HEADSET_DDS_REQUEST_DETAIL_GENERICRECEIVER_HPP__

// IMPORTANT: macros.hpp must be the first RTI header included in every header
// file so that symbols are exported correctly on Windows
#include <dds/core/macros.hpp>

#include <dds/core/QosProvider.hpp>
#include <dds/core/cond/WaitSet.hpp>
#include <dds/domain/DomainParticipant.hpp>
#include <dds/sub/find.hpp>
#include <dds/sub/DataReader.hpp>
#include <dds/sub/cond/ReadCondition.hpp>
#include "request/Common.hpp"

#include <climits>

namespace headunit {
namespace dds {
namespace request {
namespace detail {

// dds::sub::qos::DataReaderQos default_reader_qos() {
//     auto defaultQosProvider = ::dds::core::QosProvider::Default();
//     auto defaultSubscriberQos = defaultQosProvider.subscriber_qos();
//     auto defaultParticipantQos = defaultQosProvider.participant_qos();
//     auto defaultDataReaderQos = defaultQosProvider.datareader_qos();
//     return defaultDataReaderQos;
// };

template <typename T>
class GenericReceiver {
public:
    typedef T DataType;

    GenericReceiver() :
            reader_(::dds::core::null),
            any_sample_cond_(::dds::core::null),
            not_read_sample_cond_(::dds::core::null),
            max_samples_per_read_(dds::core::LENGTH_UNLIMITED)
    {
    }

    GenericReceiver(
            const EntityParams& params,
            ::dds::topic::Topic<T> topic_desc,
            ::dds::sub::DataReaderListener<T>* listener = NULL,
            const ::dds::core::status::StatusMask& mask =
                ::dds::core::status::StatusMask::none())
            : reader_(::dds::core::null),
              any_sample_cond_(::dds::core::null),
              not_read_sample_cond_(::dds::core::null),
              max_samples_per_read_(dds::core::LENGTH_UNLIMITED)
    {
        initialize(params, topic_desc, listener, mask);
    }

    void initialize(
        const EntityParams& params,
        ::dds::topic::Topic<T> topic_desc,
        ::dds::sub::DataReaderListener<T>* listener,
        const ::dds::core::status::StatusMask& mask)
    {
        // Get customized Qos
        ::dds::sub::qos::DataReaderQos qos;
        if (params.datareader_qos().is_set()) {
            qos = params.datareader_qos().get();
        }
        // } else {
        //     qos = default_reader_qos();
        // }

        // Get Subscriber
        ::dds::sub::Subscriber subscriber = params.subscriber();
        if (subscriber == ::dds::core::null) {
            subscriber = ::dds::sub::Subscriber(params.participant());

        }

        reader_ = ::dds::sub::DataReader<T>(subscriber, topic_desc, qos, listener, mask);

        // Create auxiliary ReadConditions and WaitSet
        any_sample_cond_ = ::dds::sub::cond::ReadCondition(
            reader_, ::dds::sub::status::DataState::any());
        not_read_sample_cond_ = ::dds::sub::cond::ReadCondition(
            reader_, ::dds::sub::status::SampleState::not_read());

        waitset_.attach_condition(not_read_sample_cond_);

        // Get max samples per read to validate min_count and max_count parameters
        max_samples_per_read_ = qos.policy<::dds::core::policy::ResourceLimits>()
                .max_samples();
    }

    void close()
    {
        if (closed()) {
            return;
        }

        waitset_ = ::dds::core::null;
        any_sample_cond_.close();
        not_read_sample_cond_.close();
        reader_.close();
    }

    bool closed() const
    {
        return reader_->closed();
    }

    ::dds::sub::LoanedSamples<T> get_samples(int max_count, bool take_samples)
    {
        return read_or_take(max_count, take_samples);
    }


    ::dds::sub::LoanedSamples<T> read_or_take(
            int max_count,
            ::dds::sub::cond::ReadCondition& condition,
            bool take_samples)
    {
        if (max_count == ::dds::core::LENGTH_UNLIMITED) {
            max_count = INT_MAX;
        }

        if (take_samples) {
            return reader_.select()
                .max_samples(max_count)
                .take();
        } else {
            return reader_.select()
                .max_samples(max_count)
                .read();
        }
    }


    ::dds::sub::LoanedSamples<T> read_or_take(int max_count, bool take_samples)
    {
        return read_or_take(max_count, any_sample_cond_, take_samples);
    }

    ::dds::sub::LoanedSamples<T> take_samples()
    {
        return read_or_take(::dds::core::LENGTH_UNLIMITED, any_sample_cond_, true);
    }

    ::dds::sub::LoanedSamples<T> take_samples(::dds::sub::cond::ReadCondition cond)
    {
        return read_or_take(::dds::core::LENGTH_UNLIMITED, cond, true);
    }

    ::dds::sub::LoanedSamples<T> read_samples()
    {
        return read_or_take(::dds::core::LENGTH_UNLIMITED, any_sample_cond_, false);
    }

    ::dds::sub::LoanedSamples<T> read_samples(::dds::sub::cond::ReadCondition cond)
    {
        return read_or_take(::dds::core::LENGTH_UNLIMITED, cond, false);
    }

    ::dds::sub::LoanedSamples<T> receive_samples(
            int min_count,
            int max_count,
            const ::dds::core::Duration& max_wait,
            bool take_samples)
    {
        validate(min_count, max_count, max_wait);
        if (!wait_for_any_sample(min_count, max_wait)) {
            return ::dds::sub::LoanedSamples<T>();
        }

        return read_or_take(max_count, take_samples);
    }


    void validate(
        int min_count, int max_count,
        const ::dds::core::Duration& max_wait)
    {
        if (max_count == ::dds::core::LENGTH_UNLIMITED) {
            max_count = max_samples_per_read_;
        }

        if (min_count == ::dds::core::LENGTH_UNLIMITED) {
            min_count = max_samples_per_read_;
        }

        if ((max_count == 0) || (max_count < 0)) {
            throw ::dds::core::InvalidArgumentError("max_count");
        }

        if (max_count < min_count) {
            throw ::dds::core::InvalidArgumentError("max_count < min_count");
        }

        if (max_wait<=::dds::core::Duration::zero()) {
            throw ::dds::core::InvalidArgumentError("zero duration");
        }

    }

    ::dds::sub::DataReader<T> reader() const
    {
        return reader_;
    }

    void listener(::dds::sub::DataReaderListener<T>* the_listener)
    {
        reader_.listener(
                the_listener,
                ::dds::core::status::StatusMask::data_available());
    }

    int touch_samples(int max_count, ::dds::sub::cond::ReadCondition condition)
    {
        // Read up to max_count samples with the data state defined by condition
        // and return how many there are.
        return reader_.select()
                      .max_samples(max_count)
                      .read()
                      .length();
    }

    bool wait_for_samples(
        int min_sample_count,
        const ::dds::core::Duration& max_wait,
        ::dds::core::cond::WaitSet& waitset,
        ::dds::sub::cond::ReadCondition& initial_condition,
        ::dds::sub::cond::ReadCondition& condition)
    {
        if (min_sample_count == ::dds::core::LENGTH_UNLIMITED) {
            min_sample_count = INT_MAX;
        }

        int sample_count = touch_samples(min_sample_count, initial_condition);
        min_sample_count -= sample_count;

        ::dds::domain::DomainParticipant participant =
            reader_.subscriber().participant();
        ::dds::core::cond::WaitSet::ConditionSeq active_conditions;
        ::dds::core::Duration remaining_wait = max_wait;

        // This implementation is further discussed in jira COP-46, COP-39
        while (min_sample_count > 0) {
            if (min_sample_count == 1) {
                // optimized case: no need to compute wait_time
                waitset.wait(active_conditions, remaining_wait);
            } else {
                // general case
                ::dds::core::Time before_wait = participant.current_time();
                waitset.wait(active_conditions, remaining_wait);
                ::dds::core::Duration wait_time =
                    participant.current_time() - before_wait;

                remaining_wait -= wait_time;
            }

            // waitset only contains 'condition'
            if (active_conditions.size() != 1) {
                return false; // wait timed out
            }

            if (min_sample_count > 1) {
                // Get the number of samples we've received, and mark them
                // as read so that wait() will go back to sleep waiting
                // for new samples.
                sample_count = touch_samples(min_sample_count, condition);
                min_sample_count -= sample_count;
            } else {
                // If we woke up from the waitset, we assume we have at least
                // one sample to read so we can skip touch_samples
                min_sample_count--;
            }

        }

        return true;
    }


    bool wait_for_any_sample(
        int min_count, const ::dds::core::Duration& max_wait)
    {
        return wait_for_samples(
            min_count,
            max_wait,
            waitset_,
            any_sample_cond_,
            not_read_sample_cond_);
    }

    ::dds::sub::LoanedSamples<T> receive_samples(
        int min_count, const ::dds::core::Duration& max_wait)
    {
        if (!wait_for_any_sample(min_count, max_wait)) {
            return ::dds::sub::LoanedSamples<T>();
        }

        return reader_.take();
    }

private:
    ::dds::sub::DataReader<T> reader_;
    ::dds::sub::cond::ReadCondition any_sample_cond_;
    ::dds::sub::cond::ReadCondition not_read_sample_cond_;
    ::dds::core::cond::WaitSet waitset_;
    int max_samples_per_read_;
};

} // namespace detail
} // namespace request
} // namespace dds
} // namespace headunit


#endif // __HEADSET_DDS_REQUEST_DETAIL_GENERICRECEIVER_HPP__
