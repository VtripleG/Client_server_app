cmake_minimum_required(VERSION 3.14)

project(server LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Network)
find_package(Qt6 REQUIRED COMPONENTS Sql)
include_directories(${PROJECT_SOURCE_DIR})





add_executable(server
  main.cpp
  Server.h
  User.h
  databasecontroller.h
  Server.cpp
  User.cpp
  databasecontroller.cpp
)


target_link_libraries(server Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(server Qt${QT_VERSION_MAJOR}::Network)
target_link_libraries(server Qt${QT_VERSION_MAJOR}::Sql)


install(TARGETS server
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})


