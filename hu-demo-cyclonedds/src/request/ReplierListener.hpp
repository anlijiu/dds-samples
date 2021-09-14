/*
(c) Copyright, Real-Time Innovations, 2016-2017.
All rights reserved.

No duplications, whole or partial, manual or electronic, may be made
without express written permission. Any such copies, or
revisions thereof, must display this notice unaltered.
This code contains trade secrets of Real-Time Innovations, Inc.

 */

#ifndef RTI_REQUEST_REPLIERLISTENER_HPP_
#define RTI_REQUEST_REPLIERLISTENER_HPP_

namespace rti { namespace request {

template <typename RequestType, typename ReplyType>
class Replier;

/**
 * @ingroup ReplierModule
 * @headerfile rti/request/ReplierListener.hpp
 * @brief Called when a Replier has new available requests.
 *
 * @brief \dref_details_ReplierListener
 */
template <typename RequestType, typename ReplyType>
class ReplierListener {
public:
    virtual ~ReplierListener()
    {
    }

    /**
     * \dref_ReplierListener_on_request_available
     */
    virtual void on_request_available(
            Replier<RequestType, ReplyType>& replier) = 0;
};


} } // namespace rti::request

#endif // RTI_REQUEST_REPLIERLISTENER_HPP_
