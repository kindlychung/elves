cmake_minimum_required(VERSION 2.8)
project(elves CXX)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(elves  main.cpp)
# set(LIBBSD_HEADERS /usr/include/)
# set(LIBBSD_SO /usr/lib/x86_64-linux-gnu/libbsd.so)
# include_directories(${LIBBSD_HEADERS})
# target_link_libraries(elves ${CONAN_LIBS} ${LIBBSD_SO})
target_link_libraries(elves ${CONAN_LIBS} )
