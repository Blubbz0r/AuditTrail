project(Common)

add_library(
    ${PROJECT_NAME}

    DebugNew.h
    Log.h
    LogEasylogging.h LogEasylogging.cpp
    STLUtils.h STLUtils.cpp
)

target_include_directories(
    ${PROJECT_NAME}

    PUBLIC
        ${PROJECT_SOURCE_DIR}
)

set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "libs")
