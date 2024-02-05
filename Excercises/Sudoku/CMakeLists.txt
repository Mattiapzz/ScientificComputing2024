# statdard cmake for c++ 11
cmake_minimum_required(VERSION 3.5)

# project name
project(Sudoku)

# set the standard
set(CMAKE_CXX_STANDARD 11)

# set the source files (all .cc in src folder)
file(GLOB SOURCES "src/*.c*")

# set the include files (all .h in src/include folder)
file(GLOB INCLUDES "src/include/*.h*" "src/*.h*")

# set a third parties directory
set(THIRD_PARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_parties)
set(THIRD_PARTY_DIR_USR ${THIRD_PARTY_DIR}/usr )


include_directories(${THIRD_PARTY_DIR_USR}/include )
link_directories(${THIRD_PARTY_DIR_USR}/lib)

# set the executable
add_executable(sudoku ${SOURCES} ${INCLUDES})

# set the include directories
target_include_directories(sudoku PUBLIC ${INCLUDES})

# set tyhe LIBS
set(LIBS libfmt.a)

#
target_link_libraries( sudoku ${LIBS} )



# ████████╗██╗  ██╗██╗██████╗ ██████╗     ██████╗  █████╗ ██████╗ ████████╗██╗███████╗███████╗
# ╚══██╔══╝██║  ██║██║██╔══██╗██╔══██╗    ██╔══██╗██╔══██╗██╔══██╗╚══██╔══╝██║██╔════╝██╔════╝
#    ██║   ███████║██║██████╔╝██║  ██║    ██████╔╝███████║██████╔╝   ██║   ██║█████╗  ███████╗
#    ██║   ██╔══██║██║██╔══██╗██║  ██║    ██╔═══╝ ██╔══██║██╔══██╗   ██║   ██║██╔══╝  ╚════██║
#    ██║   ██║  ██║██║██║  ██║██████╔╝    ██║     ██║  ██║██║  ██║   ██║   ██║███████╗███████║
#    ╚═╝   ╚═╝  ╚═╝╚═╝╚═╝  ╚═╝╚═════╝     ╚═╝     ╚═╝  ╚═╝╚═╝  ╚═╝   ╚═╝   ╚═╝╚══════╝╚══════╝
                                                                                            

# if not present, download and compile the third parties
if(NOT EXISTS ${THIRD_PARTY_DIR_USR})
  message(STATUS "Downloading and compiling third parties...")
  include(external.cmake)
  add_dependencies(sudoku fmt)
endif()