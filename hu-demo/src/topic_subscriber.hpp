#ifndef __TopicSubscriber__
#define __TopicSubscriber__


class TopicSubscriber {
public:
    virtual ~TopicSubscriber() {}
    virtual int received_count() = 0;
};

#endif //__TopicSubscriber__
