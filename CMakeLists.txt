cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(dbus-daemon CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")

set(SRC_FILES_DBUSDAEMON
    dbustest.cpp
)

# import systemd
find_package(PkgConfig REQUIRED)
pkg_check_modules(SYSTEMD REQUIRED IMPORTED_TARGET libsystemd)
include_directories(${SYSTEMD_INCLUDE_DIRS})
link_directories(${SYSTEMD_LIBRARY_DIRS})

add_executable(${PROJECT_NAME} ${SRC_FILES_DBUSDAEMON})

target_link_libraries(${PROJECT_NAME} pthread)
target_link_libraries(${PROJECT_NAME} systemd)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)




