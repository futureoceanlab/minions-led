cmake_minimum_required(VERSION 3.10)

project(minions)
set(CMAKE_CXX_STANDARD 14)


include_directories(include)

file(GLOB SOURCES "src/*.cpp")
add_executable(minions ${SOURCES})

# Link Realtime libraries
target_link_libraries(minions rt)

# Link against wiringPi
find_library(WIRINGPI_LIBRARIES NAMES wiringPi)
target_link_libraries(minions ${WIRINGPI_LIBRARIES})
