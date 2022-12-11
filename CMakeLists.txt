cmake_minimum_required(VERSION 3.0.0)
project(opengl-app VERSION 0.1.0)

#set(CMAKE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
#include(CTest)
#enable_testing()
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

# I was able to use this find_package for glfw after installing libglfw3-dev. If not installed the LinkGLFW cmake will have to be used
find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)

file(STRINGS assets/glsl/plainVertexShader.vs VERTEX_SHADER)

# Add glad as a library to the project. Header and source files exist in project
# The add_library will create a static library for glad called libglad.a in build folder
add_library(glad ${CMAKE_CURRENT_SOURCE_DIR}/external/include/glad.h ${CMAKE_CURRENT_SOURCE_DIR}/external/src/glad.c)

# Create an executable based on the triangle.cpp source file
add_executable(opengl-app ${SOURCE_DIR}/triangle.cpp ${SOURCE_DIR}/ShaderPipeline.cpp)

target_include_directories(glad PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/external/include) # This is to let cmake know what header files to use to build the library

# Tell project where to find header files then, link libraries glad and open gl to the project. glfw was already previously linked
target_include_directories(opengl-app PRIVATE ${OPENGL_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/external/include)

# For some reason I had to add ${CMAKE_DL_LIBS}
# Got this from following link: https://stackoverflow.com/questions/56841870/how-to-fix-undefined-reference-to-symbol-dlcloseglibc-2-2-5-from-glad-c
target_link_libraries(opengl-app PRIVATE ${OPENGL_LIBRARIES} glfw glad ${CMAKE_DL_LIBS})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

# Enable C++17
set_target_properties(opengl-app PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO)

