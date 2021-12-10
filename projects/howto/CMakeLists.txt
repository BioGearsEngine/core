option(${ROOT_PROJECT_NAME}_BUILD_HOWTOS     "Select to build sdk how to examples." ON)
cmake_policy(SET CMP0087 NEW)
message(STATUS "Configuring Examples")
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
    message(STATUS "           : ${EXAMPLE_NAME}")
    
    add_source_files(HDRS LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/${name}/src/ REGEX "*.h" SOURCE_GROUP "Headers\\Private\\")
    add_source_files(SRCS LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/${name}/src REGEX "*.cpp" SOURCE_GROUP "Sources\\")
    add_source_files(PUBLIC_HDRS LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/${name}/include REGEX "*.h" SOURCE_GROUP "Headers\\Public\\")
    
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
        DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}"
        CXX_STANDARD 14
        VS_DEBUGGER_WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/runtime
        VS_DEBUGGER_ENVIRONMENT "${VS_DEBUGGER_ENVIRONMENT_LIST}"
        )

     target_compile_definitions(${TARGET_NAME} PRIVATE ${${PREFIX}_CPPFLAGS} PRIVATE $<$<PLATFORM_ID:Windows>:"_CRT_SECURE_NO_WARNINGS"> )
     target_compile_options(${TARGET_NAME} PRIVATE $<$<PLATFORM_ID:Windows>:/bigobj>  )
  
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
	  Biogears::libbiogears_common
      ${CMAKE_THREAD_LIBS_INIT}
      Threads::Threads
      howto::common
    )
      
    target_link_libraries(${TARGET_NAME} ${${EXAMPLE_NAME}_LIBS} )
    if(TARGET prepare_runtime_dir AND ${ROOT_PROJECT_NAME}_PREPARE_RUNTIME_DIR) 
      add_dependencies(${TARGET_NAME} prepare_runtime_dir)
    endif()
   
    add_dependencies(all_howtos ${TARGET_NAME})
    
if (NOT (${ROOT_PROJECT_NAME}_BUILD_INSTALLER AND APPLE)) 
  install(TARGETS ${TARGET_NAME} EXPORT all_howtos
     RUNTIME DESTINATION bin/${${ROOT_PROJECT_NAME}_PLATFORM_PREFIX}$<$<BOOL:${${ROOT_PROJECT_NAME}_BREAKOUT_INSTALL}>:/$<CONFIG>>/howtos
     LIBRARY DESTINATION ${LIBRARY_INSTALL_DIR}/${${ROOT_PROJECT_NAME}_PLATFORM_PREFIX}$<$<BOOL:${${ROOT_PROJECT_NAME}_BREAKOUT_INSTALL}>:/$<CONFIG>>
     ARCHIVE DESTINATION lib/${${ROOT_PROJECT_NAME}_PLATFORM_PREFIX}$<$<BOOL:${${ROOT_PROJECT_NAME}_BREAKOUT_INSTALL}>:/$<CONFIG>>
  )
else()
  install(TARGETS ${TARGET_NAME} EXPORT all_howtos
     RUNTIME DESTINATION "./"
     LIBRARY DESTINATION ${LIBRARY_INSTALL_DIR}/${${ROOT_PROJECT_NAME}_PLATFORM_PREFIX}$<$<BOOL:${${ROOT_PROJECT_NAME}_BREAKOUT_INSTALL}>:/$<CONFIG>>
     ARCHIVE DESTINATION lib/${${ROOT_PROJECT_NAME}_PLATFORM_PREFIX}$<$<BOOL:${${ROOT_PROJECT_NAME}_BREAKOUT_INSTALL}>:/$<CONFIG>>
  )
endif()

  endif(BUILD_EX_${EXAMPLE_NAME})
endfunction(add_howto)

