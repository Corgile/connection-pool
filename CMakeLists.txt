cmake_minimum_required(VERSION 3.26)
project(connection_pool)

set(CMAKE_CXX_STANDARD 17)
set(TARGET connection-pool)

execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/vendor)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH})
include(debug-macro)

include_directories(${PROJECT_SOURCE_DIR}/include)
aux_source_directory(${PROJECT_SOURCE_DIR}/source SOURCE_FILE)
aux_source_directory(${PROJECT_SOURCE_DIR}/include SOURCE_FILE)
aux_source_directory(${PROJECT_SOURCE_DIR}/include/conf SOURCE_FILE)

#add_executable(connection-pool main.cpp ${SOURCE_FILE})
add_library(${TARGET} SHARED ${SOURCE_FILE})
#target_link_libraries(${TARGET} PRIVATE sqlite3)
target_link_libraries(${TARGET} PRIVATE dbg_macro)

target_include_directories(
        ${TARGET}
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include;
        ${CMAKE_CURRENT_SOURCE_DIR}/vendor/dbg-macro;>
        $<INSTALL_INTERFACE:include>
)

install(TARGETS ${TARGET}
        DESTINATION lib
)
add_subdirectory(test)
