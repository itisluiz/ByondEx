cmake_minimum_required(VERSION 3.21)
project("byondex" VERSION 0.1 LANGUAGES CXX)

## Project's base, containing all that's to be inherited
set(PROJECT_BASE ${PROJECT_NAME}-base)
add_library(${PROJECT_BASE} INTERFACE)
target_compile_features(${PROJECT_BASE} INTERFACE cxx_std_17)
target_include_directories(${PROJECT_BASE} INTERFACE "include" "include/gui/imgui" "include/gui/imgui/backend" "include/render/dxsdk")

## QoL definitions
target_compile_definitions(${PROJECT_BASE} INTERFACE PROJECT_VERSION="${PROJECT_VERSION}" BUILD_SHARED_LIBS=$<BOOL:${BUILD_SHARED_LIBS}>)

## Project's dependencies adding and linking
if(NOT TARGET minhook)
	add_subdirectory("lib/minhook")
endif()

if(NOT TARGET acquire)
	add_subdirectory("lib/acquire")
endif()

if(NOT TARGET csys)
	add_subdirectory("lib/csys")
endif()

target_link_directories(${PROJECT_BASE} INTERFACE "lib")
target_link_libraries(${PROJECT_BASE} INTERFACE minhook acquire csys "dbghelp.lib" "directx/d3d9" "directx/d3dx9" "byond/byondcore")

## Source globbing, exclude main file
file(GLOB_RECURSE PROJECT_SOURCES "src/*.cpp" "src/*.cxx" "src/*.cc")
list(FILTER PROJECT_SOURCES EXCLUDE REGEX "src/main.cc$")
target_sources(${PROJECT_BASE} INTERFACE ${PROJECT_SOURCES})

## Project output, end-product
set(PROJECT_TARGET ${PROJECT_NAME})
add_library(${PROJECT_TARGET} SHARED "src/main.cc")
target_link_libraries(${PROJECT_TARGET} PUBLIC ${PROJECT_BASE})
