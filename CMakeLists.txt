cmake_minimum_required(VERSION 3.10)

project(synthrush LANGUAGES CXX)

set(CMAKE_BUILD_TYPE Debug)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)
add_subdirectory(src/vendor/raylib)

add_subdirectory(src/vendor/json)

add_executable(${PROJECT_NAME}
  src/main.cpp

  src/random.cpp
  src/random.h

  src/game.cpp
  src/game.h

  src/entities/enemy.cpp
  src/entities/enemy.h

  src/scenes/gameScene.cpp
  src/scenes/gameScene.h

  src/ui/button.h 
  src/ui/button.cpp

  src/entity.h 
  src/scene.h
)

target_link_libraries(${PROJECT_NAME} PRIVATE
  raylib
  nlohmann_json
)

if(CMAKE_C_COMPILER_ID MATCHES "GNU")
  target_link_libraries(${PROJECT_NAME} PRIVATE m)
endif()


set(DATA_DIR "${CMAKE_SOURCE_DIR}/data")
set(OUTPUT_DATA_DIR "${CMAKE_BINARY_DIR}/data")
add_custom_target(copyData ALL
  COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${DATA_DIR}" "${OUTPUT_DATA_DIR}"
)
add_dependencies(${PROJECT_NAME} copyData)

set(ASSETS_DIR "${CMAKE_SOURCE_DIR}/assets")
set(OUTPUT_ASSETS_DIR "${CMAKE_BINARY_DIR}/assets")
add_custom_target(copyAssets ALL
  COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${ASSETS_DIR}" "${OUTPUT_ASSETS_DIR}"
)
add_dependencies(${PROJECT_NAME} copyAssets)
