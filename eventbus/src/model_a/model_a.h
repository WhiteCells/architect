// #ifndef _MODEL_A_H_
// #define _MODEL_A_H_

#pragma once

// #include "../sync_event/event.h"
// #include "../async_event/event.h"
// #include "../msg/b.h"
#include <iostream>

class EventBus;

class ModelA
{
public:
    ModelA(EventBus &eb);

    void send();

private:
    EventBus &m_eb;
};

// #endif // _MODEL_A_H_