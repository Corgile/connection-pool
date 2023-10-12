enable_testing()

include(/home/brian/Projects/connection-pool/cmake/debug-macro.cmake)
# 添加测试用例的可执行文件
add_executable(sqlite-test
        main.cpp
        sqlite_conn.hpp
        student.hpp
)

# 链接您的库到测试用例
target_link_libraries(sqlite-test PRIVATE connection-pool)
target_link_libraries(sqlite-test PRIVATE sqlite3)

# 添加测试
add_test(NAME sqlite-test COMMAND sqlite-test)
