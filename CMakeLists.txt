project(AuditTrail)

cmake_minimum_required(VERSION 3.0.2)

if (EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
elseif (EXISTS ${CMAKE_SOURCE_DIR}/conanbuildinfo.cmake)
    include(${CMAKE_SOURCE_DIR}/conanbuildinfo.cmake)
else ()
    error("Run conan install first!")
endif()

CONAN_BASIC_SETUP()

set(CMAKE_DEBUG_POSTFIX "d")

# dependency root directory
set(dependency_directory ${PROJECT_SOURCE_DIR}/thirdParty)

# Add logging
set(LOG_DIR ${dependency_directory}/easylogging/9.80)

add_subdirectory(src)
add_subdirectory(test)
