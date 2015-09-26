set(CURRENT_PROJECT_NAME Common)

include_directories(
    ${LOG_DIR}
)

add_library(${CURRENT_PROJECT_NAME} DebugNew.h
									Log.h
									LogEasylogging.h LogEasylogging.cpp
                                    STLUtils.h STLUtils.cpp
                                    ${XML_SOURCE_DIR}/pugixml.hpp ${XML_SOURCE_DIR}/pugixml.cpp ${XML_SOURCE_DIR}/pugiconfig.hpp)
