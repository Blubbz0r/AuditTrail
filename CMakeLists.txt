project(AuditTrail)

cmake_minimum_required(VERSION 3.0.2)

# set output directories of executables and libraries
set(OUTPUT_DIR ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR}) #.exe
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR}) # .dll
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR}) # .lib
# in case multiple configurations exist, we will still put the
# executables and librariers into one common folder, instead
# of e.g. Release or Debug subfolders.
# The CMAKE_DEBUG_POSTFIX will take care of separating between the different configs.
foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${OUTPUT_DIR})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${OUTPUT_DIR})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${OUTPUT_DIR})
endforeach(OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES)

set(CMAKE_DEBUG_POSTFIX "d")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
add_definitions(-DUNICODE -D_UNICODE /MP)

# dependency root directory
set(dependency_directory ${PROJECT_SOURCE_DIR}/thirdParty)

# Add logging
set(LOG_DIR ${dependency_directory}/easylogging/9.80)

# Add xml parser
set(XML_DIR ${dependency_directory}/pugixml/1.6)
set(XML_SOURCE_DIR ${XML_DIR}/src)

# Add gmock and gtest
set(GMOCK_ROOT_DIR ${dependency_directory}/googlemock)
set(GMOCK_INCLUDE_DIR ${GMOCK_ROOT_DIR}/include)
set(GMOCK_LIB_DIR ${GMOCK_ROOT_DIR}/lib)
set(GTEST_ROOT_DIR ${dependency_directory}/googletest)
set(GTEST_INCLUDE_DIR ${GTEST_ROOT_DIR}/include)
set(GTEST_LIB_DIR ${GTEST_ROOT_DIR}/lib)

add_subdirectory(src)
add_subdirectory(test)
