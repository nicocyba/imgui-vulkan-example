#pragma once

#include <string>
#include "imgui.h"

static const char* WINDOW_TITLE = "My Title";
static const int WINDOW_WIDTH = 1280;
static const int WINDOW_HEIGHT = 640;

class WindowManager {
private:
    ImVec4& clear_color;

public:
    WindowManager(ImVec4& clear_color);

    void render();
};