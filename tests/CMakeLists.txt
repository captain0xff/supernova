cmake_minimum_required(VERSION 3.0.0)

project(test)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (WIN32)
	link_libraries(mingw32 SDL2main SDL2 SDL2_image SDL2_mixer SDL2_ttf SDL2_net)
else ()
	link_libraries(SDL2 SDL2_image SDL2_mixer SDL2_ttf SDL2_net)
endif()

add_executable(main main.cpp ../src/engine.cpp)
