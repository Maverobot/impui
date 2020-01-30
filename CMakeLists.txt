cmake_minimum_required(VERSION 3.4)
project(impui)

# Workaround for: OpenGL_GL_PREFERENCE has not been set to "GLVND" or "LEGACY"
if(POLICY CMP0072)
  cmake_policy(SET CMP0072 OLD)
endif(POLICY CMP0072)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

# Build imgui library
add_library(
  imgui SHARED
  imgui/imgui.cpp imgui/imgui_draw.cpp imgui/imgui_widgets.cpp
  imgui/imgui_demo.cpp imgui/imgui_impl_opengl2.cpp imgui/imgui_impl_glfw.cpp)
target_link_libraries(imgui PUBLIC glfw ${OPENGL_LIBRARIES})
target_include_directories(imgui PUBLIC imgui)

# Build impui library
add_library(impui SHARED src/canvas.cpp)
target_include_directories(impui PUBLIC imgui include)
target_link_libraries(impui PUBLIC imgui)

# Build examples
add_executable(hello_world examples/hello_world.cpp)
target_include_directories(hello_world PRIVATE include)
target_link_libraries(hello_world impui)
set_target_properties(hello_world PROPERTIES CXX_STANDARD 17)
