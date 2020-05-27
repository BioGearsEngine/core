cmake_policy(SET CMP0070 NEW)

option(${ROOT_PROJECT_NAME}_WEBSITE_TOOLS "Use Python To Build Website" OFF)

if(${ROOT_PROJECT_NAME}_WEBSITE_TOOLS) 
       add_subdirectory(src) 
endif()

message(STATUS " ")
