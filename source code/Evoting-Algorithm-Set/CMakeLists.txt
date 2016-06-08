cmake_minimum_required (VERSION 2.8)
project (Evoting-Auth-Server)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3 -Wall")
######################### pre-handler #########################
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.8)
        message(FATAL_ERROR "GCC version >=4.8 required.")
    endif()
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 3.3)
        message(FATAL_ERROR "Clang version >=3.3 required.")
    endif()
elseif (MSVC14) #TODO: What about other MSVC versions?
else()
    message(WARNING "Your compiler (${CMAKE_CXX_COMPILER_ID}) has not been tested on this project. Only Clang and GCC has been tested. Please report any problems at the project page on GitHub.")
endif()

######################### get libraries #########################
include_directories(.)

#find_package(Threads REQUIRED)
#
#find_package(Boost 1.54.0 COMPONENTS regex system thread coroutine context filesystem date_time REQUIRED)
#include_directories(${Boost_INCLUDE_DIR})
#
#if(APPLE)
#    set(OPENSSL_ROOT_DIR "/usr/local/opt/openssl")
#endif()
#
#find_package(OpenSSL)
#
#if(!OPENSSL_FOUND)
#    message(FATAL_ERROR "openssl not found" ${PROJECT_SOURCE_DIR})
#endif()

#include_directories(${OPENSSL_INCLUDE_DIR})
######################### compiling #########################
add_executable(user_alg src/user/user_alg_test.cpp)
#target_link_libraries(https_server ${Boost_LIBRARIES})
#target_link_libraries(user_alg ${OPENSSL_LIBRARIES})
#target_link_libraries(user_alg ${CMAKE_THREAD_LIBS_INIT})


######################### windows handler #########################
if(MSYS) #TODO: Is MSYS true when MSVC is true?
    target_link_libraries(https_examples ws2_32 wsock32)
endif()
