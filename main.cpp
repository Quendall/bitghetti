#include <chrono>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <vector>

#include "engine/window/window_factory.h"

using std::thread, std::vector;

const int SCR_HEIGHT = 600;
const int SCR_WIDTH = 800;

const double MS_PER_UPDATE = 1000.0 / 50.0;
const double MS_PER_RENDER = 1000.0 / 60.0;

int main() {
    context_window* window = create_window(SCR_WIDTH, SCR_HEIGHT, "bitghetti");

    // Main rendering loop
    // -------------------

    double prev_time = glfwGetTime(), fps_timer = prev_time;
    double lag = 0.0;
    int frames = 0, updates = 0;

    while (window->is_open()) {
        double curr_time = glfwGetTime();
        double delta_time = curr_time - prev_time;
        prev_time = curr_time;
        lag += delta_time;

        window->process_input();

        while (lag >= MS_PER_UPDATE) {
            // Update game simulation
            
            lag -= MS_PER_UPDATE;
        }

        // Render here (lag / MS_PER_UPDATE)

        window->update();
        frames++;

        if (glfwGetTime() - fps_timer > 1.0) {
            fps_timer++;
            std::cout << "\rFPS: " << frames << "\tUPS: " << updates
                      << std::flush;
            frames = 0;
        }
    }

    window->terminate();
    return EXIT_SUCCESS;
}