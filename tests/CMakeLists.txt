cmake_minimum_required(VERSION 3.6.0)

project(test)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

# add_compile_definitions(IMAGE_ENABLED)
# add_compile_definitions(MIXER_ENABLED)
# add_compile_definitions(TTF_ENABLED)
add_compile_definitions(NET_ENABLED)

if (WIN32)
	link_libraries(mingw32 SDL3 SDL3_net)
else ()
	link_libraries(SDL3)
endif()

set(SRC_PATH ../src)
# file(GLOB SOURCES ${SRC_PATH}/*.cpp)
file(GLOB SOURCES ${SRC_PATH}/core.cpp ${SRC_PATH}/networking.cpp)

add_executable(main main.cpp ${SOURCES})
