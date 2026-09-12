cmake_minimum_required(VERSION 3.16)
project(sdrpp_tests)

add_executable(sdrpp_tests test_main.cpp)

target_link_libraries(sdrpp_tests PRIVATE sdrpp_core)

target_include_directories(sdrpp_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/core/src
    ${CMAKE_SOURCE_DIR}/core/src/dsp
)

target_compile_options(sdrpp_tests PRIVATE ${SDRPP_COMPILER_FLAGS})

add_test(NAME sdrpp_unit_tests COMMAND sdrpp_tests)
