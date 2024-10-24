set(SRC_DIR ${CMAKE_CURRENT_LIST_DIR})

set(INCLUDE_PATH_DIRS
    ${INCLUDE_PATH_DIRS}
    ${SRC_DIR}
)

set(SRCS_FILES
    ${SRCS_FILES}
    ${SRC_DIR}/src_main.c
)

include (${SRC_DIR}/app/CMakeLists.txt)
include (${SRC_DIR}/bsp/CMakeLists.txt)

