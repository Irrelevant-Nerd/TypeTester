cmake_minimum_required(VERSION 3.16)

project(TypeTester LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Collect every .c file in the project root and every .h file in include/.
# CONFIGURE_DEPENDS makes the build re-check the folders, so newly added
# files get picked up on the next build without re-running CMake by hand.
file(GLOB SRC_FILES    CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.c")
file(GLOB HEADER_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")

add_executable(${PROJECT_NAME} ${SRC_FILES} ${HEADER_FILES})

# Lets you write #include "scan_input.h" from any source file
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")

# Helpful warnings
if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()
