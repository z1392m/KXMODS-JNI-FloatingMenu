cmake_minimum_required(VERSION 3.22)
project(kovakmenu LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(FetchContent)
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.91.9b
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(imgui)

add_library(kovak SHARED
    src/KovakMenu.cpp
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

target_include_directories(kovak PRIVATE
    src
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)

find_library(log-lib log)
find_library(android-lib android)
find_library(gles-lib GLESv3)

target_link_libraries(kovak PRIVATE ${log-lib} ${android-lib} ${gles-lib})

target_compile_definitions(kovak PRIVATE IMGUI_IMPL_OPENGL_ES3)
target_compile_options(kovak PRIVATE -Wall -Wextra -Wno-unused-parameter)

set_target_properties(kovak PROPERTIES
    OUTPUT_NAME "kovak"
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN YES
)
