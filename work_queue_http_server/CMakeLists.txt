cmake_minimum_required(VERSION 3.7)
project(work_queue_http_server)
set(CMAKE_CXX_STANDARD 11)

########################################
if($ENV{CLION_IDE})
    message(STATUS "Using clion ide")
    SET(CPP_NETLIB_PATH "/opt/cpp-netlib")
endif()
########################################

###### cpp-netlib
if(DEFINED CPP_NETLIB_PATH)
    SET(CPP_NETLIB_INCLUDE_DIR ${CPP_NETLIB_PATH}/include)
    SET(CPP_NETLIB_LIBRARY_DIR ${CPP_NETLIB_PATH}/lib)

    INCLUDE_DIRECTORIES(${CPP_NETLIB_INCLUDE_DIR})

    LINK_DIRECTORIES(${CPP_NETLIB_LIBRARY_DIR})
    SET(CPP_NETLIB_LIBRARIES
            libcppnetlib-server-parsers.a
            )

    message(STATUS "CPP_NETLIB_INCLUDE_DIR: ${CPP_NETLIB_INCLUDE_DIR}")
    message(STATUS "CPP_NETLIB_LIBRARY_DIR: ${CPP_NETLIB_LIBRARY_DIR}")
    message(STATUS "CPP_NETLIB_LIBRARIES: ${CPP_NETLIB_LIBRARIES}")
else()
    message(SEND_ERROR "Please set CPP_NETLIB_PATH")
endif()


set(SOURCE_FILES hello_world_async_server_with_work_queue.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME}
        ${CPP_NETLIB_LIBRARIES}
        pthread
        )
