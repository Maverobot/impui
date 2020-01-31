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
  imgui/imgui.cpp
  imgui/imgui_draw.cpp
  imgui/imgui_widgets.cpp
  imgui/imgui_demo.cpp
  imgui/imgui_impl_opengl2.cpp
  imgui/imgui_impl_glfw.cpp
  imgui/imgui_plot.cpp)
target_link_libraries(imgui PUBLIC glfw ${OPENGL_LIBRARIES})
target_include_directories(imgui PUBLIC imgui)

# Build impui library
add_library(impui SHARED src/canvas.cpp)
target_include_directories(impui PUBLIC imgui include)
target_link_libraries(impui PUBLIC imgui)

function(add_example name)
  add_executable(${name} examples/${name}.cpp)
  target_include_directories(${name} PRIVATE include)
  target_link_libraries(${name} impui)
  set_target_properties(${name} PROPERTIES CXX_STANDARD 17)
endfunction()

# Build examples
add_example(hello_world)
add_example(simple_plot)

# Add clang-format and clang-tidy check
include(${CMAKE_CURRENT_LIST_DIR}/cmake/ClangTools.cmake OPTIONAL
        RESULT_VARIABLE CLANG_TOOLS)
if(CLANG_TOOLS)
  file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
  file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
       ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)
  add_format_target(${PROJECT_NAME} FILES ${SOURCES} ${HEADERS})
  add_tidy_target(${PROJECT_NAME} FILES ${SOURCES})
endif()
