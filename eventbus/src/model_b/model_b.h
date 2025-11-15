// #ifndef _MODEL_B_H_
// #define _MODEL_B_H_

#pragma once

// #include "../sync_event/event.h"
// #include "../async_event/event.h"
// #include "../msg/a.h"

class EventBus;

class ModelB
{
public:
    ModelB(EventBus &eb);
    void send();

private:
    EventBus &m_eb;
};

// #endif // _MODEL_B_H_
