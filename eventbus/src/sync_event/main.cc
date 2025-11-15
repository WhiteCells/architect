#include "event.h"
#include <iostream>

struct PlayerMoveEvent
{
    int x;
    int y;
};

EventBus bus;

int main()
{
    bus.subscribe<PlayerMoveEvent>([](const PlayerMoveEvent &e) {
        std::cout << "Player moved to " << e.x << ", " << e.y << std::endl;
    });
    bus.publish(PlayerMoveEvent {1, 2});
    return 0;
}