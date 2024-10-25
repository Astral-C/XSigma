cmake_minimum_required(VERSION 3.30)
project(XSigma VERSION 0.1 LANGUAGES CXX)

option(MAKE_TEST "Build test application" OFF)

find_package(LibLZMA)

add_library(XSigma STATIC xsigma.hpp xsigma.cpp)
target_link_libraries(XSigma PRIVATE LibLZMA::LibLZMA)
set_property(TARGET XSigma PROPERTY CXX_STANDARD 26)

if(MAKE_TESTS)

add_executable(XSigmaPatcher test/main.cpp)
target_link_libraries(XSigmaPatcher PRIVATE XSigma)
target_include_directories(XSigmaPatcher PRIVATE ${CMAKE_CURRENT_LIST_DIR})
set_property(TARGET XSigmaPatcher PROPERTY CXX_STANDARD 26)

endif()
