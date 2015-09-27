include_directories(
	../Common
    ${XML_SOURCE_DIR}
)

add_library(${PROJECT_NAME} ActiveParticipant.h ActiveParticipant.cpp
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
                            Media.h
                            Message.h
                            Node.h
                            Writer.h
                            WriterPugixml.h WriterPugixml.cpp)

add_dependencies(${PROJECT_NAME} Common)