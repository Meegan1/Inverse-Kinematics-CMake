cmake_minimum_required(VERSION 3.15)
project(Assignment_1)

set(CMAKE_CXX_STANDARD 14)

add_executable(Assignment_1 main.cpp Animation/BVH.cpp Animation/BVH.h Rendering/Engine.cpp Rendering/Engine.h Rendering/Camera.cpp Rendering/Camera.h Rendering/Input.h)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
target_link_libraries(${PROJECT_NAME} Qt5::Widgets)

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})

find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${GLUT_LIBRARY})

find_package(GLM REQUIRED)
include_directories(${GLM_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${GLM_LIBRARY_DIRS})