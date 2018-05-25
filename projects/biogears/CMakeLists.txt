###############################################################################
# Policy adjustments
###############################################################################
cmake_minimum_required(VERSION 3.8.0)
cmake_policy(VERSION 3.8.0)
###############################################################################
# Options
###############################################################################

###############################################################################
# Base Variables
###############################################################################
set(PROJECT_NAME biogears)
set(PREFIX bio)

set(${PREFIX}_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include" PARENT_SCOPE)
set(${PREFIX}_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include" )
set(${PREFIX}_PRIVATE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src" PARENT_SCOPE)
set(${PREFIX}_PRIVATE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src" )
set(${PREFIX}_GENERATED_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}" PARENT_SCOPE)
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
find_package(Eigen3 REQUIRED)
find_package(Log4cpp REQUIRED)
find_package(CodeSynthesis REQUIRED)

if (ARA_${PREFIX}_BUILD_STATIC)
  add_definitions("-D${PREFIX}_BUILT_AS_STATIC")
  add_definitions("-DARACORE_BUILT_AS_STATIC")
endif()
###############################################################################
#Code Generation
###############################################################################
#REGISTER_XSD_DIR("recursive/biogears" "biogears" "schema" "schema")
REGISTER_XSD_FILE( BioGearsDataModel.xsd BioGearsDataModel.cfg biogears schema resources/schema )
GENERATE_XSD_SCHEMA( data-model-schema.xsd data-model-schema.cfg biogears schema resources/schema )

###############################################################################
#Sorce and Header Defines
###############################################################################
message(STATUS "")
message(STATUS "Configuring ${PROJECT_NAME}")

#Biogears String Export
add_source_files(PUBLIC_STRING_HDRS  ${CMAKE_CURRENT_SOURCE_DIR}/string/include "*.h"   "Headers\\Public\\")
add_source_files(STRING_SRCS  ${CMAKE_CURRENT_SOURCE_DIR}/string/src "*.cpp"   "Sources\\")
#Biogears CDM
add_source_files(GEN_CDM_HDRS ${CMAKE_CURRENT_BINARY_DIR}/biogears "*.hxx"  "Headers\\Public\\")
add_source_files(PUBLIC_CDM_HDRS ${CMAKE_CURRENT_SOURCE_DIR}/cdm "*.h"  "Headers\\Public\\")
add_source_files(GEN_CDM_SRCS ${CMAKE_CURRENT_BINARY_DIR}/biogears "*.cxx" "Sources\\")
#Biogears Core
add_source_files(HDRS ${CMAKE_CURRENT_SOURCE_DIR}/src "*.h"   "Headers\\Private\\")
add_source_files(HDRS ${CMAKE_CURRENT_SOURCE_DIR}/src "*.hpp"   "Headers\\Private\\")
add_source_files(SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src "*.cpp" "Sources\\")
add_source_files(SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src "*.c" "Sources\\")
add_source_files(PUBLIC_HDRS ${CMAKE_CURRENT_SOURCE_DIR}/include "*.h"  "Headers\\Public\\")
add_source_files(PUBLIC_HDRS ${CMAKE_CURRENT_SOURCE_DIR}/include "*.hpp"  "Headers\\Public\\")
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
if(WIN32)
  list(APPEND ${PREFIX}_CPPFLAGS_EXPORT )
  list(APPEND ${PREFIX}_CPPFLAGS "-D_SCL_SECURE_NO_WARNINGS" "-D_CRT_SECURE_NO_WARNINGS" /bigobj  $ENV{PARALLEL_COMPILE} )
elseif(CMAKE_COMPILER_IS_GNUCXX)
list(APPEND ${PREFIX}_CPPFLAGS_EXPORT )
  list(APPEND ${PREFIX}_CPPFLAGS  ${CodeSynthesis_CPPFLAGS})
  list(APPEND ${PREFIX}_LDFLAGS "-Wl,--no-as-needed" )
endif()

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

  ###############################################################################
  # COMPILATION & LINKAGE MODIFICATIONS
  ###############################################################################
  foreach(MY_ITEM ${${PREFIX}_CPPFLAGS_EXPORT})
      TARGET_COMPILE_OPTIONS(lib${PROJECT_NAME}
          PUBLIC ${MY_ITEM}
      )
  endforeach(MY_ITEM)
  foreach(MY_ITEM ${${PREFIX}_CPPFLAGS})
      TARGET_COMPILE_OPTIONS(lib${PROJECT_NAME}
          PRIVATE ${MY_ITEM}
      )
  endforeach(MY_ITEM)

  list(APPEND ${PREFIX}_INCLUDES
        ${PREFIX}_INCLUDE_DIR
        ${PREFIX}_PRIVATE_INCLUDE_DIR
        ${PREFIX}_GENERATED_INCLUDE_DIR
#        Boost_INCLUDE_DIRS
        Eigen3_INCLUDE_DIRS
        Log4cpp_INCLUDE_DIRS
  )
  if(ARA_${ROOT_PROJECT_NAME}_BUILD_JAVATOOLS)
	  list(APPEND ${PREFIX}_INCLUDES   Java_JNI_INCLUDE_DIR)
  endif()
  list(REMOVE_DUPLICATES ${PREFIX}_INCLUDES)
  ensure_paths_set(${PREFIX}_INCLUDES lib${PROJECT_NAME})

  foreach( ITEM IN LISTS ${PREFIX}_INCLUDES)
    message(STATUS "  ${${ITEM}}")
  endforeach()
  set(${PREFIX}_LIBS
        ${CMAKE_THREAD_LIBS_INIT}
        ${CMAKE_DL_LIBS}
 #       ${Boost_LIBRARIES}
        ${EIGEN_LIBRARIES}
        ${Log4cpp_LIBRARIES}
        ${CodeSynthesis_LIBRARIES}
  )
  set(${PREFIX}_LIBS ${${PREFIX}_LIBS} PARENT_SCOPE)
  target_link_libraries(lib${PROJECT_NAME}  ${${PREFIX}_LIBS} )

else()
  if (WIN32)
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
    # libBiogears_String 
    ###############################################################################
    foreach(MY_ITEM ${${PREFIX}_CPPFLAGS_EXPORT})
        TARGET_COMPILE_OPTIONS(lib${PROJECT_NAME}_string
            PUBLIC ${MY_ITEM}
        )
    endforeach(MY_ITEM)
    foreach(MY_ITEM ${${PREFIX}_CPPFLAGS})
        TARGET_COMPILE_OPTIONS(lib${PROJECT_NAME}_string
            PRIVATE ${MY_ITEM}
        )
    endforeach(MY_ITEM)

    list(APPEND ${PREFIX}_string_INCLUDES
          ${${PREFIX}_INCLUDE_DIR}
          ${${PREFIX}_PRIVATE_INCLUDE_DIR}
          ${${PREFIX}_GENERATED_INCLUDE_DIR}
          ${CMAKE_CURRENT_SOURCE_DIR}/string/include
    )


    set(${PREFIX}_string_LIBS
          ${CMAKE_THREAD_LIBS_INIT}
          ${CMAKE_DL_LIBS}
    )

    target_include_directories(lib${PROJECT_NAME}_string  PUBLIC ${${PREFIX}_string_INCLUDES} )
    target_link_libraries(lib${PROJECT_NAME}_string  ${${PREFIX}_string_LIBS} )
    install(TARGETS lib${PROJECT_NAME}_string
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

  foreach(MY_ITEM ${${PREFIX}_CPPFLAGS_EXPORT})
      TARGET_COMPILE_OPTIONS(lib${PROJECT_NAME}_cdm
          PUBLIC ${MY_ITEM}
      )
  endforeach(MY_ITEM)
  foreach(MY_ITEM ${${PREFIX}_CPPFLAGS})
      TARGET_COMPILE_OPTIONS(lib${PROJECT_NAME}_cdm
          PRIVATE ${MY_ITEM}
      )
  endforeach(MY_ITEM)
  list(APPEND ${PREFIX}_cdm_INCLUDES
          ${${PREFIX}_INCLUDE_DIR}
          ${${PREFIX}_PRIVATE_INCLUDE_DIR}
          ${${PREFIX}_GENERATED_INCLUDE_DIR}
          ${CMAKE_CURRENT_SOURCE_DIR}/string/include
          ${CMAKE_CURRENT_SOURCE_DIR}/cdm/include
          ${CodeSynthesis_INCLUDE_DIRS}
          ${Eigen3_INCLUDE_DIRS}
  )


  set(${PREFIX}_cdm_LIBS
        ${CMAKE_THREAD_LIBS_INIT}
        ${CMAKE_DL_LIBS}
        ${CodeSynthesis_LIBRARIES}
  )
	message(STATUS "${PREFIX}_cdm_LIBS=${${PREFIX}_cdm_LIBS}")
  target_include_directories(lib${PROJECT_NAME}_cdm PUBLIC ${${PREFIX}_cdm_INCLUDES} )
  if(WIN32)
    target_link_libraries(lib${PROJECT_NAME}_cdm  lib${PROJECT_NAME}_string ${${PREFIX}_cdm_LIBS} )
  else()
    target_link_libraries(lib${PROJECT_NAME}_cdm  ${${PREFIX}_cdm_LIBS} )
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

    foreach(MY_ITEM ${${PREFIX}_CPPFLAGS_EXPORT})
      TARGET_COMPILE_OPTIONS(lib${PROJECT_NAME}
          PUBLIC ${MY_ITEM}
      )
  endforeach(MY_ITEM)
  foreach(MY_ITEM ${${PREFIX}_CPPFLAGS})
      TARGET_COMPILE_OPTIONS(lib${PROJECT_NAME}
          PRIVATE ${MY_ITEM}
      )
  endforeach(MY_ITEM)

  list(APPEND ${PREFIX}_INCLUDES
          ${${PREFIX}_INCLUDE_DIR}
          ${${PREFIX}_PRIVATE_INCLUDE_DIR}
          ${${PREFIX}_GENERATED_INCLUDE_DIR}
          ${CMAKE_CURRENT_SOURCE_DIR}/string/include
          ${CMAKE_CURRENT_SOURCE_DIR}/cdm/include
#          ${Boost_INCLUDE_DIRS}
          ${Eigen3_INCLUDE_DIRS}
          ${Log4cpp_INCLUDE_DIRS}
  )


  set(${PREFIX}_LIBS
        ${CMAKE_THREAD_LIBS_INIT}
        ${CMAKE_DL_LIBS}
#        ${Boost_LIBRARIES}
        ${EIGEN_LIBRARIES}
        ${Log4cpp_LIBRARIES}
  )

  target_include_directories(lib${PROJECT_NAME} PUBLIC ${${PREFIX}_INCLUDES} )
  target_link_libraries(lib${PROJECT_NAME}  lib${PROJECT_NAME}_cdm ${${PREFIX}_LIBS} )


  ###############################################################################
  # libBiogears_jni
  ###############################################################################
  if(Java_FOUND AND ARA_${ROOT_PROJECT_NAME}_BUILD_JAVATOOLS)
  message(STATUS "JAVA STUFF")
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

       foreach(MY_ITEM ${${PREFIX}_CPPFLAGS_EXPORT})
         TARGET_COMPILE_OPTIONS(lib${PROJECT_NAME}_jni
             PUBLIC ${MY_ITEM}
         )
     endforeach(MY_ITEM)
     foreach(MY_ITEM ${${PREFIX}_CPPFLAGS})
         TARGET_COMPILE_OPTIONS(lib${PROJECT_NAME}_jni
             PRIVATE ${MY_ITEM}
         )
     endforeach(MY_ITEM)

     list(APPEND ${PREFIX}_jni_INCLUDES
             ${${PREFIX}_INCLUDE_DIR}
             ${${PREFIX}_PRIVATE_INCLUDE_DIR}
             ${${PREFIX}_GENERATED_INCLUDE_DIR}
             ${CMAKE_CURRENT_SOURCE_DIR}/string/include
             ${CMAKE_CURRENT_SOURCE_DIR}/cdm/include
             ${CMAKE_CURRENT_SOURCE_DIR}/jni/include
             ${Java_JNI_INCLUDE_DIR}
     )

     set(${PREFIX}_jni_LIBS
           ${CMAKE_THREAD_LIBS_INIT}
           ${CMAKE_DL_LIBS}
     )

     target_include_directories(lib${PROJECT_NAME}_jni  PUBLIC ${${PREFIX}_jni_INCLUDES} )
     target_link_libraries(lib${PROJECT_NAME}_jni  lib${PROJECT_NAME} ${${PREFIX}_jni_LIBS} )
     install(TARGETS lib${PROJECT_NAME}_jni
         RUNTIME DESTINATION bin
         LIBRARY DESTINATION ${LIBRARY_INSTALL_DIR}
         ARCHIVE DESTINATION lib
      )
   endif()
endif()


if(CMAKE_COMPILER_IS_GNUCXX)
    set_target_properties(lib${PROJECT_NAME} PROPERTIES LINK_FLAGS ${${PREFIX}_LDFLAGS})
endif()

 install(TARGETS lib${PROJECT_NAME}
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
    DESTINATION include/
    FILE_PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
    )
  
message(STATUS "")
