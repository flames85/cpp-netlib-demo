cmake_minimum_required(VERSION 3.7)
project(http_client)
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
    #    INCLUDE_DIRECTORIES(${CPP_NETLIB_INCLUDE_DIR}/boost)

    LINK_DIRECTORIES(${CPP_NETLIB_LIBRARY_DIR})
    SET(CPP_NETLIB_LIBRARIES
            libcppnetlib-client-connections.a
            libcppnetlib-uri.a
            )

    message(STATUS "CPP_NETLIB_INCLUDE_DIR: ${CPP_NETLIB_INCLUDE_DIR}")
    message(STATUS "CPP_NETLIB_LIBRARY_DIR: ${CPP_NETLIB_LIBRARY_DIR}")
    message(STATUS "CPP_NETLIB_LIBRARIES: ${CPP_NETLIB_LIBRARIES}")
else()
    message(SEND_ERROR "Please set CPP_NETLIB_PATH")
endif()

##### openssl
find_package(OpenSSL)

set(SOURCE_FILES main.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME}
        ${CPP_NETLIB_LIBRARIES}
        ${OPENSSL_LIBRARIES}
        pthread
        )
