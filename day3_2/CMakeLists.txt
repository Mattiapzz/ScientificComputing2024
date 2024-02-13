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


# add the executable (all file *.cc) from the folder src_test
file(GLOB SOURCES_TEST "src_test/*.c*")

# add the executable (all file *.cc) from the folder src_test
file(GLOB INCLUDES_TEST "src_test/*.h*")

# add the executable
foreach( testsourcefile ${SOURCES_TEST} )
  get_filename_component(testname ${testsourcefile} NAME_WE)
  add_executable( ${testname} ${testsourcefile} ${SOURCES} ${INCLUDES} ${INCLUDES_TEST} )
  target_include_directories(${testname} PUBLIC src/include src_test)
  # target_link_libraries(${testname} gtest gtest_main pthread)
  add_test(NAME ${testname} COMMAND ${testname} )
endforeach()

