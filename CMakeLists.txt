cmake_minimum_required(VERSION 3.10)

project(synthrush LANGUAGES CXX)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)
add_subdirectory(src/vendor/raylib)

add_executable(${PROJECT_NAME}
  src/main.cpp

  src/game.cpp
  src/game.h

  src/entities/enemy.cpp
  src/entities/enemy.h

  src/scenes/gameScene.cpp
  src/scenes/gameScene.h

  src/entity.h 
  src/scene.h
)

target_link_libraries(${PROJECT_NAME} PRIVATE
  raylib
)

if(CMAKE_C_COMPILER_ID MATCHES "GNU")
  target_link_libraries(${PROJECT_NAME} PRIVATE m)
endif()
