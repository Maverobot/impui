include(FetchContent)
FetchContent_Declare(
  concurrency_utils
  GIT_REPOSITORY https://github.com/Maverobot/concurrency_utils.git
  GIT_TAG v0.1)
FetchContent_GetProperties(concurrency_utils)
if(NOT concurrency_utils_POPULATED)
  FetchContent_Populate(concurrency_utils)
endif()

# Create concurrency_utils library
find_package(Threads REQUIRED)
add_library(concurrency_utils INTERFACE)
target_include_directories(
  concurrency_utils INTERFACE
  $<BUILD_INTERFACE:${concurrency_utils_SOURCE_DIR}/include>)
target_link_libraries(concurrency_utils INTERFACE Threads::Threads)
