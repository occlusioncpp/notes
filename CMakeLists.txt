cmake_minimum_required(VERSION 3.10)

project(notes)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_STANDART_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 COMPONENTS Core Sql REQUIRED)

file(GLOB_RECURSE SOURCES sources/*.cpp sources/*.hpp resources.qrc resources/*.*)

include_directories(sources)

add_executable(notes ${SOURCES})

target_link_libraries(notes Qt::Core Qt::Sql)