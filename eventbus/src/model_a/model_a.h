#pragma once

#include "../async_event/event.h"
#include <iostream>
#include <memory>

class ModelA
{
public:
    ModelA(EventBus &eb);
    ~ModelA() = default;

    void send(int i);

    void cancel();

private:
    EventBus &m_eb;
    std::shared_ptr<EventBus::Subscription> m_sub;
};
