# Require a specific CMake version, here 3.20 for C++23 support
cmake_minimum_required(VERSION 3.25)

# set the project name and version
project(imgui-vulkan-example VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED on)

# Setup fetching of deps
include(FetchContent)

# FetchContent_Declare(
#   imgui
#   GIT_REPOSITORY git@github.com:ocornut/imgui.git
#   GIT_TAG v1.92.4  # pin a tag or commit
# )
# FetchContent_MakeAvailable(imgui)


# Add the path to header files
include_directories(${CMAKE_SOURCE_DIR}/include)

# set c++ compiler to clang++
set(CMAKE_CXX_COMPILER clang++-20)


set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS on)

# Compile executable
add_executable(main 
        src/main.cpp
)


# Specify the output name with a custom directory
set_target_properties(main PROPERTIES OUTPUT_NAME main)
set_target_properties(main PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
target_link_libraries(main PRIVATE 
        imgui::imgui
)

target_compile_options(main PRIVATE -Wall -Wextra -Wpedantic -Werror -fsanitize=address)
target_link_options(main PRIVATE -fsanitize=address)
