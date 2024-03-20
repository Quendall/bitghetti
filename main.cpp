#include <cstdlib>

#include "engine/window/window_factory.h"

const int SCR_HEIGHT = 600;
const int SCR_WIDTH = 800;

int main() {
    context_window* window = create_window(SCR_WIDTH, SCR_HEIGHT, "bitghetti");

    // Main rendering loop
    while (window->is_open()) {
        window->process_input();

        // Render here

        window->update();
    }

    window->terminate();
    return EXIT_SUCCESS;
}