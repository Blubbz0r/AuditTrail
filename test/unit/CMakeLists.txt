set(CURRENT_PROJECT_NAME UnitTests)

include_directories(
    ${GMOCK_INCLUDE_DIR}
    ${GTEST_INCLUDE_DIR}
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/src/AuditTrail
    ${PROJECT_SOURCE_DIR}/src/Common
)

add_executable(${CURRENT_PROJECT_NAME}  main.cpp
                                        LogNoop.h
                                        ApplicationActivityTests.cpp
                                        AuditLogUsedTests.cpp
                                        BeginTransferringInstancesTests.cpp
                                        DataExportTests.cpp
                                        DataImportTests.cpp
                                        Tests.cpp
                                        WriterMock.h)

add_dependencies(${CURRENT_PROJECT_NAME} AuditTrail Common)

target_link_libraries(${CURRENT_PROJECT_NAME}
                        debug ${GMOCK_LIB_DIR}/gmock${CMAKE_DEBUG_POSTFIX}${CMAKE_FIND_LIBRARY_SUFFIXES}
                        debug ${OUTPUT_DIR}/AuditTrail${CMAKE_DEBUG_POSTFIX}${CMAKE_FIND_LIBRARY_SUFFIXES}
                        debug ${OUTPUT_DIR}/Common${CMAKE_DEBUG_POSTFIX}${CMAKE_FIND_LIBRARY_SUFFIXES}
                        optimized ${GMOCK_LIB_DIR}/gmock${CMAKE_FIND_LIBRARY_SUFFIXES}
                        optimized ${OUTPUT_DIR}/AuditTrail${CMAKE_FIND_LIBRARY_SUFFIXES}
                        optimized ${OUTPUT_DIR}/Common${CMAKE_FIND_LIBRARY_SUFFIXES})
