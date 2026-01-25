load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "imgui",
    srcs = [
        "imgui_demo.cpp",
        "imgui.cpp",
        "imgui_draw.cpp",
        "imgui_tables.cpp",
        "imgui_widgets.cpp",
        "backends/imgui_impl_glfw.cpp",
        "backends/imgui_impl_vulkan.cpp",
    ],
    hdrs = [
        "imconfig.h",
        "imgui.h",
        "imgui_internal.h",
        "imstb_rectpack.h",
        "imstb_textedit.h",
        "imstb_truetype.h",
        "backends/imgui_impl_glfw.h",
        "backends/imgui_impl_vulkan.h",
    ],
    includes = [
        ".",
        "backends",
    ],
    visibility = ["//visibility:public"],
)
