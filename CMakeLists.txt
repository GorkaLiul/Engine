cmake_minimum_required(VERSION 3.20.0)
project(Engine)

#std config

	set(CMAKE_CXX_STANDARD 20)
	set(CMAKE_CXX_STANDARD_REQUIRED OFF)
	set(CMAKE_CXX_EXTENSIONS OFF)


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/glad/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/imgui)

include_directories($CMAKE_CURRENT_SOURCE_DIR}/src)
set(SOURCES
	src/main.cpp
	src/window.cpp
  src/shader.cpp
)

#executable target
add_executable(${PROJECT_NAME} ${SOURCES})


# Find and link GLFW
find_package(glfw3 REQUIRED)
message(STATUS "Found GLFW VERSION: ${GLFW3_VERSION}")
if(glfw3_FOUND)
	message(STATUS "Found GLFW ${GLFW3_VERSION}")
else()
	message(FATAL_ERROR "GLFW not found")
endif()

#  glad as a subdirectory
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/glad)


#imGui: 

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/imgui)

# Include directories for glad
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/glad/include)

# Link libraries (glfw  galad imGui)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw glad imgui)

#FOR OPEN GL:
#  find_package(OpenGL REQUIRED COMPONENTS OpenGL)
# target_link_libraries(${PROJECT_NAME} OpenGL::OpenGL)


#imGui program:

# Add executable target for ImGui program
#add_executable(${PROJECT_NAME}_imgui imguitest.cpp)
#target_include_directories(${PROJECT_NAME}_imgui PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/glad/include ${CMAKE_CURRENT_SOURCE_DIR}/imgui)
#target_link_libraries(${PROJECT_NAME}_imgui PRIVATE glfw glad imgui)

