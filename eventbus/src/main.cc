// #include "sync_event/event.h"
#include "async_event/event.h"
#include "model_a/model_a.h"
#include "model_b/model_b.h"

int main()
{
    EventBus eb;
    ModelA a(eb);
    ModelB b(eb);
    a.send();
    b.send();
    return 0;
}