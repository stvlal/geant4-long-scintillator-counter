cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(tof_detector)

find_package(Geant4 REQUIRED ui_all vis_all)

include(${Geant4_USE_FILE})

file(GLOB sources ${PROJECT_SOURCE_DIR}/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/*.hh)

file(GLOB MACRO_FILES
	"*.mac"
)

file(GLOB STL_FILES
	"*.stl"
)

file(GLOB C_FILES
	"*.C"
)

file(COPY ${MACRO_FILES} DESTINATION ${PROJECT_BINARY_DIR})
file(COPY ${STL_FILES} DESTINATION ${PROJECT_BINARY_DIR})
file(COPY ${C_FILES} DESTINATION ${PROJECT_BINARY_DIR})

add_executable(tof_Detector tof_Detector.cc ${sources} ${headers})
target_link_libraries(tof_Detector ${Geant4_LIBRARIES})
target_link_libraries(tof_Detector cadmesh)

add_custom_target(tof_detector DEPENDS tof_Detector)
