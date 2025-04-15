cmake_minimum_required(VERSION 3.15)
project(NodeEditor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find OpenCV, ImGui, GLFW from vcpkg
find_package(OpenCV REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)


# Include paths
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}/third_party/imgui"
    "${CMAKE_CURRENT_SOURCE_DIR}/third_party/imgui_backends"
    "${CMAKE_CURRENT_SOURCE_DIR}/third_party/imgui_node_editor"
)

# ImGui core sources
set(IMGUI_SOURCES
    third_party/imgui/imgui.cpp
    third_party/imgui/imgui_draw.cpp
    third_party/imgui/imgui_widgets.cpp
    third_party/imgui/imgui_tables.cpp
    third_party/imgui/backends/imgui_impl_glfw.cpp
    third_party/imgui/backends/imgui_impl_opengl3.cpp
)

# imgui-node-editor sources
set(NODE_EDITOR_SOURCES
    third_party/imgui_node_editor/imgui_node_editor.cpp
    third_party/imgui_node_editor/imgui_node_editor_api.cpp
    third_party/imgui_node_editor/imgui_canvas.cpp
    third_party/imgui_node_editor/crude_json.cpp
)

# Gather project sources (core/nodes/main)
file(GLOB_RECURSE SRC_FILES "src/*.cpp" "src/*.h")

# Define executable
add_executable(NodeEditor
    ${SRC_FILES}
    ${IMGUI_SOURCES}
    ${NODE_EDITOR_SOURCES}
)

# Link libraries
target_link_libraries(NodeEditor PRIVATE ${OpenCV_LIBS} imgui::imgui glfw opengl32)
target_link_libraries(NodeEditor PRIVATE glad::glad)