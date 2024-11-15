#include "event_queue.h"



EventQueue::EventQueue(boost::asio::io_context& io)
    : io_context_{io}, queue_{}
{
}

EventQueue::~EventQueue()
{
}

void EventQueue::AddEvent(shrd_event event)
{
    queue_.push(event);

    std::cout << queue_.size() << std::endl; // !!! тест
}

const bool EventQueue::IsEmpty() const
{
    return queue_.empty();
}

shrd_event EventQueue::ExtractTop()
{
   auto t = queue_.top();   
    queue_.pop();
    std::cout << queue_.size() << std::endl; // !!! тест
    return t;
}

