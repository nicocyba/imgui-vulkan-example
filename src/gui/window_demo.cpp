#include "imgui.h"
#include <vector>

void gui_window_simple(bool& show_demo_window, bool& show_another_window, ImVec4& clear_color) {
    ImGuiIO& io = ImGui::GetIO();
    static float f = 0.0f;
    static int counter = 0;

    ImGui::Begin("Hello, world123!"); // Create a window called "Hello, world!"
                                      // and append into it.

    ImGui::Text("This is some useful text."); // Display some text (you can
                                              // use a format strings too)
    ImGui::Checkbox("Demo Window", &show_demo_window); // Edit bools storing our window open/close state
    ImGui::Checkbox("Another Window", &show_another_window);

    ImGui::SliderFloat("float", &f, 0.0f, 1.0f); // Edit 1 float using a slider from 0.0f to 1.0f
    ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

    if (ImGui::Button("Button")) { // Buttons return true when clicked (most
                                   // widgets return true when edited/activated)
        counter++;
    }
    ImGui::SameLine();
    ImGui::Text("counter = %d", counter);

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    ImGui::End();
}


void gui_window_another(bool& show_another_window) {
    ImGui::Begin("Another Window", &show_another_window); // Pass a pointer to our bool variable (the
                                                          // window will have a closing button that will
                                                          // clear the bool when clicked)
    ImGui::Text("Hello from another window!");
    if (ImGui::Button("Close Me")) {
        show_another_window = false;
    }
    ImGui::End();
}

void gui_window_container() {
    ImGui::Begin("Map Container");

    // Define a structure for map objects
    struct MapObject {
        ImVec2 pos;
        ImVec2 vel;
        int type;   // 0=ship, 1=portal, 2=npc, 3=resource
        ImU32 color;
        const char* label;
    };

    static std::vector<MapObject> objects;
    static ImVec2 map_size(600, 400);

    // Initialize objects on first call
    if (objects.empty()) {
        objects = {
            {{50, 50}, {1.5f, 0.8f}, 0, ImGui::GetColorU32(ImVec4(1.0f, 0.0f, 0.0f, 1.0f)), "Ship 1"},
            {{150, 100}, {-1.0f, 1.2f}, 1, ImGui::GetColorU32(ImVec4(0.0f, 1.0f, 0.0f, 1.0f)), "Portal 1"},
            {{200, 200}, {0.5f, -0.7f}, 2, ImGui::GetColorU32(ImVec4(0.0f, 0.0f, 1.0f, 1.0f)), "NPC 1"},
            {{300, 150}, {-0.8f, 0.3f}, 3, ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 0.0f, 1.0f)), "Resource 1"},
            {{400, 300}, {0.3f, -0.5f}, 0, ImGui::GetColorU32(ImVec4(1.0f, 0.5f, 0.0f, 1.0f)), "Ship 2"},
            {{500, 50}, {-0.6f, 0.9f}, 3, ImGui::GetColorU32(ImVec4(0.5f, 1.0f, 0.5f, 1.0f)), "Resource 2"},
        };
    }

    // Update positions
    for (auto& obj : objects) {
        obj.pos.x += obj.vel.x;
        obj.pos.y += obj.vel.y;

        // Wrap around edges
        if (obj.pos.x < 0) obj.pos.x += map_size.x;
        if (obj.pos.x > map_size.x) obj.pos.x -= map_size.x;
        if (obj.pos.y < 0) obj.pos.y += map_size.y;
        if (obj.pos.y > map_size.y) obj.pos.y -= map_size.y;
    }

    // Get canvas position and draw list
    ImVec2 canvas_pos = ImGui::GetCursorScreenPos();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    // Draw map background border
    ImVec2 map_end = ImVec2(canvas_pos.x + map_size.x, canvas_pos.y + map_size.y);
    draw_list->AddRectFilled(canvas_pos, map_end, ImGui::GetColorU32(ImVec4(0.1f, 0.1f, 0.15f, 1.0f)));
    draw_list->AddRect(canvas_pos, map_end, ImGui::GetColorU32(ImVec4(0.5f, 0.5f, 0.5f, 1.0f)), 1.0f);

    // Draw grid
    ImU32 grid_color = ImGui::GetColorU32(ImVec4(0.2f, 0.2f, 0.2f, 0.5f));
    for (int x = 0; x <= (int)map_size.x; x += 50) {
        draw_list->AddLine(ImVec2(canvas_pos.x + x, canvas_pos.y),
                          ImVec2(canvas_pos.x + x, map_end.y), grid_color);
    }
    for (int y = 0; y <= (int)map_size.y; y += 50) {
        draw_list->AddLine(ImVec2(canvas_pos.x, canvas_pos.y + y),
                          ImVec2(map_end.x, canvas_pos.y + y), grid_color);
    }

    // Draw objects
    for (const auto& obj : objects) {
        ImVec2 screen_pos = ImVec2(canvas_pos.x + obj.pos.x, canvas_pos.y + obj.pos.y);
        float size = 6.0f;

        switch (obj.type) {
            case 0: // Ship - circle
                draw_list->AddCircleFilled(screen_pos, size, obj.color, 12);
                draw_list->AddCircle(screen_pos, size + 1.0f, ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 0.5f)), 12);
                break;

            case 1: // Portal - square
                draw_list->AddRectFilled(ImVec2(screen_pos.x - size, screen_pos.y - size),
                                        ImVec2(screen_pos.x + size, screen_pos.y + size), obj.color);
                draw_list->AddRect(ImVec2(screen_pos.x - size, screen_pos.y - size),
                                  ImVec2(screen_pos.x + size, screen_pos.y + size),
                                  ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 0.5f)));
                break;

            case 2: // NPC - triangle (pointing up)
                draw_list->AddTriangleFilled(ImVec2(screen_pos.x, screen_pos.y - size),
                                           ImVec2(screen_pos.x - size, screen_pos.y + size),
                                           ImVec2(screen_pos.x + size, screen_pos.y + size), obj.color);
                draw_list->AddTriangle(ImVec2(screen_pos.x, screen_pos.y - size),
                                      ImVec2(screen_pos.x - size, screen_pos.y + size),
                                      ImVec2(screen_pos.x + size, screen_pos.y + size),
                                      ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 0.5f)));
                break;

            case 3: // Resource - diamond (4-pointed star)
                draw_list->AddTriangleFilled(ImVec2(screen_pos.x, screen_pos.y - size),
                                           ImVec2(screen_pos.x - size, screen_pos.y),
                                           ImVec2(screen_pos.x, screen_pos.y + size), obj.color);
                draw_list->AddTriangleFilled(ImVec2(screen_pos.x, screen_pos.y - size),
                                           ImVec2(screen_pos.x + size, screen_pos.y),
                                           ImVec2(screen_pos.x, screen_pos.y + size), obj.color);
                break;
        }
    }

    // Reserve space for the canvas
    ImGui::Dummy(map_size);

    // Display object list and information
    ImGui::Separator();
    ImGui::Text("Objects on map:");
    for (const auto& obj : objects) {
        const char* type_name = "";
        switch (obj.type) {
            case 0: type_name = "Ship"; break;
            case 1: type_name = "Portal"; break;
            case 2: type_name = "NPC"; break;
            case 3: type_name = "Resource"; break;
        }
        ImGui::BulletText("%s - Pos: (%.0f, %.0f) | Vel: (%.1f, %.1f)", 
                         type_name, obj.pos.x, obj.pos.y, obj.vel.x, obj.vel.y);
    }

    ImGui::End();
}