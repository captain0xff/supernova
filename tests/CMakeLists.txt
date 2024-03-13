cmake_minimum_required(VERSION 3.6.0)

project(test)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_definitions(IMAGE_ENABLED)
add_compile_definitions(MIXER_ENABLED)
add_compile_definitions(TTF_ENABLED)
add_compile_definitions(NET_ENABLED)

if (WIN32)
	link_libraries(mingw32 SDL2main SDL2 SDL2_image SDL2_mixer SDL2_ttf SDL2_net)
else ()
	link_libraries(SDL2 SDL2_image SDL2_mixer SDL2_ttf SDL2_net)
endif()

set(SRC_PATH ../src)
file(GLOB SOURCES ${SRC_PATH}/*.cpp)

add_executable(main main.cpp ${SOURCES})
