#pragma once

#include <string>
#include "imgui.h"

#include "imgui_helper.h"

static const char* WINDOW_TITLE = "My Title";
static const int WINDOW_WIDTH = 1280;
static const int WINDOW_HEIGHT = 640;

class WindowManager {
private:
    // vulkan and glfw variables
    GLFWwindow* window = nullptr;
    ImGui_ImplVulkanH_Window* wd = nullptr;
    VkResult err = VkResult::VK_NOT_READY;

    // imgui variables
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    bool render_content();

public:
    WindowManager() = default;
    ~WindowManager() = default;


    bool setup() {
        glfwSetErrorCallback(glfw_error_callback);
        if (!glfwInit()) {
            return false;
        }

        // Create window with Vulkan context
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        float main_scale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor()); // Valid on GLFW 3.3+ only
        window = glfwCreateWindow((int)(WINDOW_WIDTH * main_scale), (int)(WINDOW_HEIGHT * main_scale), WINDOW_TITLE, nullptr, nullptr);
        if (!glfwVulkanSupported()) {
            printf("GLFW: Vulkan Not Supported\n");
            return false;
        }

        ImVector<const char*> extensions;
        uint32_t extensions_count = 0;
        const char** glfw_extensions = glfwGetRequiredInstanceExtensions(&extensions_count);
        for (uint32_t i = 0; i < extensions_count; i++) {
            extensions.push_back(glfw_extensions[i]);
        }
        SetupVulkan(extensions);

        // Create Window Surface
        VkSurfaceKHR surface;
        err = glfwCreateWindowSurface(g_Instance, window, g_Allocator, &surface);
        check_vk_result(err);

        // Create Framebuffers
        int w, h;
        glfwGetFramebufferSize(window, &w, &h);
        wd = &g_MainWindowData;
        SetupVulkanWindow(wd, surface, w, h);

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // enable docking

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        // ImGui::StyleColorsLight();

        // Setup scaling
        ImGuiStyle& style = ImGui::GetStyle();
        style.ScaleAllSizes(main_scale); // Bake a fixed style scale. (until we have a solution for
                                        // dynamic style scaling, changing this requires resetting
                                        // Style + calling this again)
        style.FontScaleDpi = main_scale; // Set initial font scale. (using io.ConfigDpiScaleFonts=true
                                        // makes this unnecessary. We leave both here for
                                        // documentation purpose)

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForVulkan(window, true);
        ImGui_ImplVulkan_InitInfo init_info = {};
        // init_info.ApiVersion = VK_API_VERSION_1_3;              // Pass in your
        // value of VkApplicationInfo::apiVersion, otherwise will default to header
        // version.
        init_info.Instance = g_Instance;
        init_info.PhysicalDevice = g_PhysicalDevice;
        init_info.Device = g_Device;
        init_info.QueueFamily = g_QueueFamily;
        init_info.Queue = g_Queue;
        init_info.PipelineCache = g_PipelineCache;
        init_info.DescriptorPool = g_DescriptorPool;
        init_info.MinImageCount = g_MinImageCount;
        init_info.ImageCount = wd->ImageCount;
        init_info.Allocator = g_Allocator;
        init_info.PipelineInfoMain.RenderPass = wd->RenderPass;
        init_info.PipelineInfoMain.Subpass = 0;
        init_info.PipelineInfoMain.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
        init_info.CheckVkResultFn = check_vk_result;
        ImGui_ImplVulkan_Init(&init_info);

        // Load Fonts
        // - If no fonts are loaded, dear imgui will use the default font. You can
        // also load multiple fonts and use ImGui::PushFont()/PopFont() to select
        // them.
        // - AddFontFromFileTTF() will return the ImFont* so you can store it if you
        // need to select the font among multiple.
        // - If the file cannot be loaded, the function will return a nullptr. Please
        // handle those errors in your application (e.g. use an assertion, or display
        // an error and quit).
        // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype
        // for higher quality font rendering.
        // - Read 'docs/FONTS.md' for more instructions and details. If you like the
        // default font but want it to scale better, consider using the 'ProggyVector'
        // from the same author!
        // - Remember that in C/C++ if you want to include a backslash \ in a string
        // literal you need to write a double backslash \\ !
        // style.FontSizeBase = 20.0f;
        // io.Fonts->AddFontDefault();
        // io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf");
        // io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf");
        // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf");
        // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf");
        // ImFont* font =
        // io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf");
        // IM_ASSERT(font != nullptr);

        // Our state
        // bool show_demo_window = true;
        // bool show_another_window = false;

        return true;
    }

    
    bool render() {
        while (!glfwWindowShouldClose(window)) {
            // Poll and handle events (inputs, window resize, etc.)
            // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to
            // tell if dear imgui wants to use your inputs.
            // - When io.WantCaptureMouse is true, do not dispatch mouse input data to
            // your main application, or clear/overwrite your copy of the mouse data.
            // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input
            // data to your main application, or clear/overwrite your copy of the
            // keyboard data. Generally you may always pass all inputs to dear imgui,
            // and hide them from your application based on those two flags.
            glfwPollEvents();

            // Resize swap chain?
            int fb_width, fb_height;
            glfwGetFramebufferSize(window, &fb_width, &fb_height);
            if (fb_width > 0 && fb_height > 0 && (g_SwapChainRebuild || g_MainWindowData.Width != fb_width || g_MainWindowData.Height != fb_height)) {
                ImGui_ImplVulkan_SetMinImageCount(g_MinImageCount);
                ImGui_ImplVulkanH_CreateOrResizeWindow(
                    g_Instance, g_PhysicalDevice, g_Device, wd, g_QueueFamily, g_Allocator, fb_width, fb_height, g_MinImageCount, 0);
                g_MainWindowData.FrameIndex = 0;
                g_SwapChainRebuild = false;
            }
            if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0) {
                ImGui_ImplGlfw_Sleep(10);
                continue;
            }

            // Start the Dear ImGui frame
            ImGui_ImplVulkan_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            /*
            BUILD CUSTOM GUI
            */
            render_content();


            // Rendering
            ImGui::Render();
            ImDrawData* draw_data = ImGui::GetDrawData();
            const bool is_minimized = (draw_data->DisplaySize.x <= 0.0f || draw_data->DisplaySize.y <= 0.0f);
            if (!is_minimized) {
                wd->ClearValue.color.float32[0] = clear_color.x * clear_color.w;
                wd->ClearValue.color.float32[1] = clear_color.y * clear_color.w;
                wd->ClearValue.color.float32[2] = clear_color.z * clear_color.w;
                wd->ClearValue.color.float32[3] = clear_color.w;
                FrameRender(wd, draw_data);
                FramePresent(wd);
            }
        }

        // Cleanup
        err = vkDeviceWaitIdle(g_Device);
        check_vk_result(err);
        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        CleanupVulkanWindow();
        CleanupVulkan();

        glfwDestroyWindow(window);
        glfwTerminate();
        return true;
    }

};