cmake_minimum_required(VERSION 3.0)

project(supernova)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCES src/engine.cpp)
set(HEADERS src/engine.h)

set(CMAKE_INSTALL_PREFIX $ENV{PREFIX})
include(GNUInstallDirs)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_net REQUIRED)

add_library(${PROJECT_NAME} SHARED ${HEADERS} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PUBLIC
	SDL2::SDL2
	SDL2_image::SDL2_image
	SDL2_mixer::SDL2_mixer
	SDL2_ttf::SDL2_ttf
	SDL2_net::SDL2_net
)

# Setting which header files should be supplied with the library
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${HEADERS}")

install(TARGETS ${PROJECT_NAME}
        EXPORT "${PROJECT_NAME}-config"
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}"
        PRIVATE_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}"
        INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}")

install(EXPORT "${PROJECT_NAME}-config" DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")

export(TARGETS ${PROJECT_NAME} FILE "${PROJECT_NAME}-exports.cmake")
