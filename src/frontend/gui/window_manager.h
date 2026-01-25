#pragma once

#include "imgui.h"

class WindowManager {
private:
    ImVec4& clear_color;

public:
    WindowManager(ImVec4& clear_color);

    void render();
};