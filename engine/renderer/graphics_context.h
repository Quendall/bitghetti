#pragma once

class graphics_context {
    virtual void init() = 0;
    virtual void swap_buffers() = 0;
};