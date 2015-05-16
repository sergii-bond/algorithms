cmake_minimum_required(VERSION 3.0.0)
project (knapsack1)

include(CheckCXXCompilerFlag)

#option (USE_MYMATH "Use tutorial provided math implementation" ON)

# The version number.
#set (Tutorial_VERSION_MAJOR 1)
#set (Tutorial_VERSION_MINOR 0)
# configure a header file to pass some of the CMake settings
# to the source code
#configure_file (
#      "${PROJECT_SOURCE_DIR}/TutorialConfig.h.in"
#      "${PROJECT_BINARY_DIR}/TutorialConfig.h"
#               )
# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
include_directories("${PROJECT_BINARY_DIR}")

#if (USE_MYMATH)
#  include_directories ("${PROJECT_SOURCE_DIR}/MathFunctions")
#  add_subdirectory (MathFunctions)
#  set (EXTRA_LIBS ${EXTRA_LIBS} MathFunctions)
#endif (USE_MYMATH)

if(UNIX)
    ###!!!set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -g -Ofast -march=native")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -g")

    # if we don't already set the standard for the compiler, detect the
    # best one available and use it
    if(NOT "${CMAKE_CXX_FLAGS}" MATCHES "std=c\\+\\+(0x|11|1y|14)")
        check_cxx_compiler_flag(-std=c++14 HAS_CXX14)
        if(HAS_CXX14)
            message("-- Compiler supports C++14 (using it)")
            set(STDOPT "-std=c++14")
        endif()

        if(NOT STDOPT)
            check_cxx_compiler_flag(-std=c++1y HAS_CXX1Y)
            if(HAS_CXX1Y)
                message("-- Compiler supports C++1y (using it)")
                set(STDOPT "-std=c++1y")
            endif()
        endif()

        if(NOT STDOPT)
            check_cxx_compiler_flag(-std=c++11 HAS_CXX11)
            if(HAS_CXX11)
                message("-- Compiler supports C++11 (using it)")
                set(STDOPT "-std=c++11")
            endif()
        endif()

        if(NOT STDOPT)
            check_cxx_compiler_flag(-std=c++0x HAS_CXX0X)
            if(HAS_CXXOX)
                message("-- Compiler supports C++0x (using it)")
                set(STDOPT "-std=c++0x")
            endif()
        endif()

        if(NOT STDOPT)
            message(FATAL_ERROR
                    "meta requires a compiler with at least C++0x support")
        endif()

        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${STDOPT}")
    endif()
endif()

set (tsp_SRCS tsp)
add_executable(tsp ${tsp_SRCS})
#target_link_libraries (tutorial ${EXTRA_LIBS})
# add the install targets
#set (CMAKE_INSTALL_PREFIX "./")
#install (TARGETS tutorial DESTINATION bin)
#install (FILES "${PROJECT_BINARY_DIR}/TutorialConfig.h" DESTINATION include)
