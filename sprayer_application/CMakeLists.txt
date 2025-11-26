cmake_minimum_required(VERSION 3.16)
project(sprayer_application)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT BUILD_EXAMPLES)
  find_package(isobus REQUIRED)
endif()
find_package(Threads REQUIRED)

# Set the output directory for the executable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/sprayer)

add_executable(
  sprayer
  main.cpp
  console_logger.cpp
  vt_application.cpp
  vt_application.hpp
  seeder.cpp
  seeder.hpp
  object_pool.hpp
  section_control_implement_sim.hpp
  section_control_implement_sim.cpp)
target_link_libraries(
  sprayer PRIVATE isobus::Isobus isobus::HardwareIntegration
                        Threads::Threads isobus::Utility)

add_custom_command(
  TARGET sprayer
  POST_BUILD
  COMMENT "Copying BasePool.iop to build directory"
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/BasePool.iop
          $<TARGET_FILE_DIR:sprayer>/BasePool.iop)
