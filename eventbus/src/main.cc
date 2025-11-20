#include "async_event/event.h"
#include "model_a/model_a.h"
#include "model_b/model_b.h"

int main()
{
    EventBus eb;
    ModelA a(eb);
    ModelB b(eb);
    std::mutex mtx;
    std::condition_variable cv;
    int i = 0;

    std::thread t1([&]() {
        for (int j = 0; j < 100000; ++j) {
            {
                std::lock_guard<std::mutex> lock(mtx);
                i = j;
            }
            cv.notify_all();
            if (j == 99997) {
                a.send(j);
            }
            if (j == 99999) {
                a.send(j);
            }
        }
    });

    std::thread t2([&]() {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [&] {
            return i >= 99998;
        });
        b.cancel();
    });

    // b.send();
    // b.cancel();
    // a.send(0);

    // for (auto i = 1; i < 10; ++i) {
    //     if (!(i % 2)) {
    //         // b.cancel();
    //     }
    //     a.send(i);
    // }

    // b.send();

    t1.join();
    t2.join();

    while (1) {
    }

    return 0;
}