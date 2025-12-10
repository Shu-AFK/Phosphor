cmake_minimum_required(VERSION 3.15)
project(phosphor)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/external/imgui)

set(IMGUI_SOURCES
  ${IMGUI_DIR}/imgui.cpp
  ${IMGUI_DIR}/imgui_draw.cpp
  ${IMGUI_DIR}/imgui_widgets.cpp
  ${IMGUI_DIR}/imgui_tables.cpp
  # ${IMGUI_DIR}/imgui_demo.cpp # uncomment for demo window
  ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
  ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

file(GLOB_RECURSE SRC_FILES ${CMAKE_SOURCE_DIR}/src/*.cpp)

add_library(glad external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)

set(NFD_PORTAL ON CACHE BOOL "" FORCE)
add_subdirectory(external/nativefiledialog-extended)

add_executable(phosphor
  ${SRC_FILES}
  ${IMGUI_SOURCES}
)

target_compile_options(phosphor PRIVATE
  $<$<CONFIG:Release>:-O2>
)

target_include_directories(phosphor PRIVATE
  ${CMAKE_SOURCE_DIR}/src
  ${IMGUI_DIR}
  ${IMGUI_DIR}/backends
  ${CMAKE_SOURCE_DIR}/external/stb
  ${GLFW_INCLUDE_DIRS}
)

target_link_libraries(phosphor PRIVATE
  glad
  nfd
  ${GLFW_LIBRARIES}
  OpenGL::GL
)
