cmake_minimum_required(VERSION 3.8.0)

# Policy adjustments
cmake_policy(VERSION 3.8.0)

###############################################################################
# Step 1:  Set the name of this example to a unique identifier
###############################################################################
set(EXAMPLE_NAME howto-SarinExposure)

string(TOLOWER ${EXAMPLE_NAME} TARGET_NAME)
option(BUILD_EX_${EXAMPLE_NAME}     "Select to build Example ${ROOT_PROJECT_NAME}" ON)

if(BUILD_EX_${EXAMPLE_NAME})
  # BASE VARIABLES
  set(${EXAMPLE_NAME}_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include" PARENT_SCOPE)
  set(${EXAMPLE_NAME}_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include" )
  set(${EXAMPLE_NAME}_PRIVATE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src" PARENT_SCOPE)
  set(${EXAMPLE_NAME}_PRIVATE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src" )
  set(${EXAMPLE_NAME}_GENERATED_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}" PARENT_SCOPE)
  set(${EXAMPLE_NAME}_GENERATED_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}" )
  
  ###############################################################################
  # Requirements
  ###############################################################################
  set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
 
  set(CMAKE_MODULE_PATH  ${PROJECT_SOURCE_DIR}/cmake/common ${ARA_${ROOT_PROJECT_NAME}_EXTERNAL}/cmake)
  find_package(Threads REQUIRED)
  
  if (ARA_${ROOT_PROJECT_NAME}_BUILD_STATIC)
    add_definitions("-DBIOGEARS_BUILT_AS_STATIC")
  endif()
  ###############################################################################
  #Code Generation
  ###############################################################################
  
  ###############################################################################
  #Sorce and Header Defines
  ###############################################################################
  message(STATUS "Configuring ${EXAMPLE_NAME}")
  
  add_source_files(HDRS ${CMAKE_CURRENT_SOURCE_DIR}/../common-source "*.h"   "Headers\\Private\\")
  add_source_files(HDRS ${CMAKE_CURRENT_SOURCE_DIR}/src "*.hpp"   "Headers\\Private\\")
  add_source_files(SRCS ${CMAKE_CURRENT_SOURCE_DIR}/../common-source "*.cpp" "Sources\\")
  add_source_files(SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src "*.cpp" "Sources\\")
  add_source_files(SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src "*.c" "Sources\\")
  add_source_files(PUBLIC_HDRS ${CMAKE_CURRENT_SOURCE_DIR}/include "*.h"  "Headers\\Public\\")
  add_source_files(PUBLIC_HDRS ${CMAKE_CURRENT_SOURCE_DIR}/include "*.hpp"  "Headers\\Public\\")
  
  set(${EXAMPLE_NAME}_HEADERS ${HDRS} ${PUBLIC_HDRS} ${GEN_HDRS})
  set(${EXAMPLE_NAME}_SOURCES ${SRCS} ${GEN_SRCS})
  
  ###############################################################################
  #Define Logic
  ###############################################################################
  if(WIN32)
    list(APPEND ${EXAMPLE_NAME}_CPPFLAGS_EXPORT )
    list(APPEND ${EXAMPLE_NAME}_CPPFLAGS "-D_CRT_SECURE_NO_WARNINGS" $ENV{PARALLEL_COMPILE} )
  elseif(CMAKE_COMPILER_IS_GNUCXX)
    list(APPEND ${EXAMPLE_NAME}_LDFLAGS "-Wl,--no-as-needed" )
  endif()
  
  add_executable(${TARGET_NAME} ${${EXAMPLE_NAME}_SOURCES} ${${EXAMPLE_NAME}_HEADERS})
  
  set_target_properties(${TARGET_NAME}
      PROPERTIES
      DEFINE_SYMBOL ${EXAMPLE_NAME}_EXPORTS
      FOLDER Examples
      OUTPUT_NAME "${TARGET_NAME}"
      PROJECT_LABEL "${TARGET_NAME}"
      DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX}
      CXX_STANDARD 14
      VS_DEBUGGER_WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/runtime
      )
  if(CMAKE_COMPILER_IS_GNUCXX)
      set_target_properties(${TARGET_NAME} PROPERTIES LINK_FLAGS ${${EXAMPLE_NAME}_LDFLAGS})
  endif()
  
  ###############################################################################
  # COMPILATION & LINKAGE MODIFICATIONS
  ###############################################################################
  foreach(MY_ITEM ${${EXAMPLE_NAME}_CPPFLAGS_EXPORT})
      TARGET_COMPILE_OPTIONS(${TARGET_NAME}
          PUBLIC ${MY_ITEM}
      )
  endforeach(MY_ITEM)
  foreach(MY_ITEM ${${EXAMPLE_NAME}_CPPFLAGS})
      TARGET_COMPILE_OPTIONS(${TARGET_NAME}
          PRIVATE ${MY_ITEM}
      )
  endforeach(MY_ITEM)
  
  target_include_directories(${TARGET_NAME}
      PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/"
      PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../common-source/"
      PUBLIC  "${CMAKE_CURRENT_SOURCE_DIR}/src/"
  )
  
  list(APPEND ${EXAMPLE_NAME}_INCLUDES
        ${EXAMPLE_NAME}_INCLUDE_DIR
        bio_INCLUDE_DIR 
  )
  
  ENSURE_PATHS_SET(${EXAMPLE_NAME}_INCLUDES ${TARGET_NAME})
  
  list(APPEND ${EXAMPLE_NAME}_LIBS
    libbiogears
    ${CMAKE_THREAD_LIBS_INIT}
  )
    
  target_link_libraries(${TARGET_NAME}  ${${EXAMPLE_NAME}_LIBS} )
  
endif(BUILD_EX_${EXAMPLE_NAME})
