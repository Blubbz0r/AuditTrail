project(AuditTrail)

add_library(
    ${PROJECT_NAME}

    ActiveParticipant.h ActiveParticipant.cpp
    ApplicationActivity.h ApplicationActivity.cpp
    AuditLogUsed.h AuditLogUsed.cpp
    BeginTransferringInstances.h BeginTransferringInstances.cpp
    CodedValueType.h CodedValueType.cpp
    DataExport.h DataExport.cpp
    DataImport.h DataImport.cpp
    Entity.h
    EntityActiveParticipant.h EntityActiveParticipant.cpp
    EntityEvent.h EntityEvent.cpp
    EntityParticipantObject.h EntityParticipantObject.cpp
    Event.h
    InstancesAccessed.h InstancesAccessed.cpp
    InstancesTransferred.h InstancesTransferred.cpp
    Media.h
    Message.h
    Node.h
    SOPClass.h
    StudyDeleted.h StudyDeleted.cpp
    Writer.h
    WriterPugixml.h WriterPugixml.cpp
)

add_dependencies(${PROJECT_NAME} Common)

target_include_directories(
    ${PROJECT_NAME}

    PUBLIC
        ${PROJECT_SOURCE_DIR}

    PRIVATE
        $<TARGET_PROPERTY:Common,INTERFACE_INCLUDE_DIRECTORIES>
)

set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "libs")
