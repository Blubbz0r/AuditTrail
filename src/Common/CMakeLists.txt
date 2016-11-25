project(Common)

add_library(
    ${PROJECT_NAME}

    DebugNew.h
    Log.h
    LogEasylogging.h LogEasylogging.cpp
    STLUtils.h STLUtils.cpp
    ${XML_SOURCE_DIR}/pugixml.hpp ${XML_SOURCE_DIR}/pugixml.cpp ${XML_SOURCE_DIR}/pugiconfig.hpp
)

target_include_directories(
    ${PROJECT_NAME}

    PUBLIC
        ${PROJECT_SOURCE_DIR}

    PRIVATE
        ${LOG_DIR}
)
