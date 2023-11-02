enable_testing()

include(FetchContent)
set(VENDOR_PATH ${CMAKE_CURRENT_SOURCE_DIR}/vendor)
message(STATUS ${VENDOR_PATH})

if (NOT EXISTS ${VENDOR_PATH}/dbg-macro)
    message(STATUS "拉取 sharkdp/dbg-macro")
    # 拉取 dbg-macro 子模块并切换到指定分支
    execute_process(COMMAND proxychains git clone -b master https://github.com/sharkdp/dbg-macro.git ${VENDOR_PATH}/dbg-macro)
endif ()

#FetchContent_Declare(dbg_macro GIT_REPOSITORY https://github.com/sharkdp/dbg-macro.git GIT_TAG master)
FetchContent_Declare(dbg_macro SOURCE_DIR ${VENDOR_PATH}/dbg-macro)
FetchContent_MakeAvailable(dbg_macro)

add_compile_definitions(DBG_MACRO_NO_WARNING)
include_directories(vendor/dbg-macro)

# 添加测试用例的可执行文件
add_executable(sqlite-test
        main.cpp
        sqlite_conn.hpp
        student.hpp
)

# 链接您的库到测试用例
target_link_libraries(sqlite-test PRIVATE connection-pool)
target_link_libraries(sqlite-test PRIVATE sqlite3)
target_link_libraries(sqlite-test PRIVATE dbg_macro)

# 添加测试
add_test(NAME sqlite-test COMMAND sqlite-test)
