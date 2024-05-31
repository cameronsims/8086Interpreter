# CMakeList.txt : CMake project for 8086Interpreter, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project(8086Interpreter)

#######################################################################

# CPP Implementation Files
file(GLOB_RECURSE SRC_FILES "${PROJECT_SOURCE_DIR}/src/*.cpp")

# CPP Header Files
file(GLOB_RECURSE INC_FILES "${PROJECT_SOURCE_DIR}/include/*.h")

#######################################################################

# Add source to this project's executable.
add_executable(8086Int ${SRC_FILES} ${INC_FILES})

# Include Files
target_include_directories(8086Int PUBLIC ${INC_FILES})



if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET 8086Int PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
