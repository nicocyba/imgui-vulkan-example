#include "frontend/gui/window_manager.h"

// Main code
int main(int, char**) {
    WindowManager windowManager{};

    if (!windowManager.setup()) {
        return 1;
    }

    if (!windowManager.render()) {
        return 1;
    }

    return 0;
}