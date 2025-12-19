cmake_minimum_required(VERSION 3.16)
project(sprayer_example)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT BUILD_EXAMPLES)
  find_package(isobus REQUIRED)
endif()
find_package(Threads REQUIRED)

add_executable(
  SprayerExample
  main.cpp
  console_logger.cpp
  vt_application.cpp
  vt_application.hpp
  sprayer.cpp
  sprayer.hpp
  object_pool.hpp
  section_control_implement_sim.hpp
  section_control_implement_sim.cpp)
target_link_libraries(
  SprayerExample PRIVATE isobus::Isobus isobus::HardwareIntegration
                        Threads::Threads isobus::Utility)

add_custom_command(
  TARGET SprayerExample
  POST_BUILD
  COMMENT "Copying BasePool.iop to build directory"
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/BasePool.iop
          $<TARGET_FILE_DIR:SprayerExample>/BasePool.iop)