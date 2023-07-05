cmake_minimum_required(VERSION 3.0)

project(supernova)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(ENABLE_IMAGE "Enables SDL_image support." ON)
option(ENABLE_MIXER "Enables SDL_mixer support." ON)
option(ENABLE_TTF "Enables SDL_ttf support." ON)
option(ENABLE_NET "Enables SDL_net support." ON)

set(CMAKE_INSTALL_PREFIX $ENV{PREFIX})
include(GNUInstallDirs)

set(SRC_PATH src)
set(HEADERS ${SRC_PATH}/engine.h ${SRC_PATH}/core.h ${SRC_PATH}/constants.h)
set(SOURCES ${SRC_PATH}/core.cpp)

set(LIBS SDL2::SDL2)

find_package(SDL2 REQUIRED)

if (ENABLE_IMAGE)
	find_package(SDL2_image REQUIRED)
	list(APPEND LIBS SDL2_image::SDL2_image)
	add_compile_definitions(${PROJECT_NAME} IMAGE_ENABLED)
endif()

if (ENABLE_MIXER)
	find_package(SDL2_mixer REQUIRED)
	list(APPEND LIBS SDL2_mixer::SDL2_mixer)
	add_compile_definitions(${PROJECT_NAME} MIXER_ENABLED)
	list(APPEND SOURCES ${SRC_PATH}/mixer.cpp)
	list(APPEND HEADERS ${SRC_PATH}/mixer.h)
endif()

if (ENABLE_TTF)
	find_package(SDL2_ttf REQUIRED)
	list(APPEND LIBS SDL2_ttf::SDL2_ttf)
	add_compile_definitions(${PROJECT_NAME} TTF_ENABLED)
	list(APPEND SOURCES ${SRC_PATH}/font.cpp)
	list(APPEND HEADERS ${SRC_PATH}/font.h)
endif()

if (ENABLE_NET)
	find_package(SDL2_net REQUIRED)
	list(APPEND LIBS SDL2_net::SDL2_net)
	add_compile_definitions(${PROJECT_NAME} NET_ENABLED)
	list(APPEND SOURCES ${SRC_PATH}/networking.cpp)
	list(APPEND HEADERS ${SRC_PATH}/networking.h)
endif()

add_library(${PROJECT_NAME} SHARED ${HEADERS} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PUBLIC ${LIBS})

# Setting which header files should be supplied with the library
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${HEADERS}")

install(TARGETS ${PROJECT_NAME}
		EXPORT "${PROJECT_NAME}-config"
		RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
		LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}"
		PRIVATE_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}"
		INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}"
)

install(EXPORT "${PROJECT_NAME}-config" DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")

export(TARGETS ${PROJECT_NAME} FILE "${PROJECT_NAME}-exports.cmake")
