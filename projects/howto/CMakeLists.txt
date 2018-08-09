option(ARA_${ROOT_PROJECT_NAME}_BUILD_HOWTOS     "Select to build sdk how to examples." ON)

function(add_howto name)
  ###############################################################################
  # Step 1:  Set the name of this example to a unique identifier
  ###############################################################################
  set(EXAMPLE_NAME howto-${name})

  string(TOLOWER ${EXAMPLE_NAME} TARGET_NAME)
  option(BUILD_EX_${EXAMPLE_NAME}     "Select to build Example ${ROOT_PROJECT_NAME}" ON)

  if(BUILD_EX_${EXAMPLE_NAME})
    # BASE VARIABLES
    set(${EXAMPLE_NAME}_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/${name}/include" )
    set(${EXAMPLE_NAME}_PRIVATE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/${name}/src" )
    set(${EXAMPLE_NAME}_GENERATED_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}/${name}/" )
    
    ###############################################################################
    # Requirements
    ###############################################################################
    
    ###############################################################################
    #Code Generation
    ###############################################################################
    
    ###############################################################################
    #Sorce and Header Defines
    ###############################################################################
    message(STATUS "Configuring ${EXAMPLE_NAME}")
    
    add_source_files(HDRS ${CMAKE_CURRENT_SOURCE_DIR}/${name}/src/ "*.h"   "Headers\\Private\\")
    add_source_files(SRCS ${CMAKE_CURRENT_SOURCE_DIR}/${name}/src "*.cpp" "Sources\\")
    add_source_files(PUBLIC_HDRS ${CMAKE_CURRENT_SOURCE_DIR}/${name}/include "*.h"  "Headers\\Public\\")
    
    set(${EXAMPLE_NAME}_HEADERS ${HDRS} ${PUBLIC_HDRS} )
    set(${EXAMPLE_NAME}_SOURCES ${SRCS} ) 
    
    ###############################################################################
    #Define Logic
    ###############################################################################  
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

    if(WIN32)
      target_compile_definitions(${TARGET_NAME}  PRIVATE "_CRT_SECURE_NO_WARNINGS"  )
    elseif(CMAKE_COMPILER_IS_GNUCXX)
      #target_link_options(${TARGET_NAME} "LINKER:--no-as-needed" )
      #set_target_properties(${TARGET_NAME} PROPERTIES LINK_FLAGS ${${EXAMPLE_NAME}_LDFLAGS})
    endif()
  
    ###############################################################################
    # COMPILATION & LINKAGE MODIFICATIONS
    ###############################################################################
    target_compile_definitions(${TARGET_NAME} PUBLIC ${${PREFIX}_CPPFLAGS_INTERFACE})
    target_compile_definitions(${TARGET_NAME} PRIVATE ${${PREFIX}_CPPFLAGS})

    target_include_directories(${TARGET_NAME}
      PRIVATE  ${${EXAMPLE_NAME}_INCLUDE_DIR} ${${EXAMPLE_NAME}_PRIVATE_INCLUDE_DIR} ${${EXAMPLE_NAME}_GENERATED_INCLUDE_DIR} 
               "${CMAKE_CURRENT_SOURCE_DIR}/common-source/" "${CMAKE_CURRENT_SOURCE_DIR}/include/"
      INTERFACE $<BUILD_INTERFACE:${${EXAMPLE_NAME}_GENERATED_INCLUDE_DIR}> 
                $<BUILD_INTERFACE:${${EXAMPLE_NAME}_INCLUDE_DIR}>
                $<BUILD_INTERFACE:"${CMAKE_CURRENT_SOURCE_DIR}/${names}/src/">

                $<INSTALL_INTERFACE:include>
    )

    list(APPEND ${EXAMPLE_NAME}_LIBS
      Biogears::libbiogears
      ${CMAKE_THREAD_LIBS_INIT}
      Threads::Threads
      howto::common
    )
      
    target_link_libraries(${TARGET_NAME} ${${EXAMPLE_NAME}_LIBS} )
    if(ARA_STAGE_RUNTIME) 
      add_dependencies(${TARGET_NAME} stage_runtime)
    endif()
    
  endif(BUILD_EX_${EXAMPLE_NAME})
endfunction(add_howto)

set(OUTPUT_PREFIX ${CMAKE_BINARY_DIR}/outputs)
if(UNIX)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_PREFIX}/howto/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_PREFIX}/howto/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_PREFIX}/howto/bin)
else()
  foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER _${OUTPUTCONFIG} OUTPUTCONFIG_UPPER)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY${OUTPUTCONFIG_UPPER} "${OUTPUT_PREFIX}/${OUTPUTCONFIG}/lib")
    set(CMAKE_PDB_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} "${OUTPUT_PREFIX}/${OUTPUTCONFIG}/lib")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY${OUTPUTCONFIG_UPPER} "${OUTPUT_PREFIX}/${OUTPUTCONFIG}/bin")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY${OUTPUTCONFIG_UPPER} "${OUTPUT_PREFIX}/${OUTPUTCONFIG}/bin")
  endforeach(OUTPUTCONFIG)
endif()
############################################################################
# Step 1:
#         Examples are almost never order dependent just add them as we find
#         them
############################################################################
if(ARA_${ROOT_PROJECT_NAME}_BUILD_HOWTOS)
  include ( ${PROJECT_SOURCE_DIR}/cmake/cmake-common_logic.cmake )
  
  find_package(Threads REQUIRED)


  add_source_files(COMMON_HDRS ${CMAKE_CURRENT_SOURCE_DIR}/common-source "*.h"   "Headers\\Private\\")
  add_source_files(COMMON_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/common-source "*.cpp" "Sources\\")
  add_source_files(COMMON_PUBLIC_HDRS ${CMAKE_CURRENT_SOURCE_DIR}/common-source/include "*.h"  "Headers\\Public\\")
  add_library(howto-common OBJECT ${COMMON_HDRS} ${COMMON_SRCS} ${COMMON_PUBLIC_HDRS} )
  add_library(howto::common ALIAS howto-common)
   
  set_target_properties(howto-common
        PROPERTIES
        FOLDER Examples
        OUTPUT_NAME "howto-common"
        PROJECT_LABEL "howto-common"
        DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX}
        CXX_STANDARD 14
        VS_DEBUGGER_WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/runtime
  )
  target_link_libraries(howto-common Biogears::libbiogears)
  target_include_directories(howto-common
      PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/common-source
  )

 target_compile_definitions(howto-common PRIVATE $<$<PLATFORM_ID:Windows>:"_CRT_SECURE_NO_WARNINGS">  )

  childlist( dir_list ${CMAKE_CURRENT_SOURCE_DIR} )
  foreach( dir IN LISTS dir_list )
  if(NOT dir MATCHES "common-source")
    add_howto( ${dir} )
  endif()
  endforeach()
endif()

message(STATUS "")

if (NOT (Java_FOUND AND ARA_${ROOT_PROJECT_NAME}_BUILD_JAVATOOLS) )
  message(WARNING "SDK examples require the use of gen-states which requires ARA_${ROOT_PROJECT_NAME}_BUILD_JAVATOOLS and a copy of the Java development kit.")
endif()
