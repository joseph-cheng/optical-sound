cmake_minimum_required(VERSION 3.13)

project(optical-sound C ASM)

set (BUILD_SHARED OFF FORCE)
set (BUILD_STATIC ON FORCE)

set(default_build_type "Debug")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# Include raylib
option(CUSTOMIZE_BUILD ON)
option(SUPPORT_MODULE_RAUDIO OFF)
option(SUPPORT_MODULE_RMODELS OFF)
option(SUPPORT_COMPRESSION_API OFF)

set(OPTICAL_SOUND_SRC
  src/main.c
  )

file(GLOB TRIPOD_SRC deps/tripod/src/tp/*.c deps/tripod/src/tp/*.S)


add_executable(optical-sound ${OPTICAL_SOUND_SRC} ${TRIPOD_SRC})
target_include_directories(optical-sound PRIVATE deps/tripod/src)
target_link_libraries(optical-sound PRIVATE raylib m pulse-simple c)

set_property(TARGET optical-sound PROPERTY C_STANDARD 99)

# Enable all warnings
if (MSVC)
  target_compile_options(optical-sound PRIVATE /W4 /WX)
else()
  target_compile_options(optical-sound PRIVATE -Wall -Wextra -pedantic -Werror -Wno-missing-braces -Wno-unused-function -fno-stack-protector)
  target_link_options(optical-sound PRIVATE -nostdlib)
endif()

