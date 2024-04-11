#include "event.h"

event_dispatcher* event_dispatcher::instance() {
    if (_instance == nullptr) {
        _instance = new event_dispatcher();
    }
    return _instance;
}

event_dispatcher* event_dispatcher::_instance = nullptr;

void event_dispatcher::add_listener(std::shared_ptr<event_listener> listener) {
    listeners.push_back(listener);
}

void event_dispatcher::dispatch_event(const event& e) {
    for (auto listener : listeners) {
        listener->on_event(e);
    }
}