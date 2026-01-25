#include "window_manager.h"

WindowManager::WindowManager(ImVec4& clear_color_) : clear_color(clear_color_) {}

void WindowManager::render() {
    ImGui::ShowDemoWindow();
}