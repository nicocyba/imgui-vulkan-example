# Require a specific CMake version, here 3.20 for C++23 support
cmake_minimum_required(VERSION 3.25)

# Set compiler BEFORE project()
set(CMAKE_CXX_COMPILER clang++-20)
set(CMAKE_C_COMPILER clang-20)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS on)

# set the project name and version
project(imgui-vulkan-example VERSION 1.0)

# Setup fetching of deps
include(FetchContent)

# Fetch ImGui sources
FetchContent_Declare(
  imgui_src
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG v1.92.4
)
FetchContent_MakeAvailable(imgui_src)

# Create an ImGui library target and alias it to imgui::imgui
add_library(imgui STATIC
  ${imgui_src_SOURCE_DIR}/imgui.cpp
  ${imgui_src_SOURCE_DIR}/imgui_draw.cpp
  ${imgui_src_SOURCE_DIR}/imgui_tables.cpp
  ${imgui_src_SOURCE_DIR}/imgui_widgets.cpp
)
target_include_directories(imgui PUBLIC ${imgui_src_SOURCE_DIR})
add_library(imgui::imgui ALIAS imgui)

# Add the path to header files
include_directories(${CMAKE_SOURCE_DIR}/include)

# Compile executable
add_executable(main 
        src/main.cpp
)

# Add ImGui backends (GLFW + Vulkan)
target_sources(imgui PRIVATE
  ${imgui_src_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
  ${imgui_src_SOURCE_DIR}/backends/imgui_impl_vulkan.cpp
)
target_include_directories(imgui PUBLIC
  ${imgui_src_SOURCE_DIR}
  ${imgui_src_SOURCE_DIR}/backends
)

# Force Wayland-only native platform exposure
# target_compile_definitions(imgui PRIVATE GLFW_EXPOSE_NATIVE_WAYLAND)

# Prefer Wayland over X11
set(GLFW_BUILD_X11 ON CACHE BOOL "Disable X11 backend" FORCE)
set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "Enable Wayland backend" FORCE)

# Fetch GLFW
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.4
)
FetchContent_MakeAvailable(glfw)

# Find Vulkan
find_package(Vulkan REQUIRED)

# Link ImGui backends to GLFW and Vulkan
target_link_libraries(imgui PUBLIC glfw Vulkan::Vulkan)

# Specify the output name with a custom directory
set_target_properties(main PROPERTIES OUTPUT_NAME main)
set_target_properties(main PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
target_link_libraries(main PRIVATE 
        imgui::imgui
)

target_compile_options(main PRIVATE -Wall -Wextra -Wpedantic -Werror -fsanitize=address)
target_link_options(main PRIVATE -fsanitize=address)