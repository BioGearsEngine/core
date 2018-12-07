cmake_policy(SET CMP0070 NEW)
include(UseJava)

find_program(Java_XML_EXECUTABLE
  NAMES xjc
  HINTS ${_JAVA_HINTS}
  PATHS ${_JAVA_PATHS}
)

if(NOT CMAKE_LINK_LIBRARY_SUFFIX)
  set(CMAKE_LINK_LIBRARY_SUFFIX ${CMAKE_SHARED_LIBRARY_SUFFIX})
endif()
function(xjc_generate_java _xjc_schema SRCS)
  #Assumes a file of ${XML_SCHEMA}.XJC exist that must be compiled using xjc
  #
  #ARGS
  # _xjc_schema   : STRING  : Name of xsd file without extension
  # SRCS          : VARIABLE: Var will be set to the sources generated contents cleared
  #
  # COPY_TEMPLATE : BOOLEAN : TRUE/FALSE decides if an example xml should be copied from OUTPUT_PATH
  #
  # COMPONENT     : An optional prefix path to simulate the code appearing in yoru source tree.  
  # INPOUT_PATH   : PATH    : If present all files are searched for relative to this directory else CMAKE_CURRENT_SOURCE_DIR is used
  # OUTPUT_PATH   : PATH    : If present XJC and xml files are copied to this directory else CMAKE_BINARY_DIR is used
  # GEN_PATH      : PATH    : If present output path of generated code CMAKE_CURRENT_BINARY_DIR is used by default
  # NAMESPACE     : STRING  : given to the xjc -p flag defaults to PROJECT_NAME
  # PROJECT_FOLDER: STRING  : for graphical build systems this will decide what sub group the targets appear if USE_FOLDERS is ON
  # ARGUMENTS     : list    : Arguments to pass to xjc

    cmake_parse_arguments(_xjc
      "COPY_TEMPLATE;ALWAYS_CLEAN"
      "NAMESPACE;COMPONENT;INPUT_PATH;OUTPUT_PATH;GEN_PATH;PROJECT_FOLDER"
      "ARGUMENTS;DEPENDS"
      ${ARGN}
    )

    ### BEGIN DEFAULT VALUES ###
    if(NOT DEFINED _xjc_COPY_TEMPLATE)
        set(_xjc_COPY_TEMPLATE OFF)
    endif()

    if(NOT DEFINED _xjc_COMPONENT)
        set(_xjc_COMPONENT "" )
    endif()

    if(NOT DEFINED _xjc_INPUT_PATH)
        set(_xjc_INPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
    endif()

    if(NOT DEFINED _xjc_OUTPUT_PATH)
        set(_xjc_OUTPUT_PATH ${CMAKE_BINARY_DIR})
    endif()

    if(NOT DEFINED _xjc_GEN_PATH)
        set(_xjc_GEN_PATH ${CMAKE_CURRENT_BINARY_DIR})
    endif()

    if(NOT DEFINED _xjc_NAMESPACE)
        set(_xjc_NAMESPACE ${PROJECT_NAME})
    endif()
    string(REPLACE "." "/" _xjc_NAMESPACE_PATH ${_xjc_NAMESPACE})

    if(NOT DEFINED _xjc_PROJECT_FOLDER)
        set(_xjc_PROJECT_FOLDER "Code Generators" )
    endif()
    ### END DEFAULT VALUES ###

    ### BEGIN CONFIGURE TIME GENERATION ###


    # Generating files at configure time assist in complete dep trees for the build system.
    file(GLOB JAVA_FILES "${_xjc_GEN_PATH}/${_xjc_COMPONENT}/${_xjc_COMPONENT}${_xjc_NAMESPACE_PATH}/*")
    if( NOT EXISTS ${_xjc_GEN_PATH}/${_xjc_COMPONENT}/${_xjc_COMPONENT}${_xjc_NAMESPACE_PATH} OR NOT JAVA_FILES) 
      message(STATUS "Generating ${_xjc_schema}.java")
      execute_process( 
                     WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                     COMMAND ${CMAKE_COMMAND} -E make_directory ${_xjc_GEN_PATH}/${_xjc_COMPONENT}/${_xjc_COMPONENT}${_xjc_NAMESPACE_PATH}
                     COMMAND ${CMAKE_COMMAND} -E env  ${Java_XML_EXECUTABLE} -d ${_xjc_GEN_PATH}/${_xjc_COMPONENT} -p ${_xjc_NAMESPACE} ${_xjc_ARUGMENTS} ${_xjc_INPUT_PATH}/${_xjc_schema}.xsd 
                     OUTPUT_VARIABLE XJC_OUTPUT
                     
                    )

      if(XJC_OUTPUT MATCHES ".*\\[ERROR\\].*" )
        message(FATAL_ERROR "xjc Msg:\n"
                          "${XJC_OUTPUT}\n"
             )
      elseif(XJC_OUTPUT MATCHES ".*\\[WARNING\\].*" )
        message(WARNING "xjc Msg:\n"
                            "${XJC_OUTPUT}\n"
               )
      endif() 
    endif()
    
    if (  _xjc_COPY_TEMPLATE AND (NOT EXISTS ${_xjc_OUTPUT_PATH}/${_xjc_schema}.template.xml OR NOT EXISTS ${_xjc_OUTPUT_PATH}/${_xjc_schema}.xsd) )
      message(STATUS "Staging ${_xjc_schema}.template.xml")   
      execute_process( 
                    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                     COMMAND ${CMAKE_COMMAND} -E make_directory ${_xjc_OUTPUT_PATH}
                     COMMAND ${CMAKE_COMMAND} -E copy ${_xjc_INPUT_PATH}/${_xjc_schema}.xml ${_xjc_OUTPUT_PATH}/${_xjc_schema}.template.xml
                     COMMAND ${CMAKE_COMMAND} -E copy ${_xjc_INPUT_PATH}/${_xjc_schema}.xsd ${_xjc_OUTPUT_PATH}/${_xjc_schema}.xsd   
                     ERROR_VARIABLE COPY_ERROR
                     OUTPUT_QUIET
                    )
      if(COPY_ERROR)
        message(FATAL_ERROR "File copy error"
                            "${COPY_ERROR}\n"
               )
      endif(COPY_ERROR)
    endif()
    ### END CONFIGURE TIME GENRATION ###
    file(GLOB JAVA_FILES "${_xjc_GEN_PATH}/${_xjc_COMPONENT}/${_xjc_COMPONENT}${_xjc_NAMESPACE_PATH}/*")
    set(${SRCS} ${JAVA_FILES} PARENT_SCOPE)

    ### BEGIN BUILD TIME GENERATION ###
    # Generating at build time assist in up todate code Generation.
    # Becareful most build systems will not detect new files until the next build

    #XSDs can generate multiple files, but we are only verifying the master file exist.
    
    add_custom_command(   OUTPUT  ${JAVA_FILES}
                          WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                          COMMAND ${CMAKE_COMMAND} -E make_directory ${_xjc_GEN_PATH}/${_xjc_COMPONENT}/${_xjc_COMPONENT}${_xjc_NAMESPACE_PATH} 
                          COMMAND ${CMAKE_COMMAND} -E env  ${Java_XML_EXECUTABLE} -d ${_xjc_GEN_PATH}/${_xjc_COMPONENT} -p ${_xjc_NAMESPACE} ${_xjc_ARUGMENTS} ${_xjc_INPUT_PATH}/${_xjc_schema}.xsd 
                          DEPENDS ${_xjc_INPUT_PATH}/${_xjc_schema}.xsd
                          DEPENDS ${_xjc_DEPENDS}
                          COMMENT "Generating ${_xjc_schema}.java" )

    add_custom_target( xjc_gen_${_xjc_COMPONENT}_${_xjc_schema} DEPENDS  "${JAVA_FILES}"
                         COMMENT "Checking if re-generation is required" )

    set_target_properties(xjc_gen_${_xjc_COMPONENT}_${_xjc_schema}
                          PROPERTIES
                          FOLDER ${_xjc_PROJECT_FOLDER}
                          PROJECT_LABEL "xjc_gen_${_xjc_COMPONENT}_${_xjc_schema}")
    
    if (  _xjc_COPY_TEMPLATE AND NOT EXISTS ${_xjc_OUTPUT_PATH}/${_xjc_schema}.template.xml )
      add_custom_command( OUTPUT  ${CMAKE_BINARY_DIR}/${CMAKE_BINARY_DIR}/${_xjc_schema}.xsd ${CMAKE_BINARY_DIR}/${resource_path}/${_xjc_schema}.template.xml     
                          WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                          COMMAND ${CMAKE_COMMAND} -E copy ${_xjc_INPUT_PATH}/${_xjc_schema}.xml ${_xjc_OUTPUT_PATH}/${_xjc_schema}.template.xml
                          COMMAND ${CMAKE_COMMAND} -E copy ${_xjc_INPUT_PATH}/${_xjc_schema}.xsd ${_xjc_OUTPUT_PATH}/${_xjc_schema}.xsd
                          DEPENDS ${_xjc_OUTPUT_PATH}/${_xjc_schema}.template.xml ${_xjc_OUTPUT_PATH}/${_xjc_schema}.xsd

                          COMMENT "Staging ${_xjc_schema}.template.xml" )

      add_custom_target( xjc_stage_${_xjc_COMPONENT}_${_xjc_schema} DEPENDS  ${_xjc_OUTPUT_PATH}/${_xjc_schema}.xsd ${_xjc_OUTPUT_PATH}/${_xjc_schema}.template.xml     
                         COMMENT "Checking if staging is required" )

      set_target_properties(xjc_stage_${_xjc_COMPONENT}_${_xjc_schema}
                            PROPERTIES
                            FOLDER ${_xjc_PROJECT_FOLDER}
                            PROJECT_LABEL "xjc_stage_${_xjc_COMPONENT}_${_xjc_schema}")
    endif()


    ## END BUILD TIME GNERATION ###
endfunction(xjc_generate_java)


set(CMAKE_JAVA_TARGET_OUTPUT_DIR ${CMAKE_BINARY_DIR}/outputs/jar)
set(CMAKE_JAVA_COMPILE_FLAGS -nowarn)

if(Java_FOUND AND ${ROOT_PROJECT_NAME}_BUILD_JAVATOOLS)
	add_subdirectory(schema)
	add_subdirectory(cdm)
	add_subdirectory(engine)
	add_subdirectory(gui)
	add_subdirectory(howto)

  #We need to patch CMAKE To make a distributable jar 
  #CMAKE needs an add_distributable_jar which compiles no files but simply merges
  #all jar files and sets and entry_point
  
  add_subdirectory(biogears)
endif()

message(STATUS "")
