#pragma once

#include "../async_event/event.h"

class ModelB
{
public:
    ModelB(EventBus &eb);
    ~ModelB() = default;

    void send();

    void cancel();

private:
    EventBus &m_eb;
    std::shared_ptr<EventBus::Subscription> m_sub;
};
