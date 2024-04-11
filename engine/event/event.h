#pragma once

#include <vector>
#include <memory>

class event {
  public:
    virtual ~event() = default;
};

class event_listener {
  public:
    ~event_listener() = default;
    virtual void on_event(const event& e) = 0;
};

class event_dispatcher {
  public:
    static event_dispatcher* instance();

    event_dispatcher(const event_dispatcher&) =
        delete;  // Delete copy constructor
    event_dispatcher& operator=(const event_dispatcher&) =
        delete;  // Delete assignment operator

    void add_listener(std::shared_ptr<event_listener> listener);
    void dispatch_event(const event& e);

  protected:
    event_dispatcher() {}  // Private constructor to prevent instantiation
    static event_dispatcher* _instance;
    std::vector<std::shared_ptr<event_listener>> listeners;
};

class mouse_move_event : public event {
  public:
    double x, y, z;
    mouse_move_event(double x, double y, double z) : x(x), y(y), z(z) {}
};

class mouse_down_event : public event {
  public:
    bool down;
    mouse_down_event(bool down) : down(down) {}
};