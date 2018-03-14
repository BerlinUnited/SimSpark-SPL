# - Use C++11 standard
#
# Author:
#   Kevin M. Godby <kevin@godby.org>
#
# Hedayat: modified to support gnu++11/gnu++0x instead
# Note that Microsoft Visual C++ compiler enables C++11 by default

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR CMAKE_COMPILER_IS_GNUCXX)
    include(CheckCXXCompilerFlag)
    check_cxx_compiler_flag(-std=gnu++11 SUPPORTS_STD_GNUXX11)
    check_cxx_compiler_flag(-std=gnu++0x SUPPORTS_STD_GNUXX0X)
    if(SUPPORTS_STD_GNUXX11)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
    elseif(SUPPORTS_STD_GNUXX0X)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
    else()
        message(ERROR "Compiler does not support -std=gnu++11 or -std=gnu++0x.")
    endif()
endif()