############################################################################
# Step 1:
#         Examples are almost never order dependent just add them as we  find
#         them
############################################################################
if(${ROOT_PROJECT_NAME}_BUILD_HOWTOS)
  include ( ${PROJECT_SOURCE_DIR}/cmake/cmake-common_logic.cmake )
  
  find_package(Threads REQUIRED)


  add_source_files(COMMON_HDRS LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/common-source REGEX "*.h"   SOURCE_GROUP "Headers\\Private\\")
  add_source_files(COMMON_SRCS LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/common-source REGEX "*.cpp" SOURCE_GROUP "Sources\\")
  add_source_files(COMMON_PUBLIC_HDRS LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/common-source/include REGEX  "*.h" SOURCE_GROUP  "Headers\\Public\\")
  add_library(howto-common OBJECT ${COMMON_HDRS} ${COMMON_SRCS} ${COMMON_PUBLIC_HDRS} )
  add_library(howto::common ALIAS howto-common)
   
  set_target_properties(howto-common
        PROPERTIES
        FOLDER Examples
        OUTPUT_NAME "howto-common"
        PROJECT_LABEL "howto-common"
        DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}"
        CXX_STANDARD 14
        VS_DEBUGGER_WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/runtime
        VS_DEBUGGER_ENVIRONMENT "${VS_DEBUGGER_ENVIRONMENT_LIST}"
  )
  target_link_libraries(howto-common Biogears::libbiogears)
  target_include_directories(howto-common
      PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/common-source
  )

  target_compile_definitions(howto-common  PRIVATE ${${PREFIX}_CPPFLAGS} PRIVATE $<$<PLATFORM_ID:Windows>:"_CRT_SECURE_NO_WARNINGS"> )
  target_compile_options(howto-common  PRIVATE $<$<PLATFORM_ID:Windows>:/bigobj>    PRIVATE $<$<PLATFORM_ID:Windows>:/MP${${ROOT_PROJECT_NAME}_BUILD_PARALLEL_LEVEL}>)
  
  add_custom_target(all_howtos COMMENT "Meta Target for compiling all howtos")
  set_target_properties(all_howtos PROPERTIES FOLDER "Examples" )

  childlist( dir_list ${CMAKE_CURRENT_SOURCE_DIR} )
  foreach( dir IN LISTS dir_list )
  if(NOT dir MATCHES "common-source")
    add_howto( ${dir} )
  endif()
  endforeach()
if(NOT (CMAKE_CROSSCOMPILING OR APPLE))
  install(CODE "
    include(BundleUtilities)

    function(EXELIST result curdir)
    file(REAL_PATH \${curdir} absolute_path BASE_DIRECTORY ${PROJECT_BINARY_DIR})
    file(GLOB children RELATIVE \"\${absolute_path}/\" \"\${absolute_path}/*\")
    set(dirlist \"\")
    foreach(child \${children})
      if(NOT IS_DIRECTORY \${absolute_path}/\${child} AND \${child} MATCHES \".*${CMAKE_EXECUTABLE_SUFFIX}\" )
      list(APPEND dirlist \${absolute_path}/\${child})
      set(\${result} \${dirlist} PARENT_SCOPE)
      return()
      endif()
    endforeach()
    endfunction()

    set(CONFIG_SUFFIX ${CMAKE_${CONFIG}_POSTFIX})

    set(CMAKE_PREFIX_PATH  \"${CMAKE_PREFIX_PATH}\")

    foreach(_dir IN LISTS CMAKE_PREFIX_PATH)
    list(APPEND THIRD_PARTY  \${_dir})
    list(APPEND THIRD_PARTY_LIB \${_dir}/lib)
    list(APPEND THIRD_PARTY_BIN \${_dir}/bin)
    endforeach()
		
	FILE(TO_CMAKE_PATH   \"\${CMAKE_INSTALL_PREFIX}\"  INSTALL_PATH)

	set( Biogears_RUNTIME_DESTINATION \"\${INSTALL_PATH}/bin/${${ROOT_PROJECT_NAME}_PLATFORM_PREFIX}$<$<BOOL:${${ROOT_PROJECT_NAME}_BREAKOUT_INSTALL}>:/$<CONFIG>>\" )
    set( Biogears_LIBRARY_DESTINATION \"\${INSTALL_PATH}/${LIBRARY_INSTALL_DIR}/${${ROOT_PROJECT_NAME}_PLATFORM_PREFIX}$<$<BOOL:${${ROOT_PROJECT_NAME}_BREAKOUT_INSTALL}>:/$<CONFIG>>\" )
    set( Biogears_ARCHIVE_DESTINATION \"\${INSTALL_PATH}/lib/${${ROOT_PROJECT_NAME}_PLATFORM_PREFIX}$<$<BOOL:${${ROOT_PROJECT_NAME}_BREAKOUT_INSTALL}>:/$<CONFIG>>\" )

    EXELIST(TARGET \"\${INSTALL_PATH}/bin/${${ROOT_PROJECT_NAME}_PLATFORM_PREFIX}$<$<BOOL:${${ROOT_PROJECT_NAME}_BREAKOUT_INSTALL}>:/$<CONFIG>>/howtos/\" )
    if(NOT APPLE)
      fixup_bundle(\"\${TARGET}\"
        \"\"
        \"\${THIRD_PARTY_LIB};\${THIRD_PARTY_BIN}\;\${INSTALL_PATH}/bin;\${INSTALL_PATH}/lib;\${Biogears_RUNTIME_DESTINATION};\${Biogears_LIBRARY_DESTINATION};\${Biogears_ARCHIVE_DESTINATION}\"
   
      )
    endif()
  ")
endif()
endif()
