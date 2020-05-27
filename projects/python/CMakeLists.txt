cmake_policy(SET CMP0070 NEW)

option(${ROOT_PROJECT_NAME}_WEBSITE_TOOLS "Use Python To Build Website" OFF)
if(${ROOT_PROJECT_NAME}_WEBSITE_TOOLS)
	message(STATUS "Looking for pyxbgen")
	find_program(PYXBGEN_BIN NAMES pyxbgen)
	if(NOT PYXBGEN_BIN)
		message(FATAL_ERROR "Unable to find Pyxbgen. Can not continue configuration with out it.")
	endif()
	if(NOT CMAKE_LINK_LIBRARY_SUFFIX)
		set(CMAKE_LINK_LIBRARY_SUFFIX ${CMAKE_SHARED_LIBRARY_SUFFIX})
	endif()
	function(pyxb_generate_python _pyxb_schema SRCS)
  		#
  		#ARGS
  		# _pyxb_schema   : STRING  : Name of xsd file without extension
  		# SRCS          : VARIABLE: Var will be set to the sources generated contents cleared
  		#
  		# COPY_TEMPLATE : BOOLEAN : TRUE/FALSE decides if an example xml should be copied from OUTPUT_PATH
  		#
  		# COMPONENT     : An optional prefix path to simulate the code appearing in yoru source tree.  
  		# INPUT_PATH    : PATH    : If present all files are searched for relative to this directory else CMAKE_CURRENT_SOURCE_DIR is used
  		# OUTPUT_PATH   : PATH    : If present pyxb and xml files are copied to this directory else CMAKE_BINARY_DIR is used
  		# GEN_PATH      : PATH    : If present output path of generated code CMAKE_CURRENT_BINARY_DIR is used by default
 		# PROJECT_FOLDER: STRING  : for graphical build systems this will decide what sub group the targets appear if USE_FOLDERS is ON
  		# ARGUMENTS     : list    : Arguments to pass to pyxb

    		cmake_parse_arguments(_pyxb
      		"COPY_TEMPLATE;ALWAYS_CLEAN"
		"NAMESPACE;COMPONENT;INPUT_PATH;OUTPUT_PATH;GEN_PATH;PROJECT_FOLDER"
      		"ARGUMENTS;DEPENDS"
      		${ARGN}
    		)

    		### BEGIN DEFAULT VALUES ###
    		if(NOT DEFINED _pyxb_COPY_TEMPLATE)
        		set(_pyxb_COPY_TEMPLATE OFF)
    		endif()

    		if(NOT DEFINED _pyxb_COMPONENT)
        		set(_pyxb_COMPONENT "" )
    		endif()
		
    		if(NOT DEFINED _pyxb_INPUT_PATH)
        		set(_pyxb_INPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
    		endif()

    		if(NOT DEFINED _pyxb_OUTPUT_PATH)
        		set(_pyxb_OUTPUT_PATH ${CMAKE_BINARY_DIR})
    		endif()

    		if(NOT DEFINED _pyxb_GEN_PATH)
        		set(_pyxb_GEN_PATH ${CMAKE_CURRENT_BINARY_DIR})
    		endif()
    		message(WARNING "CMAKE_CURRENT_BINARY_DIR::_pyxb_gen_path : ${CMAKE_CURRENT_BINARY_DIR}, CMAKE_CURRENT_SOURCE_DIR::_pyxb_input_path : ${CMAKE_CURRENT_SOURCE_DIR},CMAKE_BINARY_DIR::_pyxb_ouput_path : ${CMAKE_BINARY_DIR}")
    		if(NOT DEFINED _pyxb_NAMESPACE)
        		set(_pyxb_NAMESPACE ${PROJECT_NAME})
    		endif()
    		string(REPLACE "." "/" _pyxb_NAMESPACE_PATH ${_pyxb_NAMESPACE})

    		if(NOT DEFINED _pyxb_PROJECT_FOLDER)
        		set(_pyxb_PROJECT_FOLDER "Code Generators" )
    		endif()
    		### END DEFAULT VALUES ###

    		### BEGIN CONFIGURE TIME GENERATION ###


    		# Generating files at configure time assist in complete dep trees for the build system.
    		file(GLOB PYTHON_FILES "${_pyxb_GEN_PATH}/${_pyxb_COMPONENT}/${_pyxb_COMPONENT}${_pyxb_NAMESPACE_PATH}/*")
    		if( NOT EXISTS ${_pyxb_GEN_PATH}/${_pyxb_COMPONENT}/${_pyxb_COMPONENT}${_pyxb_NAMESPACE_PATH} OR NOT PYTHON_FILES)
			message(WARNING "Generating ${_pyxb_schema}.py")
      			execute_process(
                     			WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
					COMMAND ${CMAKE_COMMAND} -E make_directory ${_pyxb_GEN_PATH}/${_pyxb_COMPONENT}/${_pyxb_COMPONENT}${_pyxb_NAMESPACE_PATH}
					COMMAND ${CMAKE_COMMAND} -E env  ${PYXBGEN_BIN} -u ${_pyxb_INPUT_PATH}/${_pyxb_schema}.xsd -m ${_pyxb_GEN_PATH}/${_pyxb_NAMESPACE_PATH}/Biogears
                     			OUTPUT_VARIABLE PYXB_OUTPUT

                    		)

      		if(PYXB_OUTPUT MATCHES ".*\\[ERROR\\].*" )
        		message(FATAL_ERROR "pyxb Msg:\n"
                          "${PYXB_OUTPUT}\n"
             		)
      		elseif(PYXB_OUTPUT MATCHES ".*\\[WARNING\\].*" )
        		message(WARNING "pyxb Msg:\n"
                            		"${PYXB_OUTPUT}\n"
               		)
      		endif()
    		endif()

    		if (  _pyxb_COPY_TEMPLATE AND (NOT EXISTS ${_pyxb_OUTPUT_PATH}/${_pyxb_schema}.template.xml OR NOT EXISTS ${_pyxb_OUTPUT_PATH}/${_pyxb_schema}.xsd) )
      			message(STATUS "Staging ${_pyxb_schema}.template.xml")
      			execute_process(
                    		WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                     		COMMAND ${CMAKE_COMMAND} -E make_directory ${_pyxb_OUTPUT_PATH}
                     		COMMAND ${CMAKE_COMMAND} -E copy ${_pyxb_INPUT_PATH}/${_pyxb_schema}.xml ${_pyxb_OUTPUT_PATH}/${_pyxb_schema}.template.xml
                     		COMMAND ${CMAKE_COMMAND} -E copy ${_pyxb_INPUT_PATH}/${_pyxb_schema}.xsd ${_pyxb_OUTPUT_PATH}/${_pyxb_schema}.xsd
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
    		file(GLOB PYTHON_FILES "${_pyxb_GEN_PATH}/${_pyxb_COMPONENT}/${_pyxb_COMPONENT}${_pyxb_NAMESPACE_PATH}/*")
    		set(${SRCS} ${PYTHON_FILES} PARENT_SCOPE)

    		### BEGIN BUILD TIME GENERATION ###
    		# Generating at build time assist in up todate code Generation.
    		# Becareful most build systems will not detect new files until the next build

    		#XSDs can generate multiple files, but we are only verifying the master file exist.
    		add_custom_command(OUTPUT  ${PYTHON_FILES}
                          WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                          COMMAND ${CMAKE_COMMAND} -E make_directory ${_pyxb_GEN_PATH}/${_pyxb_COMPONENT}/${_pyxb_COMPONENT}${_pyxb_NAMESPACE_PATH}
                          COMMAND ${CMAKE_COMMAND} -E env  ${PYXBGEN_BIN} -u ${_pyxb_INPUT_PATH}/${_pyxb_schema}.xsd -m ${_pyxb_GEN_PATH}/${_pyxb_NAMESPACE_PATH}/Biogears
                          DEPENDS ${_pyxb_INPUT_PATH}/${_pyxb_schema}.xsd
                          DEPENDS ${_pyxb_DEPENDS}
                          COMMENT "Generating ${_pyxb_schema}.py" )

    		add_custom_target( pyxb_gen_${_pyxb_COMPONENT}_${_pyxb_schema} DEPENDS  "${PYTHON_FILES}"
                         COMMENT "Checking if re-generation is required" )

    		set_target_properties(pyxb_gen_${_pyxb_COMPONENT}_${_pyxb_schema}
                          PROPERTIES
                          FOLDER ${_pyxb_PROJECT_FOLDER}
                          PROJECT_LABEL "pyxb_gen_${_pyxb_COMPONENT}_${_pyxb_schema}")

    		if (  _pyxb_COPY_TEMPLATE AND NOT EXISTS ${_pyxb_OUTPUT_PATH}/${_pyxb_schema}.template.xml )
      			add_custom_command( OUTPUT  ${CMAKE_BINARY_DIR}/${CMAKE_BINARY_DIR}/${_pyxb_schema}.xsd ${CMAKE_BINARY_DIR}/${resource_path}/${_pyxb_schema}.template.xml
                          WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                          COMMAND ${CMAKE_COMMAND} -E copy ${_pyxb_INPUT_PATH}/${_pyxb_schema}.xml ${_pyxb_OUTPUT_PATH}/${_pyxb_schema}.template.xml
                          COMMAND ${CMAKE_COMMAND} -E copy ${_pyxb_INPUT_PATH}/${_pyxb_schema}.xsd ${_pyxb_OUTPUT_PATH}/${_pyxb_schema}.xsd
                          DEPENDS ${_pyxb_OUTPUT_PATH}/${_pyxb_schema}.template.xml ${_pyxb_OUTPUT_PATH}/${_pyxb_schema}.xsd

                          COMMENT "Staging ${_pyxb_schema}.template.xml" )

      			add_custom_target( pyxb_stage_${_pyxb_COMPONENT}_${_pyxb_schema} DEPENDS  ${_pyxb_OUTPUT_PATH}/${_pyxb_schema}.xsd ${_pyxb_OUTPUT_PATH}/${_pyxb_schema}.template.xml
                         COMMENT "Checking if staging is required" )

      			set_target_properties(pyxb_stage_${_pyxb_COMPONENT}_${_pyxb_schema}
                            		PROPERTIES
                            		FOLDER ${_pyxb_PROJECT_FOLDER}
                            		PROJECT_LABEL "pyxb_stage_${_pyxb_COMPONENT}_${_pyxb_schema}")
    		endif()


    		## END BUILD TIME GNERATION ###
		endfunction(pyxb_generate_python)
	endif()

if(${ROOT_PROJECT_NAME}_WEBSITE_TOOLS) 
       add_subdirectory(schema)
       add_subdirectory(biogears)
       add_subdirectory(cdm) 
endif()

message(STATUS " ")
