#pragma once

#include <string>
#include <tuple>

class context_window {
  public:
    virtual void terminate() = 0;
    virtual void update() = 0;
    virtual void process_input() = 0;
    virtual double get_time() = 0;
    virtual std::tuple<double, double> get_cursor_pos() = 0;
    virtual bool is_open() const = 0;
};