/**
 * 针对每一个dds topic 的transeiver
 * 基于以太网的更高层 dds 协议 与 CAN总线有着本质的不同
 * 不同于CAN总线的设计， CAN总线的 transeiver 是面向总线
 * 而本 transeiver 则是面向一个个独立的 dds topic
 */

class DdsMessageTranseiver {
}

template<typename T>
class DdsMessageTranseiverImpl : public DdsMessageTranseiver {
    ::dds::
    ::dds::topic::Topic<hvac_msgs::msg::Ac> mTopicAc;
    ::dds::sub::DataReader<hvac_msgs::msg::Ac> mReaderAc;
}
