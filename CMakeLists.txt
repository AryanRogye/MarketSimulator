cmake_minimum_required(VERSION 3.21)

project(MarketSim LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

# OpenGL
find_package(OpenGL REQUIRED)

# GLFW
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)

FetchContent_MakeAvailable(glfw)

# ImGui
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.92.4
)

FetchContent_MakeAvailable(imgui)

add_library(imgui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp

    # GLFW + OpenGL backend
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui PUBLIC
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)

target_link_libraries(imgui PUBLIC
    glfw
    OpenGL::GL
)

# ImPlot
FetchContent_Declare(
    implot
    GIT_REPOSITORY https://github.com/epezent/implot.git
    GIT_TAG v0.17
)

FetchContent_MakeAvailable(implot)

add_library(implot STATIC
    ${implot_SOURCE_DIR}/implot.cpp
    ${implot_SOURCE_DIR}/implot_items.cpp
)

target_include_directories(implot PUBLIC
    ${implot_SOURCE_DIR}
)

target_link_libraries(implot PUBLIC
    imgui
)

# MarketSim
add_executable(MarketSim
    src/main.cpp
    src/ui.cpp
    src/MarketState.cpp
    src/Stock.cpp
    src/MarketSim.cpp
    src/User.cpp
    src/Order.cpp
)

target_link_libraries(MarketSim PRIVATE
    glfw
    imgui
    implot
)

target_include_directories(MarketSim
    SYSTEM PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_options(MarketSim PRIVATE
    -Wall
    -Wextra
    -Wconversion
    -Wsign-conversion
    -pedantic
    -Wdeprecated-declarations
)
