#include "model_a.h"
#include "../async_event/event.h"
#include "../msg/a.h"
#include "../msg/b.h"
#include <iostream>

ModelA::ModelA(EventBus &eb) :
    m_eb(eb)
{
    m_eb.subscribe<ModelAMsg>([](const ModelAMsg &e) {
        std::cout << "ModelA recv msg: " << e.m_s << std::endl;
    });
}

void ModelA::send()
{
    m_eb.publish(ModelBMsg {1});
}