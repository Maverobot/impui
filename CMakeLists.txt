cmake_minimum_required(VERSION 3.4)
project(
  impui
  VERSION 0.0.1
  LANGUAGES CXX)

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
target_include_directories(
  imgui PUBLIC $<INSTALL_INTERFACE:imgui>
               $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/imgui>)

# Build impui library
add_library(impui SHARED src/canvas.cpp src/simple_plotter.cpp)
add_library(ImPui::impui ALIAS impui)
target_include_directories(
  impui
  PUBLIC $<INSTALL_INTERFACE:include>
         $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
         $<INSTALL_INTERFACE:imgui>
         $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/imgui>)
target_link_libraries(impui PUBLIC imgui)

# Define example target helper function
function(add_example name)
  add_executable(${name} examples/${name}.cpp)
  target_include_directories(${name} PRIVATE include)
  target_link_libraries(${name} impui)
  set_target_properties(${name} PROPERTIES CXX_STANDARD 14)
  set_target_properties(${name} PROPERTIES CXX_EXTENSIONS OFF)
endfunction()

# Build examples
add_example(hello_world)
add_example(simple_plot)

# Install impui
include(GNUInstallDirs)
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/ImPui)

install(
  TARGETS impui imgui
  EXPORT impui-targets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

# This is required so that the exported target has the name ImPui and not impui
set_target_properties(impui PROPERTIES EXPORT_NAME ImPui)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(DIRECTORY imgui/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Export the targets to a script
install(
  EXPORT impui-targets
  FILE ImPuiTargets.cmake
  NAMESPACE ImPui::
  DESTINATION ${INSTALL_CONFIGDIR})

# Create a ConfigVersion.cmake file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/ImPuiConfigVersion.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY AnyNewerVersion)

configure_package_config_file(
  ${CMAKE_CURRENT_LIST_DIR}/cmake/ImPuiConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/ImPuiConfig.cmake INSTALL_DESTINATION
  ${INSTALL_CONFIGDIR})

# Install the config, configversion and custom find modules
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/ImPuiConfig.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/ImPuiConfigVersion.cmake
        DESTINATION ${INSTALL_CONFIGDIR})

# ##############################################################################

export(
  EXPORT impui-targets
  FILE ${CMAKE_CURRENT_BINARY_DIR}/ImPuiTargets.cmake
  NAMESPACE ImPui::)

# Register package in user's package registry
export(PACKAGE ImPui)

# # Add clang-format and clang-tidy check
include(${CMAKE_CURRENT_LIST_DIR}/cmake/ClangTools.cmake OPTIONAL
        RESULT_VARIABLE CLANG_TOOLS)
if(CLANG_TOOLS)
  file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
       ${CMAKE_CURRENT_SOURCE_DIR}/examples/*.cpp)
  file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
       ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)
  add_format_target(${PROJECT_NAME} FILES ${SOURCES} ${HEADERS})
  add_tidy_target(${PROJECT_NAME} FILES ${SOURCES})
endif()
