#include <utility>

#include "Thread.h"

void Thread::threadLoop() {
    loop();
    if (!messageQueue.empty()) {
        ThreadMessage message;
        messageQueue.pop(message);
        handler(message);
    }
}

Thread::Thread(void (*loop)(), void (*handler)(ThreadMessage)) {
    this->loop = loop;
    this->handler = handler;

    this->this_thread = std::thread(&Thread::threadLoop, this);
}

void Thread::join() {
    this_thread.join();
}

void Thread::detach() {
    this_thread.detach();
}

void Thread::postMessage(ThreadMessage message) {
    messageQueue.push(std::move(message));
}
