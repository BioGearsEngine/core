option(${ROOT_PROJECT_NAME}_BUILD_HOWTOS     "Select to build sdk how to examples." ON)
cmake_policy(SET CMP0087 NEW)
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
      ${CMAKE_THREAD_LIBS_INIT}
      Threads::Threads
      howto::common
    )
      
    target_link_libraries(${TARGET_NAME} ${${EXAMPLE_NAME}_LIBS} )
	  if(TARGET stage_runtime AND ${ROOT_PROJECT_NAME}_STAGE_RUNTIME) 
      add_dependencies(${TARGET_NAME} stage_runtime)
    endif()
   
    add_dependencies(howtos ${TARGET_NAME})
     
	install(TARGETS ${TARGET_NAME} EXPORT howtos
     RUNTIME DESTINATION bin/howtos
     LIBRARY DESTINATION ${LIBRARY_INSTALL_DIR}
     ARCHIVE DESTINATION lib
  )

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
    set(CMAKE_PDB_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER}    "${OUTPUT_PREFIX}/${OUTPUTCONFIG}/lib")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY${OUTPUTCONFIG_UPPER} "${OUTPUT_PREFIX}/${OUTPUTCONFIG}/bin")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY${OUTPUTCONFIG_UPPER} "${OUTPUT_PREFIX}/${OUTPUTCONFIG}/bin")
  endforeach(OUTPUTCONFIG)
endif()
############################################################################
# Step 1:
#         Examples are almost never order dependent just add them as we  find
#         them
############################################################################
if(${ROOT_PROJECT_NAME}_BUILD_HOWTOS)
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

  target_compile_definitions(howto-common  PRIVATE ${${PREFIX}_CPPFLAGS} PRIVATE $<$<PLATFORM_ID:Windows>:"_CRT_SECURE_NO_WARNINGS"> )
  target_compile_options(howto-common  PRIVATE $<$<PLATFORM_ID:Windows>:/bigobj>    PRIVATE $<$<PLATFORM_ID:Windows>:/MP${${ROOT_PROJECT_NAME}_BUILD_PARALLEL_LEVEL}>)
  
  add_custom_target(howtos COMMENT "Meta Target for compiling all howtos")
  
  childlist( dir_list ${CMAKE_CURRENT_SOURCE_DIR} )
  foreach( dir IN LISTS dir_list )
  if(NOT dir MATCHES "common-source")
    add_howto( ${dir} )
  endif()
  endforeach()
  install(CODE "
		include(BundleUtilities)

		function(EXELIST result curdir)
		file(GLOB children RELATIVE \${curdir} \${curdir}/*)
		set(dirlist \"\")
		foreach(child \${children})
			if(NOT IS_DIRECTORY \${curdir}/\${child} AND \${child} MATCHES \".*${CMAKE_EXECUTABLE_SUFFIX}\" )
			list(APPEND dirlist \${curdir}/\${child})
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


		EXELIST(TARGET ${CMAKE_INSTALL_PREFIX}/bin/howtos )

		fixup_bundle(\${TARGET}
						\"\"
						\"\${THIRD_PARTY_LIB};\${THIRD_PARTY_BIN}\;${CMAKE_LIBRARY_OUTPUT_DIRECTORY};${CMAKE_ARCHIVE_OUTPUT_DIRECTORY$};${CMAKE_RUNTIME_OUTPUT_DIRECTORY}\"
   
		)
	")
endif()

message(STATUS "")
