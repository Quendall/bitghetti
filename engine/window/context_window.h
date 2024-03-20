#pragma once

#include <string>

class context_window {
  public:
    virtual void terminate() = 0;
    virtual void update() = 0;
    virtual void process_input() = 0;
    virtual bool is_open() const = 0;
};