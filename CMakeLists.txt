cmake_minimum_required(VERSION 3.10)

project(supernova3)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# The satellite libs are not built by default as they are not ready yet
option(ENABLE_IMAGE "Enables SDL_image support." ON)
option(ENABLE_MIXER "Enables SDL_mixer support." ON)
option(ENABLE_TTF "Enables SDL_ttf support." ON)
option(ENABLE_NET "Enables SDL_net support." ON)

if (NOT ANDROID)
	set(CMAKE_INSTALL_PREFIX $ENV{PREFIX})
	include(GNUInstallDirs)
endif()

set(SRC_PATH src)
set(HEADERS
	${SRC_PATH}/engine.h
	${SRC_PATH}/core.h
	${SRC_PATH}/constants.h
	${SRC_PATH}/enums.h
	${SRC_PATH}/print.h
	${SRC_PATH}/logging.h
)
set(SOURCES ${SRC_PATH}/core.cpp)

set(LIBS SDL3::SDL3)

if (NOT ANDROID)
	find_package(SDL3 REQUIRED)
endif()

if (ENABLE_IMAGE)
	if (NOT ANDROID)
		find_package(SDL3_image REQUIRED)
	endif()
	list(APPEND LIBS SDL3_image::SDL3_image)
	add_compile_definitions(${PROJECT_NAME} IMAGE_ENABLED)
	list(APPEND SOURCES ${SRC_PATH}/graphics.cpp)
	list(APPEND HEADERS ${SRC_PATH}/graphics.h)
endif()

if (ENABLE_MIXER)
	if (NOT ANDROID)
		find_package(SDL3_mixer REQUIRED)
	endif()
	list(APPEND LIBS SDL3_mixer::SDL3_mixer)
	add_compile_definitions(${PROJECT_NAME} MIXER_ENABLED)
	list(APPEND SOURCES ${SRC_PATH}/mixer.cpp)
	list(APPEND HEADERS ${SRC_PATH}/mixer.h)
endif()

if (ENABLE_TTF)
	if (NOT ANDROID)
		find_package(SDL3_ttf REQUIRED)
	endif()
	list(APPEND LIBS SDL3_ttf::SDL3_ttf)
	add_compile_definitions(${PROJECT_NAME} TTF_ENABLED)
	list(APPEND SOURCES ${SRC_PATH}/font.cpp)
	list(APPEND HEADERS ${SRC_PATH}/font.h)
endif()

if (ENABLE_NET)
	if (NOT ANDROID)
		find_package(SDL3_net REQUIRED)
	endif()
	list(APPEND LIBS SDL3_net::SDL3_net)
	add_compile_definitions(${PROJECT_NAME} NET_ENABLED)
	list(APPEND SOURCES ${SRC_PATH}/networking.cpp)
	list(APPEND HEADERS ${SRC_PATH}/networking.h)
endif()

add_library(${PROJECT_NAME} SHARED ${HEADERS} ${SOURCES})

target_compile_options(${PROJECT_NAME} PUBLIC -Wall -Wextra -Wpedantic)

target_link_libraries(${PROJECT_NAME} PUBLIC ${LIBS})

# Setting which header files should be supplied with the library
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${HEADERS}")

if (NOT ANDROID)
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
endif()
