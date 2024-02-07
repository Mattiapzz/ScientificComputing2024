# statdard cmake for c++ 11
cmake_minimum_required(VERSION 3.5)

# project name
project(day3)

# set the standard
set(CMAKE_CXX_STANDARD 11)

# set the source files (all .cc in src folder)
file(GLOB SOURCES "src/*.c*")

# set the include files (all .h in src/include folder)
file(GLOB INCLUDES "src/include/*.h*" "src/*.h*")

# set the executable
add_executable(day3 ${SOURCES} ${INCLUDES})


# set the include directories
target_include_directories(day3 PUBLIC ${INCLUDES})



#
target_link_libraries( day3 ${LIBS} )
