#ifndef _EVENT_H_
#define _EVENT_H_

#include <functional>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <mutex>

class EventBus
{
public:
    EventBus() = default;
    ~EventBus() = default;

    template <typename Event>
    void subscribe(std::function<void(const Event &)> handler)
    {
        std::lock_guard<std::mutex> lock(m_mtx);
        auto &hlds = m_subs[std::type_index(typeid(Event))];
        hlds.push_back([handler](const void *p) {
            handler(*static_cast<const Event *>(p));
        });
    }

    template <typename Event>
    void publish(const Event &event)
    {
        std::lock_guard<std::mutex> lock(m_mtx);
        auto it = m_subs.find(std::type_index(typeid(Event)));
        if (it == m_subs.end()) {
            return;
        }
        for (const auto &fn : it->second) {
            fn(&event);
        }
    }

private:
    std::mutex m_mtx;
    std::unordered_map<std::type_index, std::vector<std::function<void(const void *)>>> m_subs;
};

#endif // _EVENT_H_