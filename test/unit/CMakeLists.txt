project(UnitTests)

add_executable(
    ${PROJECT_NAME}
    
    main.cpp
    LogNoop.h
    ApplicationActivityTests.cpp
    AuditLogUsedTests.cpp
    BeginTransferringInstancesTests.cpp
    DataExportTests.cpp
    DataImportTests.cpp
    InstancesAccessedTests.cpp
    InstancesTransferredTests.cpp
    StudyDeletedTests.cpp
    TestData.h
    Tests.cpp
    WriterMock.h
)

add_dependencies(${CURRENT_PROJECT_NAME} AuditTrail Common)

target_include_directories(
    ${PROJECT_NAME}

    PRIVATE
        $<TARGET_PROPERTY:AuditTrail,INTERFACE_INCLUDE_DIRECTORIES>
        $<TARGET_PROPERTY:Common,INTERFACE_INCLUDE_DIRECTORIES>
)

target_link_libraries(
    ${PROJECT_NAME}

    AuditTrail
    Common
    ${CONAN_LIBS}
)

set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "tests")
