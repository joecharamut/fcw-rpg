#ifndef FCWRPG_THREAD_H
#define FCWRPG_THREAD_H

#include <queue>
#include <mutex>
#include <thread>

template <class T>
class ThreadQueue {
private:
    std::queue<T> queue;
    std::mutex mutex;

public:
    ThreadQueue() = default;

    void push(T elem) {
        mutex.lock();
        queue.push(elem);
        mutex.unlock();
    }

    bool pop(T &elem) {
        bool retValue = false;
        mutex.lock();
        if (!queue.empty()) {
            elem = queue.front();
            queue.pop();
            retValue = true;
        }
        mutex.unlock();
        return retValue;
    }

    bool empty() {
        mutex.lock();
        bool isEmpty = queue.empty();
        mutex.unlock();
        return isEmpty;
    }

};

class ThreadMessage {
public:
    ThreadMessage() = default;
    ThreadMessage(std::string msg) : message(msg) {}
    std::string message;
};

class Thread {
public:
    Thread(void (*loop)(), void (*handler)(ThreadMessage));
    void join();
    void detach();
    void postMessage(ThreadMessage message);

private:
    ThreadQueue<ThreadMessage> messageQueue;
    std::thread this_thread;

    void (*loop)();
    void (*handler)(ThreadMessage);

    void threadLoop();
};

#endif //FCWRPG_THREAD_H
