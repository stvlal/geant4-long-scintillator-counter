cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(tof_detector)

find_package(Geant4 REQUIRED ui_all vis_all)

include(${Geant4_USE_FILE})

file(GLOB sources ${PROJECT_SOURCE_DIR}/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/*.hh)

add_executable(tof_Detector tof_Detector.cc ${sources} ${headers})
target_link_libraries(tof_Detector ${Geant4_LIBRARIES})

add_custom_target(tof_detector DEPENDS tof_Detector)
