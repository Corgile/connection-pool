include(FetchContent)

if (NOT EXISTS ${CMAKE_SOURCE_DIR}/vendor/dbg-macro)
    message(STATUS "拉取 sharkdp/dbg-macro")
    # 拉取 dbg-macro 子模块并切换到指定分支
    execute_process(COMMAND proxychains git clone -b master https://github.com/sharkdp/dbg-macro.git ${PROJECT_SOURCE_DIR}/vendor/dbg-macro)
endif ()

#FetchContent_Declare(dbg_macro GIT_REPOSITORY https://github.com/sharkdp/dbg-macro.git GIT_TAG master)
FetchContent_Declare(dbg_macro SOURCE_DIR ${PROJECT_SOURCE_DIR}/vendor/dbg-macro)
FetchContent_MakeAvailable(dbg_macro)

add_compile_definitions(DBG_MACRO_NO_WARNING)