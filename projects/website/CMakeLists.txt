###############################################################################
# Policy adjustments
###############################################################################

cmake_minimum_required(VERSION 3.8.0)
cmake_policy(VERSION 3.8.0)

###############################################################################
# Options
###############################################################################

option(Biogears_BUILD_WEBSITE "Adds targets to assist in compiling documentation for the biogears website (Requires Python,Latex,Doxygen,and Perl" OFF)
if(Biogears_BUILD_WEBSITE)
    ###############################################################################
    # Base Variables
    ###############################################################################

    ###############################################################################
    # Requirements
    ###############################################################################

    #find_package(Perl REQUIRED)
    find_package(Doxygen REQUIRED)
    find_package (Python3 COMPONENTS Interpreter Development REQUIRED )
    find_package(LATEX COMPONENTS BIBTEX REQUIRED)  

    ###############################################################################
    #Virtual Environment
    ###############################################################################

    find_program(VIRTUALENV virtualenv)

    if(UNIX)
      set(PYTHON_SCRIPT_DIR "bin")
    else()
      set(PYTHON_SCRIPT_DIR "Scripts")
    endif()

    if(NOT VIRTUALENV)
        message(FATAL_ERROR "Could not find `virtualenv` in PATH")
    endif()

    add_custom_command(
        OUTPUT venv
        COMMAND ${VIRTUALENV} -p ${Python3_EXECUTABLE} venv
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/runtime
        )

    add_custom_command(
        OUTPUT venv.stamp
        DEPENDS venv requirements.txt
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/requirements.txt requirements.txt
        COMMAND venv/${PYTHON_SCRIPT_DIR}/pip${CMAKE_EXECUTABLE_SUFFIX} install -r requirements.txt --upgrade
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/runtime
        )

    add_custom_target(venvcreate ALL
        DEPENDS venv.stamp
        SOURCES requirements.txt
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/runtime
        )

    ###############################################################################
    #Sorce and Header Defines
    ###############################################################################

    message(STATUS "Configuring www")
    configure_file(${PROJECT_SOURCE_DIR}/share/doc/doxygen/doxyfile.in ${PROJECT_BINARY_DIR}/runtime/doc/doxygen/Doxyfile @ONLY)

    ###############################################################################
    #Define Logic
    ###############################################################################

    set(CDM2MD_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/doxygen/CDM2MD.py)
    set(DoxygenPreprocessor_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/doxygen/DoxygenPreprocessor.py)
    set(XSDToDOXYGEN_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/doxygen/XSDToDoxygen.py)
    set(Plot_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/plots/PlotDriver.py)

    ###############################################################################
    # COMPILATION & LINKAGE MODIFICATIONS
    ###############################################################################


    message(STATUS "Python_ver: ${Python3_EXECUTABLE}")
    add_custom_target(docs-prepare-py
        COMMAND venv/${PYTHON_SCRIPT_DIR}/python${CMAKE_EXECUTABLE_SUFFIX}  ${CDM2MD_SOURCE} -p xsd/BioGearsDataModel.xsd -f doc/doxygen/processed_md#cdm
        COMMAND venv/${PYTHON_SCRIPT_DIR}/python${CMAKE_EXECUTABLE_SUFFIX}  ${DoxygenPreprocessor_SOURCE} -p doc/markdown -f doc/doxygen/processed_md #cdm
        COMMAND venv/${PYTHON_SCRIPT_DIR}/python${CMAKE_EXECUTABLE_SUFFIX}  ${DoxygenPreprocessor_SOURCE}  -p doc/methodology -f doc/doxygen/processed_md #cdm
        COMMAND venv/${PYTHON_SCRIPT_DIR}/python${CMAKE_EXECUTABLE_SUFFIX}  ${XSDToDOXYGEN_SOURCE} -p xsd/BioGearsDataModel.xsd -f doc/doxygen/processed_md #cdm 
        COMMAND venv/${PYTHON_SCRIPT_DIR}/python${CMAKE_EXECUTABLE_SUFFIX}  ${Plot_SOURCE}  -b ${Biogears_Baseline_DIR} -p PlotRun-1_of_2.config -j 5 -v
        COMMAND venv/${PYTHON_SCRIPT_DIR}/python${CMAKE_EXECUTABLE_SUFFIX}  ${Plot_SOURCE}  -b ${Biogears_Baseline_DIR} -p PlotRun-2_of_2.config -j 5 -v
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/runtime
        VERBATIM
        USES_TERMINAL
    )
    set_target_properties(docs-prepare-py PROPERTIES
        FOLDER "Website"
        EXCLUDE_FROM_ALL ON
        COMMENT "Running docs-prepare"
    )
    add_custom_target(docs-doxygen-py
        COMMAND ${DOXYGEN_EXECUTABLE} Doxyfile
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/runtime/doc/doxygen
        COMMENT "Running docs-doxygen"
        VERBATIM
        USES_TERMINAL
    )
    set_target_properties(docs-doxygen-py PROPERTIES
        FOLDER "Website"
        EXCLUDE_FROM_ALL ON
        COMMENT "Running docs-prepare"
    )
    add_custom_target(docs-stage-py
        COMMAND ${CMAKE_COMMAND} -E  make_directory output/documentation
        COMMAND ${CMAKE_COMMAND} -E copy_directory doxygen/html/ output/documentation
        COMMAND ${CMAKE_COMMAND} -E tar cfv output/documentation.zip  --format=zip  -- output/documentation
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/runtime/doc/
        VERBATIM
        COMMENT "Running docs-stage"
        USES_TERMINAL
   )
   set_target_properties(docs-stage-py PROPERTIES
        FOLDER "Website"
        EXCLUDE_FROM_ALL ON
        COMMENT "Running docs-prepare"
    )
    if(Biogears_BUILD_WEBSITE_DEBUG)    
       add_custom_target(docs-prepare-cdm2md
           COMMAND ${Python3_EXECUTABLE}  ${CDM2MD_SOURCE} -p xsd/BioGearsDataModel.xsd -f doc/doxygen/processed_md #cdm
           WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/runtime
           VERBATIM
           USES_TERMINAL
       )
       set_target_properties(docs-prepare-cdm2md PROPERTIES
           FOLDER "Website"
           EXCLUDE_FROM_ALL ON
           COMMENT "Running docs-prepare"
       )
       add_custom_target(docs-prepare-preprocess-markdown
           COMMAND ${Python3_EXECUTABLE}  ${DoxygenPreprocessor_SOURCE} -p doc/markdown -f doc/doxygen/processed_md
           WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/runtime
           VERBATIM
           USES_TERMINAL
         )
          set_target_properties(docs-prepare-preprocess-markdown PROPERTIES
           FOLDER "Website"
           EXCLUDE_FROM_ALL ON
           COMMENT "Running docs-prepare"
       )
       add_custom_target(docs-prepare-preprocess-methodolgy
           COMMAND ${Python3_EXECUTABLE}  ${DoxygenPreprocessor_SOURCE} -p doc/methodology -f doc/doxygen/processed_md
           WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/runtime
           VERBATIM
           USES_TERMINAL
         )
          set_target_properties(docs-prepare-preprocess-methodolgy PROPERTIES
           FOLDER "Website"
           EXCLUDE_FROM_ALL ON
           COMMENT "Running docs-prepare"
       )
       add_custom_target(docs-prepare-xsd2doxygen
           COMMAND ${Python3_EXECUTABLE}  ${XSDToDOXYGEN_SOURCE} -p xsd/BioGearsDataModel.xsd -f doc/doxygen/processed_md #cdm
           WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/runtime
           VERBATIM
           USES_TERMINAL
       )
       set_target_properties(docs-prepare-xsd2doxygen PROPERTIES
           FOLDER "Website"
           EXCLUDE_FROM_ALL ON
           COMMENT "Running docs-prepare"
       )
    endif()
    add_dependencies(docs-prepare-py prepare_runtime_dir bg-cli venvcreate )
    add_dependencies(docs-doxygen-py docs-prepare-py )
    add_dependencies(docs-stage-py   docs-doxygen-py )
    if(TARGET prepare_runtime_dir AND Biogears_PREPARE_RUNTIME_DIR)
        add_dependencies(docs-prepare-py prepare_runtime_dir )
        if (Biogears_BUILD_WEBSITE_DEBUG)
            add_dependencies(docs-prepare-cdm2md prepare_runtime_dir )
            add_dependencies(docs-prepare-preprocess-markdown prepare_runtime_dir )
            add_dependencies(docs-prepare-preprocess-methodolgy prepare_runtime_dir )
            add_dependencies(docs-prepare-xsd2doxygen prepare_runtime_dir )
        endif()
    endif()
endif()
