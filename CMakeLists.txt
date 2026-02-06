cmake_minimum_required(VERSION 3.20)

project(FMRTCore VERSION 2.2 LANGUAGES CXX)

option(FMRT_BUILD_TESTS "Build FMRT tests" ON)

file(GLOB_RECURSE FMRT_HEADERS "include/**/*.hpp")
file(GLOB FMRT_SOURCES "src/*.cpp")

# ДОБАВЛЯЕМ МОСТ В FMRT STATIC LIB
file(GLOB FMRT_BRIDGE "bridge/*.cpp")
list(APPEND FMRT_SOURCES ${FMRT_BRIDGE})

add_library(fmrt_core STATIC ${FMRT_HEADERS} ${FMRT_SOURCES})

target_include_directories(fmrt_core
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/fmrt
        ${CMAKE_CURRENT_SOURCE_DIR}/include/internal
        ${CMAKE_CURRENT_SOURCE_DIR}/config
        ${CMAKE_CURRENT_SOURCE_DIR}/bridge
)

target_compile_features(fmrt_core PUBLIC cxx_std_17)

if(FMRT_BUILD_TESTS)
    enable_testing()

    file(GLOB TESTS "tests/*.cpp")
    add_executable(fmrt_tests ${TESTS})

    target_include_directories(fmrt_tests PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/fmrt
        ${CMAKE_CURRENT_SOURCE_DIR}/include/internal
        ${CMAKE_CURRENT_SOURCE_DIR}/config
        ${CMAKE_CURRENT_SOURCE_DIR}/bridge
    )

    # ЛИНКУЕМ МОСТ + ЯДРО
    target_link_libraries(fmrt_tests PRIVATE fmrt_core)

    add_test(NAME fmrt_tests_all COMMAND fmrt_tests)
endif()
