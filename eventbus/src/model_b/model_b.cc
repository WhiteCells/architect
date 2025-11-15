#include "model_b.h"
#include "../async_event/event.h"
#include "../msg/a.h"
#include "../msg/b.h"
#include <iostream>

ModelB::ModelB(EventBus &eb) :
    m_eb(eb)
{
    m_eb.subscribe<ModelBMsg>([](const ModelBMsg &e) {
        std::cout << "ModelB recv int: " << e.m_a << std::endl;
    });
}

void ModelB::send()
{
    m_eb.publish(ModelAMsg {"hhhhh"});
}
