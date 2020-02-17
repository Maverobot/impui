include(FetchContent)
FetchContent_Declare(
  concurrency_utils
  GIT_REPOSITORY https://github.com/Maverobot/concurrency_utils.git
  GIT_TAG master)
FetchContent_GetProperties(concurrency_utils)
if(NOT concurrency_utils_POPULATED)
  FetchContent_Populate(concurrency_utils)
  message(STATUS ${concurrency_utils_SOURCE_DIR})
  add_subdirectory(${concurrency_utils_SOURCE_DIR})
endif()
