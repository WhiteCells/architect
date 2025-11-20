#pragma once

#include <functional>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <mutex>
#include <memory>
#include <algorithm>
#include <atomic>
#include "../thread_pool/thread_pool.h"

class EventBus
{
public:
    struct Subscription
    {
        std::atomic<bool> active {true};
        std::function<void()> unsubscribe;

        void cancel()
        {
            if (active.exchange(false)) {
                unsubscribe();
            }
        }
    };

    using SubscriptionPtr = std::shared_ptr<Subscription>;

    EventBus(size_t threadCount = 4) : pool_(threadCount) {}
    ~EventBus() = default;

    template <typename Event, typename Func>
    SubscriptionPtr subscribe(Func &&handler)
    {
        using HandlerFn = std::function<void(const Event &)>;

        auto token = std::make_shared<Subscription>();
        auto idx = std::type_index(typeid(Event));
        size_t id;

        std::lock_guard<std::mutex> lock(mutex_);
        id = next_id_[idx]++;

        handlers_[idx].push_back({id,
                                  [h = std::forward<Func>(handler), token, this](std::shared_ptr<void> raw) {
                                      pool_.addTask([h, token, raw]() {
                                          if (!token->active.load()) {
                                              return;
                                          }
                                          try {
                                              auto eventPtr = static_cast<Event *>(raw.get());
                                              h(*eventPtr);
                                          }
                                          catch (...) {
                                          }
                                      });
                                  },
                                  token});

        token->unsubscribe = [this, idx, id]() {
            std::lock_guard<std::mutex> lock(mutex_);
            auto &vec = handlers_[idx];
            vec.erase(std::remove_if(vec.begin(), vec.end(),
                                     [&](const Handler &h) {
                                         return h.id == id;
                                     }),
                      vec.end());
        };

        return token;
    }

    template <typename Event>
    void publish(const Event &event)
    {
        auto idx = std::type_index(typeid(Event));

        std::vector<Handler *> callList;

        {
            std::lock_guard<std::mutex> lock(mutex_);
            auto it = handlers_.find(idx);
            if (it == handlers_.end()) {
                return;
            }

            for (auto &h : it->second) {
                if (h.token->active.load()) {
                    callList.push_back(&h);
                }
            }
        }

        auto sp = std::make_shared<Event>(event);
        std::shared_ptr<void> raw(sp, sp.get());

        for (auto *h : callList) {
            h->func(raw);
        }
    }

private:
    struct Handler
    {
        size_t id;
        std::function<void(std::shared_ptr<void>)> func;
        SubscriptionPtr token;
    };

    std::mutex mutex_;
    std::unordered_map<std::type_index, std::vector<Handler>> handlers_;
    std::unordered_map<std::type_index, size_t> next_id_;

    ThreadPool pool_;
};
