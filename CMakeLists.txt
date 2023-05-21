cmake_minimum_required(VERSION 3.20)
project(OpenGL_3D_Scene VERSION "0.0.1" DESCRIPTION "OpenGL 3D Scene" LANGUAGES C CXX)

################# Build Type ###############
if (CMAKE_BUILD_TYPE MATCHES Release)
    set(CMAKE_CXX_FLAGS "-O3 -Wall -Wextra -Wpedantic")
    add_compile_definitions(RELEASE=1)
elseif (CMAKE_BUILD_TYPE MATCHES Debug)
    set(CMAKE_CXX_FLAGS "-g -Wall -Wextra -Wpedantic")
    add_compile_definitions(_DEBUG=1)
endif ()
############################################

################ GLFW ###################
add_subdirectory(${CMAKE_SOURCE_DIR}/lib/glfw EXCLUDE_FROM_ALL)
##########################################

################ GLAD ###################
add_subdirectory(${CMAKE_SOURCE_DIR}/lib/GLAD EXCLUDE_FROM_ALL)
##########################################

################### GLM ##################
add_subdirectory(${CMAKE_SOURCE_DIR}/lib/glm EXCLUDE_FROM_ALL)
##########################################

################# imgui ##################
add_library(imgui STATIC EXCLUDE_FROM_ALL
        ${CMAKE_SOURCE_DIR}/lib/imgui/imgui.cpp
        ${CMAKE_SOURCE_DIR}/lib/imgui/imgui.h
        ${CMAKE_SOURCE_DIR}/lib/imgui/imgui_demo.cpp
        ${CMAKE_SOURCE_DIR}/lib/imgui/imgui_draw.cpp
        ${CMAKE_SOURCE_DIR}/lib/imgui/imgui_internal.h
        ${CMAKE_SOURCE_DIR}/lib/imgui/imgui_widgets.cpp
        ${CMAKE_SOURCE_DIR}/lib/imgui/imstb_rectpack.h
        ${CMAKE_SOURCE_DIR}/lib/imgui/imstb_textedit.h
        ${CMAKE_SOURCE_DIR}/lib/imgui/imstb_truetype.h
        ${CMAKE_SOURCE_DIR}/lib/imgui/imconfig.h
        ${CMAKE_SOURCE_DIR}/lib/imgui/imgui_tables.cpp
        ${CMAKE_SOURCE_DIR}/lib/imgui/backends/imgui_impl_glfw.h
        ${CMAKE_SOURCE_DIR}/lib/imgui/backends/imgui_impl_glfw.cpp
        ${CMAKE_SOURCE_DIR}/lib/imgui/backends/imgui_impl_opengl3.h
        ${CMAKE_SOURCE_DIR}/lib/imgui/backends/imgui_impl_opengl3.cpp
)
include_directories(imgui ${CMAKE_SOURCE_DIR}/lib/imgui ${CMAKE_SOURCE_DIR}/lib/imgui/backends)
# link glfw to imgui and link everything to the ${PROJECT_NAME} app
target_link_libraries(imgui PRIVATE glfw)
##########################################

################## Portable File Dialogs #################
add_subdirectory(${CMAKE_SOURCE_DIR}/lib/portable-file-dialogs)
include_directories(${CMAKE_SOURCE_DIR}/lib/portable-file-dialogs)
#########################################################

################## stb_image #################
add_library(stb_image STATIC EXCLUDE_FROM_ALL ${CMAKE_SOURCE_DIR}/lib/stb/src/stb.c)
target_include_directories(stb_image PUBLIC ${CMAKE_SOURCE_DIR}/lib/stb/include)
############################################



################### Main Project ######################
file(GLOB_RECURSE SOURCES ${CMAKE_SOURCE_DIR}/src/**.cpp ${CMAKE_SOURCE_DIR}/src/**.hpp)
add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_precompile_headers(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src/pch.hpp)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw glm glad imgui stb_image)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
target_compile_definitions(${PROJECT_NAME} PRIVATE
        # OpenGL Version
        OPENGL_VERSION_MAJOR=4
        OPENGL_VERSION_MINOR=6
        # Resources directory
        RESOURCES_DIR="${CMAKE_SOURCE_DIR}/res"
        SHADERS_DIR="${CMAKE_SOURCE_DIR}/res/shaders"
        TEXTURES_DIR="${CMAKE_SOURCE_DIR}/res/textures"
        MODELS_DIR="${CMAKE_SOURCE_DIR}/res/models"
)
# set a symlink to the assets dir
add_custom_command(
        TARGET ${PROJECT_NAME} PRE_BUILD COMMAND
        ${CMAKE_COMMAND} -E create_symlink
        ${CMAKE_SOURCE_DIR}/res ${CMAKE_CURRENT_BINARY_DIR}/res
)
