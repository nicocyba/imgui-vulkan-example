#pragma once

#include <string>
#include "imgui.h"

static const char* WINDOW_TITLE = "My Title";

class WindowManager {
private:
    ImVec4& clear_color;

public:
    WindowManager(ImVec4& clear_color);

    void render();
};