#include "window_manager.h"

WindowManager::WindowManager(ImVec4& clear_color_) : clear_color(clear_color_) {}

void WindowManager::render() {
    ImGuiIO& io = ImGui::GetIO();
    // ImGui::PushItemWidth(-ImGui::GetContentRegionAvail().x * 0.5f);

    // Calculate dimensions
    ImVec2 viewportPos = ImGui::GetMainViewport()->Pos;
    ImVec2 viewportSize = ImGui::GetMainViewport()->Size;
    float halfWidth = viewportSize.x / 2.0f;
    float halfHeight = viewportSize.y / 2.0f;

    // Window 1 - Top-left
    ImGui::SetNextWindowPos(ImVec2(viewportPos.x, viewportPos.y), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(halfWidth, halfHeight), ImGuiCond_Always);
    ImGui::Begin("Nico1", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);
    ImGui::Text("This is some useful text."); 
    ImGui::Text("%.1f FPS", io.Framerate);
    ImGui::End();

    // Window 2 - Top-right (2D Canvas)
    ImGui::SetNextWindowPos(ImVec2(viewportPos.x + halfWidth, viewportPos.y), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(halfWidth, halfHeight), ImGuiCond_Always);
    ImGui::Begin("Nico2", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);
    
    // 2D Canvas Content
    ImVec2 canvas_pos = ImGui::GetCursorScreenPos();
    ImVec2 canvas_size = ImGui::GetContentRegionAvail();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    
    // Draw a background rectangle
    draw_list->AddRectFilled(canvas_pos, ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y), 
                            ImGui::GetColorU32(ImVec4(0.1f, 0.1f, 0.1f, 1.0f)));
    
    // Draw a grid pattern
    ImU32 grid_color = ImGui::GetColorU32(ImVec4(0.3f, 0.3f, 0.3f, 0.5f));
    float grid_step = 50.0f;
    for (float x = 0; x < canvas_size.x; x += grid_step) {
        draw_list->AddLine(ImVec2(canvas_pos.x + x, canvas_pos.y), 
                          ImVec2(canvas_pos.x + x, canvas_pos.y + canvas_size.y), grid_color, 1.0f);
    }
    for (float y = 0; y < canvas_size.y; y += grid_step) {
        draw_list->AddLine(ImVec2(canvas_pos.x, canvas_pos.y + y), 
                          ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + y), grid_color, 1.0f);
    }
    
    // Draw some 2D shapes
    ImVec2 center = ImVec2(canvas_pos.x + canvas_size.x * 0.5f, canvas_pos.y + canvas_size.y * 0.5f);
    draw_list->AddCircleFilled(center, 30.0f, ImGui::GetColorU32(ImVec4(0.5f, 0.8f, 1.0f, 0.8f)));
    draw_list->AddRect(ImVec2(center.x - 50, center.y - 50), ImVec2(center.x + 50, center.y + 50), 
                       ImGui::GetColorU32(ImVec4(1.0f, 0.5f, 0.5f, 0.8f)), 0.0f, 0, 2.0f);
    
    ImGui::Dummy(canvas_size);
    ImGui::End();

    // Window 3 - Bottom-left
    ImGui::SetNextWindowPos(ImVec2(viewportPos.x, viewportPos.y + halfHeight), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(halfWidth, halfHeight), ImGuiCond_Always);
    ImGui::Begin("Nico3", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);
    ImGui::Text("This is some useful text."); 
    if (ImGui::Button("Close Me")) {
        
    }
    ImGui::End();

    // Window 4 - Bottom-right (3D Canvas)
    ImGui::SetNextWindowPos(ImVec2(viewportPos.x + halfWidth, viewportPos.y + halfHeight), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(halfWidth, halfHeight), ImGuiCond_Always);
    ImGui::Begin("Nico4", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);
    
    // 3D Canvas Content
    ImVec2 canvas_pos_3d = ImGui::GetCursorScreenPos();
    ImVec2 canvas_size_3d = ImGui::GetContentRegionAvail();
    ImDrawList* draw_list_3d = ImGui::GetWindowDrawList();
    
    // Draw a darker background for 3D view
    draw_list_3d->AddRectFilled(canvas_pos_3d, ImVec2(canvas_pos_3d.x + canvas_size_3d.x, canvas_pos_3d.y + canvas_size_3d.y), 
                               ImGui::GetColorU32(ImVec4(0.05f, 0.05f, 0.1f, 1.0f)));
    
    // Draw a border for the 3D view
    draw_list_3d->AddRect(canvas_pos_3d, ImVec2(canvas_pos_3d.x + canvas_size_3d.x, canvas_pos_3d.y + canvas_size_3d.y), 
                         ImGui::GetColorU32(ImVec4(0.5f, 0.5f, 0.5f, 0.5f)), 0.0f, 0, 1.0f);
    
    // Draw a simple 3D-like cube projection
    ImVec2 center_3d = ImVec2(canvas_pos_3d.x + canvas_size_3d.x * 0.5f, canvas_pos_3d.y + canvas_size_3d.y * 0.5f);
    float cube_size = 40.0f;
    
    // Front face (blue)
    draw_list_3d->AddQuad(
        ImVec2(center_3d.x - cube_size, center_3d.y - cube_size),
        ImVec2(center_3d.x + cube_size, center_3d.y - cube_size),
        ImVec2(center_3d.x + cube_size, center_3d.y + cube_size),
        ImVec2(center_3d.x - cube_size, center_3d.y + cube_size),
        ImGui::GetColorU32(ImVec4(0.2f, 0.5f, 1.0f, 0.8f)), 2.0f);
    
    // Right face (green)
    draw_list_3d->AddTriangleFilled(
        ImVec2(center_3d.x + cube_size, center_3d.y - cube_size),
        ImVec2(center_3d.x + cube_size + 20, center_3d.y - 20),
        ImVec2(center_3d.x + cube_size + 20, center_3d.y + cube_size + 20),
        ImGui::GetColorU32(ImVec4(0.5f, 1.0f, 0.5f, 0.6f)));
    
    // Top face (yellow)
    draw_list_3d->AddTriangleFilled(
        ImVec2(center_3d.x - cube_size, center_3d.y - cube_size),
        ImVec2(center_3d.x + cube_size, center_3d.y - cube_size),
        ImVec2(center_3d.x, center_3d.y - cube_size - 30),
        ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 0.5f, 0.8f)));
    
    ImGui::Dummy(canvas_size_3d);
    ImGui::End();

    ImGui::ShowDemoWindow();
}