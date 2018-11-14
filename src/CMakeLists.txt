cmake_minimum_required(VERSION 2.8)
project(elves CXX)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(elves  main.cpp)
FIND_PACKAGE(PkgConfig REQUIRED)
pkg_check_modules(BSD libbsd)
include_directories(${BSD_INCLUDEDIR})
target_link_libraries(elves ${CONAN_LIBS} ${BSD_LIBDIR}/libbsd.so)
