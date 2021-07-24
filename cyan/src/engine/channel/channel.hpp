#pragma once

#include <memory>
#include <vector>
#include <queue>


namespace cyan {
    template <typename T>
    struct Channel;

    /**
     * ChannelSubscriber is the receiving end of a Channel.
     * It can only be created via the associated Channel.
     * @tparam T
     */
    template <typename T>
    struct ChannelSubscriber {
        void publish(T&& data) {
            events.push(data);
        }

        void publish(const T&& data) {
            events.push(data);
        }

        template <typename Fn>
        void consume(Fn f) {
            f(events.front());
            events.pop();
        }

    private:
        ChannelSubscriber() {}
        friend Channel<T>;
        std::queue<T> events;
    };


    /**
     * Channel is the sender side of the event system.
     * Events are published here and pushed into each subscriber's queue.
     * Channels should be created with Channel::new_channel()
     * @tparam T The data contents in each queued event.
     */
    template <typename T>
    struct Channel {    // TODO: This may be garbage collectable
        Channel() : subscribers{} {} // TODO: ideally will be private, but that breaks std::make_shared

        static std::shared_ptr<Channel> new_channel() {
            return std::make_shared<Channel>();
        }

        std::shared_ptr<ChannelSubscriber<T>> new_subscriber() {
            auto subscriber = std::make_shared<ChannelSubscriber<T>>();
            subscribers.push_back(subscriber);
            return subscriber;
        }

        void publish(T&& data) {
            for (auto& sub: subscribers) {
                sub.publish(data);
            }
        }

        void publish(const T&& data) {
            for (auto& sub: subscribers) {
                sub.publish(data);
            }
        }

    private:
        std::vector<std::shared_ptr<ChannelSubscriber<T>>> subscribers;
    };
}