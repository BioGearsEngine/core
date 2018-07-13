###############################################################################
# Policy adjustments
###############################################################################

###############################################################################
# Options
###############################################################################

###############################################################################
# Base Variables
###############################################################################
set(PROJECT_NAME biogears)
set(PREFIX bio)

set(${PREFIX}_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include" )
set(${PREFIX}_PRIVATE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src" )
set(${PREFIX}_GENERATED_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}" )
set(${PREFIX}_UNIT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/unit" PARENT_SCOPE)

###############################################################################
# Requirments
###############################################################################
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
if(WIN32)
  set(Boost_USE_STATIC_LIBS ON) 
endif()
find_package(Threads REQUIRED)

#find_package(Boost 1.60.0 COMPONENTS system REQUIRED)
find_package(Log4cpp CONFIG)
verify_package(Log4cpp REQUIRED)

find_package(Eigen3 CONFIG)
verify_package(Eigen3 REQUIRED)

verify_package(CodeSynthesis REQUIRED)

###############################################################################
#Code Generation
###############################################################################
#REGISTER_XSD_DIR("recursive/biogears" "biogears" "schema" "schema")
file(GLOB_RECURSE biogears_xsd_list "${PROJECT_SOURCE_DIR}/share/xsd/biogears/*.xsd")
file(GLOB_RECURSE cdm_xsd_list "${PROJECT_SOURCE_DIR}/share/xsd/cdm/*.xsd")

GENERATE_XSD_SCHEMA( data-model-schema.xsd CONFIG data-model-schema.cfg 
                     ROOT biogears SUBPATH schema RESOURCE_FOLDER resources/schema 
		     STAGE
		     )

REGISTER_XSD_FILE  ( BioGearsDataModel.xsd CONFIG BioGearsDataModel.cfg 
                     ROOT biogears SUBPATH schema RESOURCE_FOLDER resources/schema
		     DEPENDS ${biogears_xsd_list} ${cdm_xsd_list} ${PROJECT_SOURCE_DIR}/share/xsd/CommonDataModel.xsd ${CMAKE_CURRENT_BINARY_DIR}/biogears/schema/data-model-schema.hxx
                     STAGE)
###############################################################################
#Sorce and Header Defines
###############################################################################
message(STATUS "")
message(STATUS "Configuring ${PROJECT_NAME}")

#Biogears String Export
add_source_files(PUBLIC_STRING_HDRS  ${CMAKE_CURRENT_SOURCE_DIR}/string/include "*.h"   "Headers\\Public\\")
add_source_files(STRING_SRCS  ${CMAKE_CURRENT_SOURCE_DIR}/string/src "*.cpp"   "Sources\\")
#Biogears CDM
add_source_files(PUBLIC_CDM_HDRS ${CMAKE_CURRENT_SOURCE_DIR}/cdm "*.h"  "Headers\\Public\\")
add_source_files(GEN_CDM_HDRS ${CMAKE_CURRENT_BINARY_DIR}/biogears "*.hxx"  "Headers\\Public\\")
add_source_files(GEN_CDM_SRCS ${CMAKE_CURRENT_BINARY_DIR}/biogears "*.cxx" "Sources\\")
#Biogears Core
add_source_files(HDRS ${CMAKE_CURRENT_SOURCE_DIR}/src "*.h"   "Headers\\Private\\")
add_source_files(SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src "*.cpp" "Sources\\")

add_source_files(PUBLIC_HDRS ${CMAKE_CURRENT_SOURCE_DIR}/include "*.h"  "Headers\\Public\\")
#Biogears JNI
add_source_files(PUBLIC_JNI_HDRS ${CMAKE_CURRENT_SOURCE_DIR}/jni/include "*.h"  "Headers\\Public\\")
add_source_files(JNI_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/jni/src "*cpp" "Sources\\")

set(LIB${PREFIX}_STRING_HEADERS ${STRING_HDRS} ${PUBLIC_STRING_HDRS} ${GEN_STRING_HDRS})
set(LIB${PREFIX}_STRING_SOURCES ${STRING_SRCS} ${GEN_STRING_SRCS})

set(LIB${PREFIX}_CDM_HEADERS ${CDM_HDRS} ${PUBLIC_CDM_HDRS} ${GEN_CDM_HDRS})
set(LIB${PREFIX}_CDM_SOURCES ${CDM_SRCS} ${GEN_CDM_SRCS})

set(LIB${PREFIX}_HEADERS ${HDRS} ${PUBLIC_HDRS} ${GEN_HDRS})
set(LIB${PREFIX}_SOURCES ${SRCS} ${GEN_SRCS})

set(LIB${PREFIX}_JNI_HEADERS ${JNI_HDRS} ${PUBLIC_JNI_HDRS} ${GEN_JNI_HDRS})
set(LIB${PREFIX}_JNI_SOURCES ${JNI_SRCS} ${GEN_JNI_SRCS})


###############################################################################
#Define Logic
###############################################################################

if (ARA_${PREFIX}_BUILD_STATIC)
  add_definitions("-D${PREFIX}_BUILT_AS_STATIC")
endif()

if(ARA_${PREFIX}_BUILD_STATIC)
  add_library(lib${PROJECT_NAME} STATIC ${LIB${PREFIX}_SOURCES} ${LIB${PREFIX}_HEADERS})
	set_target_properties(lib${PROJECT_NAME}
    PROPERTIES
    DEFINE_SYMBOL ${PROJECT_NAME}_EXPORTS
    FOLDER "Libraries"
    OUTPUT_NAME "${PROJECT_NAME}"
    COMPILE_PDB_NAME "${PROJECT_NAME}"
    PROJECT_LABEL "lib${PROJECT_NAME}"
    DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX}
    CXX_STANDARD 14
    )

  if(WIN32)
    list(APPEND ${PREFIX}_CPPFLAGS "-D_SCL_SECURE_NO_WARNINGS" "-D_CRT_SECURE_NO_WARNINGS" "-DNOMINMAX" /bigobj  $ENV{PARALLEL_COMPILE} )
  elseif(CMAKE_COMPILER_IS_GNUCXX)
    list(APPEND ${PREFIX}_CPPFLAGS  ${CodeSynthesis_CPPFLAGS})
    list(APPEND ${PREFIX}_LDFLAGS "-Wl,--no-as-needed" )
    set_target_properties(lib${PROJECT_NAME} PROPERTIES LINK_FLAGS ${${PREFIX}_LDFLAGS})
endif()
  ###############################################################################
  # COMPILATION & LINKAGE MODIFICATIONS
  ###############################################################################
  target_compile_definitions(lib${PROJECT_NAME} PUBLIC ${${PREFIX}_CPPFLAGS_INTERFACE})
  target_compile_definitions(lib${PROJECT_NAME} PRIVATE ${${PREFIX}_CPPFLAGS} )
  target_compile_options(lib${PROJECT_NAME} PRIVATE $<$<PLATFORM_ID:Windows>:/bigobj> )
  
  target_include_directories(lib${PROJECT_NAME}
      PRIVATE  ${${PREFIX}_INCLUDE_DIR} ${${PREFIX}_PRIVATE_INCLUDE_DIR} ${${PREFIX}_GENERATED_INCLUDE_DIR}
      INTERFACE $<BUILD_INTERFACE:${${PREFIX}_GENERATED_INCLUDE_DIR}> 
                $<BUILD_INTERFACE:${${PREFIX}_INCLUDE_DIR}>
                $<INSTALL_INTERFACE:include>
  )
  
  if(ARA_${ROOT_PROJECT_NAME}_BUILD_JAVATOOLS)
	  target_include_directories(lib${PROJECT_NAME} PRIVATE ${Java_INCLUDE_DIRS})
  endif()

  set(${PREFIX}_LIBS
        Threads::Threads
        Eigen3::Eigen
        Log4cpp::Log4cpp
        CodeSynthesis::XSD
        ${CMAKE_DL_LIBS}
  )
  target_link_libraries(lib${PROJECT_NAME}  ${${PREFIX}_LIBS} )
  add_dependencies(lib${PROJECT_NAME} xsd_gen_schema_BioGearsDataModel xsd_gen_schema_data-model-schema)

else()
  if (WIN32)
    ###############################################################################
    # libBiogears_String 
    ###############################################################################
    add_library(lib${PROJECT_NAME}_string SHARED ${LIB${PREFIX}_STRING_SOURCES} ${LIB${PREFIX}_STRING_HEADERS})

	  set_target_properties(lib${PROJECT_NAME}_string
      PROPERTIES
      DEFINE_SYMBOL ${PROJECT_NAME}_string_EXPORTS
      FOLDER "Libraries"
      OUTPUT_NAME "${PROJECT_NAME}_string"
      COMPILE_PDB_NAME "${PROJECT_NAME}_string"
      PROJECT_LABEL "lib${PROJECT_NAME}_string"
      DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX}
      CXX_STANDARD 14
      )

    ###############################################################################
    # libBiogears_String OMPILATION & LINKAGE MODIFICATIONS
    ###############################################################################
    set( ${PREFIX}_CPPFLAGS $<$<PLATFORM_ID:Windows>:"-D_SCL_SECURE_NO_WARNINGS" "-D_CRT_SECURE_NO_WARNINGS" "-DNOMINMAX" > )
    target_compile_definitions(lib${PROJECT_NAME}_string  PUBLIC ${${PREFIX}_CPPFLAGS_INTERFACE})
    target_compile_definitions(lib${PROJECT_NAME}_string  PRIVATE ${${PREFIX}_CPPFLAGS} )
    target_compile_options(lib${PROJECT_NAME}_string PRIVATE $<$<PLATFORM_ID:Windows>:/bigobj> )
    
    target_include_directories(lib${PROJECT_NAME}_string 
        PRIVATE  ${${PREFIX}_INCLUDE_DIR} ${${PREFIX}_PRIVATE_INCLUDE_DIR} ${${PREFIX}_GENERATED_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/string/include
        INTERFACE 
                  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/string/include>
                  $<INSTALL_INTERFACE:include>
    )
    
    set(${PREFIX}_string_LIBS
          Threads::Threads
          ${CMAKE_DL_LIBS}
    )
    target_link_libraries(lib${PROJECT_NAME}_string PUBLIC  ${${PREFIX}_string_LIBS} )

    install(TARGETS lib${PROJECT_NAME}_string EXPORT libBiogears
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION ${LIBRARY_INSTALL_DIR}
        ARCHIVE DESTINATION lib
     )

  endif(WIN32)
  ###############################################################################
  # libBiogears_cdm
  ###############################################################################
  add_library(lib${PROJECT_NAME}_cdm    SHARED ${LIB${PREFIX}_CDM_SOURCES} ${LIB${PREFIX}CMD__HEADERS})
  set_target_properties(lib${PROJECT_NAME}_cdm
    PROPERTIES
    DEFINE_SYMBOL ${PROJECT_NAME}_cdm_EXPORTS
    FOLDER "Libraries"
    OUTPUT_NAME "${PROJECT_NAME}_cdm"
    COMPILE_PDB_NAME "${PROJECT_NAME}_cdm"
    PROJECT_LABEL "lib${PROJECT_NAME}_cdm"
    DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX}
    CXX_STANDARD 14
    )
  add_dependencies(lib${PROJECT_NAME}_cdm xsd_gen_schema_BioGearsDataModel xsd_gen_schema_data-model-schema)

  ###############################################################################
  # libBiogears_cdm COMPILATION & LINKAGE MODIFICATIONS
  ###############################################################################
  set( ${PREFIX}_CPPFLAGS $<$<PLATFORM_ID:Windows>:"-D_SCL_SECURE_NO_WARNINGS" "-D_CRT_SECURE_NO_WARNINGS" "-DNOMINMAX" > )
  target_compile_definitions(lib${PROJECT_NAME}_cdm  PUBLIC ${${PREFIX}_CPPFLAGS_INTERFACE})
  target_compile_definitions(lib${PROJECT_NAME}_cdm  PRIVATE ${${PREFIX}_CPPFLAGS} )
  target_compile_options(lib${PROJECT_NAME}_cdm PRIVATE $<$<PLATFORM_ID:Windows>:/bigobj> )
  
  target_include_directories(lib${PROJECT_NAME}_cdm 
      PRIVATE  ${${PREFIX}_INCLUDE_DIR} ${${PREFIX}_PRIVATE_INCLUDE_DIR} ${${PREFIX}_GENERATED_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/cdm/include
      INTERFACE 
                $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/cdm/include>
                $<INSTALL_INTERFACE:include>
  )
  
  set(${PREFIX}_cdm_LIBS
            CodeSynthesis::XSD
            Eigen3::Eigen
  )
	message(STATUS "${PREFIX}_cdm_LIBS=${${PREFIX}_cdm_LIBS}")
  if(WIN32)
    target_link_libraries(lib${PROJECT_NAME}_cdm  PUBLIC lib${PROJECT_NAME}_string ${${PREFIX}_cdm_LIBS} )
  else()
    target_link_libraries(lib${PROJECT_NAME}_cdm  PUBLIC ${${PREFIX}_cdm_LIBS} )
  endif()
  install(TARGETS lib${PROJECT_NAME}_cdm
      RUNTIME DESTINATION bin
      LIBRARY DESTINATION ${LIBRARY_INSTALL_DIR}
      ARCHIVE DESTINATION lib
   )
  ###############################################################################
  # libBiogears
  ###############################################################################
  add_library(lib${PROJECT_NAME}        SHARED ${LIB${PREFIX}_SOURCES} ${LIB${PREFIX}_HEADERS})
  	set_target_properties(lib${PROJECT_NAME}
    PROPERTIES
    DEFINE_SYMBOL ${PROJECT_NAME}_EXPORTS
    FOLDER "Libraries"
    OUTPUT_NAME "${PROJECT_NAME}"
    COMPILE_PDB_NAME "${PROJECT_NAME}"
    PROJECT_LABEL "lib${PROJECT_NAME}"
    DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX}
    CXX_STANDARD 14
    )

  ###############################################################################
  # libBiogears COMPILATION & LINKAGE MODIFICATIONS
  ###############################################################################
  set( ${PREFIX}_CPPFLAGS $<$<PLATFORM_ID:Windows>:"-D_SCL_SECURE_NO_WARNINGS" "-D_CRT_SECURE_NO_WARNINGS" "-DNOMINMAX" > )
  target_compile_definitions(lib${PROJECT_NAME}  PUBLIC ${${PREFIX}_CPPFLAGS_INTERFACE})
  target_compile_definitions(lib${PROJECT_NAME}  PRIVATE ${${PREFIX}_CPPFLAGS} )
  target_compile_options(lib${PROJECT_NAME} PRIVATE $<$<PLATFORM_ID:Windows>:/bigobj> )
  
  target_include_directories(lib${PROJECT_NAME} 
      PRIVATE  ${${PREFIX}_INCLUDE_DIR} ${${PREFIX}_PRIVATE_INCLUDE_DIR} ${${PREFIX}_GENERATED_INCLUDE_DIR} 
      INTERFACE 
                $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
				$<BUILD_INTERFACE:${${PREFIX}_GENERATED_INCLUDE_DIR}>
                $<INSTALL_INTERFACE:include>
  )
  
  set(${PREFIX}_LIBS
            Eigen3::Eigen
            Log4cpp::Log4cpp
            lib${PROJECT_NAME}_cdm
  )
  message(STATUS "${PREFIX}_LIBS=${${PREFIX}_LIBS}")
  target_link_libraries(lib${PROJECT_NAME}  PUBLIC ${${PREFIX}_LIBS} )
  install(TARGETS lib${PROJECT_NAME}_cdm EXPORT libBiogears
      RUNTIME DESTINATION bin
      LIBRARY DESTINATION ${LIBRARY_INSTALL_DIR}
      ARCHIVE DESTINATION lib
   )

  ###############################################################################
  # libBiogears_jni
  ###############################################################################
  if(Java_FOUND AND ARA_${ROOT_PROJECT_NAME}_BUILD_JAVATOOLS)
     add_library(lib${PROJECT_NAME}_jni  SHARED ${LIB${PREFIX}_JNI_SOURCES} ${LIB${PREFIX}_JNI_HEADERS})
  	   set_target_properties(lib${PROJECT_NAME}_jni
       PROPERTIES
       DEFINE_SYMBOL ${PROJECT_NAME}_jni_EXPORTS
       FOLDER "Libraries"
       OUTPUT_NAME "${PROJECT_NAME}_jni"
       COMPILE_PDB_NAME "${PROJECT_NAME}_jni"
       PROJECT_LABEL "lib${PROJECT_NAME}_jni"
       DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX}
       CXX_STANDARD 14
       )
  ###############################################################################
  # libBiogears_jni COMPILATION & LINKAGE MODIFICATIONS
  ###############################################################################
  set( ${PREFIX}_CPPFLAGS $<$<PLATFORM_ID:Windows>:"-D_SCL_SECURE_NO_WARNINGS" "-D_CRT_SECURE_NO_WARNINGS" "-DNOMINMAX" > )
  target_compile_definitions(lib${PROJECT_NAME}_jni  PUBLIC ${${PREFIX}_CPPFLAGS_INTERFACE})
  target_compile_definitions(lib${PROJECT_NAME}_jni  PRIVATE ${${PREFIX}_CPPFLAGS} )
  target_compile_options(lib${PROJECT_NAME}_jni PRIVATE $<$<PLATFORM_ID:Windows>:/bigobj> )
  
  target_include_directories(lib${PROJECT_NAME}_jni 
      PRIVATE  ${${PREFIX}_INCLUDE_DIR} ${${PREFIX}_PRIVATE_INCLUDE_DIR} ${${PREFIX}_GENERATED_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/jni/include
	    PUBLIC   ${JNI_INCLUDE_DIRS}
      INTERFACE 
                $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/jni/include>
                $<INSTALL_INTERFACE:include>
  )
  
  set(${PREFIX}_jni_LIBS
            CodeSynthesis::XSD
            Eigen3::Eigen
            lib${PROJECT_NAME}
  )
	message(STATUS "${PREFIX}_jni_LIBS=${${PREFIX}_jni_LIBS}")
  if(WIN32)
    target_link_libraries(lib${PROJECT_NAME}_jni  PUBLIC lib${PROJECT_NAME}_string ${${PREFIX}_jni_LIBS} )
  else()
    target_link_libraries(lib${PROJECT_NAME}_jni  PUBLIC ${${PREFIX}_jni_LIBS} )
  endif()

     install(TARGETS lib${PROJECT_NAME}_jni EXPORT libBiogears
         RUNTIME DESTINATION bin
         LIBRARY DESTINATION ${LIBRARY_INSTALL_DIR}
         ARCHIVE DESTINATION lib
      )
   endif()
endif()



 install(TARGETS lib${PROJECT_NAME} EXPORT libBiogears
     RUNTIME DESTINATION bin
     LIBRARY DESTINATION ${LIBRARY_INSTALL_DIR}
     ARCHIVE DESTINATION lib
  )


if(NOT ARA_${PREFIX}_BUILD_STATIC)
  if(WIN32)
    install(FILES $<TARGET_PDB_FILE:lib${PROJECT_NAME}> DESTINATION bin OPTIONAL)
  endif()
else()
   if(WIN32)
      foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
          string(TOUPPER _${OUTPUTCONFIG} OUTPUTCONFIG_UPPER)
          install(FILES "${CMAKE_PDB_OUTPUT_DIRECTORY${OUTPUTCONFIG_UPPER}}/*.pdb" DESTINATION lib CONFIGURATIONS ${OUTPUTCONFIG} OPTIONAL)
      endforeach(OUTPUTCONFIG)
   endif()
endif()
 
install(DIRECTORY 
    include/biogears
    cdm/include/biogears
    jni/include/biogears
    string/include/biogears
	${CMAKE_CURRENT_BINARY_DIR}/biogears/
    DESTINATION include/
    FILE_PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
    )
  
message(STATUS "")
