#Change this if you need to target a specific CMake version
cmake_minimum_required(VERSION 2.6)
project(pdp)

# Enable debug symbols by default
if(CMAKE_BUILD_TYPE STREQUAL "")
  set(CMAKE_BUILD_TYPE Debug)
endif()
# (you can also set it on the command line: -D CMAKE_BUILD_TYPE=Release)

include_directories("${PROJECT_BINARY_DIR}")

# find sources 
file(GLOB srcs RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*.cpp" "*.h")

# Define executable
set(EXECUTABLE_NAME "pdp")
add_executable(${EXECUTABLE_NAME} ${srcs})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# tell cmake where our custom modules are
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

# Detect and add SFML
#Find any version 2.X of SFML
#See the FindSFML.cmake file for additional details and instructions
find_package(SFML 2 REQUIRED audio)
if(SFML_FOUND)
  include_directories(${SFML_INCLUDE_DIR})
  target_link_libraries(${EXECUTABLE_NAME} ${SFML_AUDIO_LIBRARY})
endif()

# Lua
find_package(Lua51 REQUIRED)
if (LUA51_FOUND)
	include_directories(${LUA_INCLUDE_DIR})
	target_link_libraries(${EXECUTABLE_NAME} ${LUA_LIBRARIES})
endif()

# Irrlicht
find_package(Irrlicht REQUIRED)
include_directories(${IRRLICHT_INCLUDE_DIR})
target_link_libraries(${EXECUTABLE_NAME} ${IRRLICHT_LIBRARIES})

