#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <thread>

#include "engine/window/window_factory.h"

const int SCR_HEIGHT = 600;
const int SCR_WIDTH = 800;

const double MS_PER_UPDATE = 1000.0 / 50.0;
const double MAX_FPS = 60.0;
const double FRAME_TARGET = 1.0 / MAX_FPS;

int main() {
    context_window* window = create_window(SCR_WIDTH, SCR_HEIGHT, "bitghetti");

    // Main rendering loop
    // -------------------

    double prev_time = glfwGetTime(), fps_timer = prev_time;
    double lag = 0.0;
    int frames = 0;
    int pframes = 0;
    // int updates = 0;

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

        // Update FPS
        if (glfwGetTime() - fps_timer > 1.0) {
            fps_timer += 1.0;
            pframes = frames;
            frames = 0;
        }
        // Print FPS and Cursor Pos
        // auto [xpos, ypos] = window->get_cursor_pos();
        // std::cout << "\r" << std::setfill(' ') << std::setw(10) << std::left
        //           << "FPS: " << pframes << std::setw(10) << "\txpos: " << xpos
        //           << std::setw(10) << "\typos: " << ypos << std::flush;

        // Sleep if render finishes early
        // double frame_start = curr_time;
        // double frame_end = glfwGetTime();
        // double frame_duration = frame_end - frame_start;

        // if (frame_duration < FRAME_TARGET) {
        //     double sleep_duration = FRAME_TARGET - frame_duration;
        //     std::this_thread::sleep_for(
        //         std::chrono::milliseconds(int(sleep_duration * 1000)));
        // }
    }

    window->terminate();
    return EXIT_SUCCESS;
}