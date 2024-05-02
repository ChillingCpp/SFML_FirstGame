cmake_minimum_required(VERSION 3.10)
set(PROJECT_NAME main)
project(${PROJECT_NAME} VERSION 1.0.0 LANGUAGES CXX)

set(INCROOT ${PROJECT_SOURCE_DIR}/include)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src)

set(SOURCES ${SRCROOT}/VertexDeque.cpp
            ${SRCROOT}/World.cpp
            ${SRCROOT}/OBB.cpp
            ${SRCROOT}/Manifold.cpp
            ${SRCROOT}/Vehicles.cpp
            ${SRCROOT}/Game.cpp
            ${SRCROOT}/Lobby.cpp
            ${SRCROOT}/main.cpp
            ${SRCROOT}/Option.cpp
            ${SRCROOT}/Map.cpp
            ${SRCROOT}/Car.cpp
            ${SRCROOT}/RigidBody.cpp
            ${SRCROOT}/SpringJoint.cpp)

set(HEADERS ${INCROOT}/VertexDeque.hpp
            ${INCROOT}/World.hpp
            ${INCROOT}/OBB.hpp
            ${INCROOT}/Manifold.hpp
            ${INCROOT}/Vehicles.hpp
            ${INCROOT}/Game.hpp
            ${INCROOT}/Lobby.hpp
            ${INCROOT}/Option.hpp
            ${INCROOT}/Map.hpp
            ${INCROOT}/Car.hpp
            ${INCROOT}/RigidBody.hpp
            ${INCROOT}/SpringJoint.hpp)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_include_directories(${PROJECT_NAME} PRIVATE ${INCROOT})
# Detect and add SFML
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
find_package(SFML 2.6 REQUIRED COMPONENTS graphics audio network window system)

target_link_libraries(${PROJECT_NAME} sfml-graphics-d sfml-audio-d sfml-network-d sfml-window-d sfml-system-d)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
if (UNIX)
   target_link_libraries(${PROJECT_NAME} pthread)
endif (UNIX)