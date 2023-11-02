cmake_minimum_required(VERSION 3.26)
project(connection-pool)

set(CMAKE_CXX_STANDARD 17)
set(TARGET connection-pool)

include_directories(${PROJECT_SOURCE_DIR}/include)
aux_source_directory(${PROJECT_SOURCE_DIR}/source SOURCE_FILE)
aux_source_directory(${PROJECT_SOURCE_DIR}/include SOURCE_FILE)
aux_source_directory(${PROJECT_SOURCE_DIR}/include/conf SOURCE_FILE)

add_library(${TARGET} SHARED ${SOURCE_FILE})

target_include_directories(
        ${TARGET}
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
add_subdirectory(test)
