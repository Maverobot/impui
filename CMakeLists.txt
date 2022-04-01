cmake_minimum_required(VERSION 3.11)
project(
  impui
  VERSION 0.1
  LANGUAGES CXX)

# Workaround for: OpenGL_GL_PREFERENCE has not been set to "GLVND" or "LEGACY"
if(POLICY CMP0072)
  cmake_policy(SET CMP0072 OLD)
endif(POLICY CMP0072)

option(EXAMPLES "compile with examples" off)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Eigen3 REQUIRED)

include(cmake/FetchConcurrencyUtils.cmake)
include(cmake/TargetLinkLibrariesSystem.cmake)
include(cmake/StaticAnalysis.cmake)

enable_clang_tidy()
enable_cppcheck()

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
  imgui SYSTEM PUBLIC $<INSTALL_INTERFACE:imgui>
                      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/imgui>)
set_target_properties(imgui PROPERTIES CXX_CLANG_TIDY "" CXX_CPPCHECK "")

# Build impui library
add_library(impui INTERFACE)
add_library(ImPui::impui ALIAS impui)
target_include_directories(
  impui INTERFACE $<INSTALL_INTERFACE:include>
                  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

target_include_directories(
  impui SYSTEM
  INTERFACE $<INSTALL_INTERFACE:imgui>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/imgui>
            $<BUILD_INTERFACE:${EIGEN3_INCLUDE_DIR}>)

target_link_libraries_system(impui INTERFACE imgui concurrency_utils)
target_compile_features(impui INTERFACE cxx_std_14)

# Define example target helper function
function(add_example name)
  add_executable(${name} examples/${name}.cpp)
  target_include_directories(${name} PRIVATE include)
  target_link_libraries(${name} impui)
  target_compile_features(${name} PRIVATE cxx_std_14)
endfunction()

# Build examples
if(EXAMPLES)
  add_example(hello_world)
  add_example(simple_plot)
  add_example(simple_realtime_plot)
endif()

# ##############################################################################

# Add clang-format and clang-tidy check
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

# ##############################################################################

# Install impui
include(GNUInstallDirs)
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/ImPui)

install(
  TARGETS impui imgui concurrency_utils
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
  ${CMAKE_CURRENT_BINARY_DIR}/ImPuiConfig.cmake
  INSTALL_DESTINATION ${INSTALL_CONFIGDIR})

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
