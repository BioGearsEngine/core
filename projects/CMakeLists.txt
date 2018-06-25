cmake_minimum_required(VERSION 3.8.0)

enable_language(CXX)

option(ARA_${ROOT_PROJECT_NAME}_BUILD_STATIC  "Build ${ROOT_PROJECT_NAME} as a static libraries" OFF)
option(ARA_${ROOT_PROJECT_NAME}_BUILD_DOCUMENTATION "Create and install the HTML based API documentation (requires Doxygen)" OFF)
find_path (
  ARA_${ROOT_PROJECT_NAME}_Verification_DIR
  NAME Scenarios
  PATHS ${PROJECT_SOURCE_DIR}/../verification
  DOC "Optional directory path to a hiarchial verification filetree of baseline archives. This will be used by Java Utilities to find Baselines"
)
###############################################################################
# Step 1: Some packages need to be configured globally to ensure they find the 
#         right library all others should be called in the respective projects
#         to promote some independence
###############################################################################
include(ProcessorCount)
ProcessorCount(N)
if(NOT N EQUAL 0)
  math(EXPR PARALLEL_JOBS ${N}/2)
  set(ARA_${ROOT_PROJECT_NAME}_PARALLEL_JOBS ${PARALLEL_JOBS} CACHE STRING "Number of Parallel Jobs for IDEs if supported")
endif()

if(WIN32)
  set(LIBRARY_INSTALL_DIR bin)
  add_compile_options("/MP${ARA_${ROOT_PROJECT_NAME}_PARALLEL_JOBS}")
else()
  set(LIBRARY_INSTALL_DIR lib)
endif()

set(CMAKE_STATIC_LIBRARY_PREFIX "lib")
set(CMAKE_STATIC_LIBRARY_SUFFIX "_st")
set(CMAKE_SHARED_LIBRARY_PREFIX "lib")

  
set(OUTPUT_PREFIX ${CMAKE_BINARY_DIR}/outputs)
if(UNIX)
    foreach(OUTPUTCONFIG Release Debug)
        string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG_UPPER)
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} "${OUTPUT_PREFIX}/lib")
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} "${OUTPUT_PREFIX}/lib")
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} "${OUTPUT_PREFIX}/bin")
    endforeach(OUTPUTCONFIG)
else()
    foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
        string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG_UPPER)
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} "${OUTPUT_PREFIX}/${OUTPUTCONFIG}/lib")
        set(CMAKE_PDB_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} "${OUTPUT_PREFIX}/${OUTPUTCONFIG}/lib")
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} "${OUTPUT_PREFIX}/${OUTPUTCONFIG}/bin")
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} "${OUTPUT_PREFIX}/${OUTPUTCONFIG}/bin")
    endforeach(OUTPUTCONFIG)
endif()
###############################################################################
# Step 2: Project Includes are generally order dependent.  So manually maintain
#         this list
###############################################################################

find_package(Java)
option(ARA_${ROOT_PROJECT_NAME}_BUILD_JAVATOOLS     "Build Java based development tools (requires Doxygen)" ${Java_FOUND})

add_subdirectory(biogears)
add_subdirectory(scenario_driver)
add_subdirectory(test_driver)
add_subdirectory(howto)
add_subdirectory(unit)


if(Java_FOUND AND ARA_${ROOT_PROJECT_NAME}_BUILD_JAVATOOLS)
  add_subdirectory(java)
endif()

###############################################################################
# Step 3: Global Doxygen configuration 
#         you might need to tweak this if you have multiple doxy files
###############################################################################
if(ARA_${ROOT_PROJECT_NAME}_BUILD_DOCUMENTATION)
    find_package(Doxygen REQUIRED)
    if(NOT DOXYGEN_FOUND)
        message(FATAL_ERROR "Doxygen is needed to build the documentation.")
    endif()

    set(doxyfile_in ${CMAKE_SOURCE_DIR}/share/doc/cmake-common.doxy)
    set(doxyfile ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    configure_file(${doxyfile_in} ${doxyfile} @ONLY)

    add_custom_target(doc
        COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/share/doc/
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM)

   set_target_properties( doc PROPERTIES
                          DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX}
                          CXX_STANDARD 11
                          FOLDER "Other"
                          PROJECT_LABEL "Doxygen")

install(DIRECTORY ${CMAKE_SOURCE_DIR}/share/doc/html DESTINATION share/doc/kaleidoscppe/)
endif()

add_custom_command(
    OUTPUT  ${PROJECT_BINARY_DIR}/runtime 
    COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/runtime
)

add_custom_command(
	OUTPUT ${PROJECT_BINARY_DIR}/runtime/xsd
	DEPENDS ${PROJECT_BINARY_DIR}/runtime 
	${PROJECT_SOURCE_DIR}/share/xsd/BioGearsDataModel.xsd ${PROJECT_SOURCE_DIR}/share/xsd/CommonDataModel.xsd
	${PROJECT_SOURCE_DIR}/share/xsd/BioGearsConfiguration.xsd  ${PROJECT_SOURCE_DIR}/share/xsd/BioGearsEnvironment.xsd    ${PROJECT_SOURCE_DIR}/share/xsd/BioGearsEquipment.xsd   ${PROJECT_SOURCE_DIR}/share/xsd/BioGearsPhysiology.xsd  ${PROJECT_SOURCE_DIR}/share/xsd/BioGears.xsd
	${PROJECT_SOURCE_DIR}/share/xsd/cAnesthesiaActions.xsd     ${PROJECT_SOURCE_DIR}/share/xsd/EngineConfiguration.xsd    ${PROJECT_SOURCE_DIR}/share/xsd/InhalerActions.xsd      ${PROJECT_SOURCE_DIR}/share/xsd/PatientNutrition.xsd    ${PROJECT_SOURCE_DIR}/share/xsd/SubstanceQuantity.xsd
	${PROJECT_SOURCE_DIR}/share/xsd/Anesthesia.xsd             ${PROJECT_SOURCE_DIR}/share/xsd/EngineState.xsd            ${PROJECT_SOURCE_DIR}/share/xsd/Inhaler.xsd             ${PROJECT_SOURCE_DIR}/share/xsd/Patient.xsd             ${PROJECT_SOURCE_DIR}/share/xsd/Substance.xsd
	${PROJECT_SOURCE_DIR}/share/xsd/Circuit.xsd                ${PROJECT_SOURCE_DIR}/share/xsd/EnvironmentActions.xsd     ${PROJECT_SOURCE_DIR}/share/xsd/PatientActions.xsd      ${PROJECT_SOURCE_DIR}/share/xsd/Physiology.xsd          ${PROJECT_SOURCE_DIR}/share/xsd/System.xsd
	${PROJECT_SOURCE_DIR}/share/xsd/Compartment.xsd            ${PROJECT_SOURCE_DIR}/share/xsd/EnvironmentConditions.xsd  ${PROJECT_SOURCE_DIR}/share/xsd/PatientAssessments.xsd  ${PROJECT_SOURCE_DIR}/share/xsd/Properties.xsd          ${PROJECT_SOURCE_DIR}/share/xsd/TestReport.xsd
	${PROJECT_SOURCE_DIR}/share/xsd/ElectroCardioGram.xsd      ${PROJECT_SOURCE_DIR}/share/xsd/Environment.xsd            ${PROJECT_SOURCE_DIR}/share/xsd/PatientConditions.xsd   ${PROJECT_SOURCE_DIR}/share/xsd/Scenario.xsd
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/share/xsd  ${PROJECT_BINARY_DIR}/runtime/xsd
)
add_custom_command(
	OUTPUT ${PROJECT_BINARY_DIR}/runtime/jar
	DEPENDS ${PROJECT_BINARY_DIR}/runtime 
	${PROJECT_SOURCE_DIR}/share/jar/commons-logging-1.2.jar  ${PROJECT_SOURCE_DIR}/share/jar/javassist-3.16.1-GA.jar  ${PROJECT_SOURCE_DIR}/share/jar/jsr305-1.3.9.jar                     ${PROJECT_SOURCE_DIR}/share/jar/slf4j-api-1.6.1.jar
    ${PROJECT_SOURCE_DIR}/share/jar/dom4j-1.6.1.jar          ${PROJECT_SOURCE_DIR}/share/jar/javax.mail-1.6.1.jar     ${PROJECT_SOURCE_DIR}/share/jar/log4j-1.2.17.jar                     ${PROJECT_SOURCE_DIR}/share/jar/slf4j-simple-1.6.1.jar
    ${PROJECT_SOURCE_DIR}/share/jar/dsn.jar                  ${PROJECT_SOURCE_DIR}/share/jar/jchart2d-3.2.2.jar       ${PROJECT_SOURCE_DIR}/share/jar/pdfbox-2.0.0-RC3.jar                 ${PROJECT_SOURCE_DIR}/share/jar/smtp.jar
    ${PROJECT_SOURCE_DIR}/share/jar/fontbox-2.0.0-RC3.jar    ${PROJECT_SOURCE_DIR}/share/jar/jcommon-1.0.16.jar       ${PROJECT_SOURCE_DIR}/share/jar/poi-3.13-20150929.jar                ${PROJECT_SOURCE_DIR}/share/jar/swtgraphics2d.jar
    ${PROJECT_SOURCE_DIR}/share/jar/gson-1.4.jar             ${PROJECT_SOURCE_DIR}/share/jar/jdom-1.1.2.jar           ${PROJECT_SOURCE_DIR}/share/jar/poi-ooxml-3.13-20150929.jar          ${PROJECT_SOURCE_DIR}/share/jar/xml-apis-1.0.b2.jar
    ${PROJECT_SOURCE_DIR}/share/jar/guava-11.0.2.jar         ${PROJECT_SOURCE_DIR}/share/jar/jdom-2.0.2.jar           ${PROJECT_SOURCE_DIR}/share/jar/poi-ooxml-schemas-3.13-20150929.jar  ${PROJECT_SOURCE_DIR}/share/jar/xmlbeans-2.6.0.jar
    ${PROJECT_SOURCE_DIR}/share/jar/imap.jar                 ${PROJECT_SOURCE_DIR}/share/jar/jfreechart-1.0.13.jar    ${PROJECT_SOURCE_DIR}/share/jar/pop3.jar                             ${PROJECT_SOURCE_DIR}/share/jar/xmlgraphics-commons-1.3.1.jar
    ${PROJECT_SOURCE_DIR}/share/jar/iText-2.1.5.jar          ${PROJECT_SOURCE_DIR}/share/jar/jide-oss-2.9.7.jar       ${PROJECT_SOURCE_DIR}/share/jar/reflections-0.9.9-RC1-uberjar.jar    ${PROJECT_SOURCE_DIR}/share/jar/zip4j-1.3.1.jar
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/share/jar  ${PROJECT_BINARY_DIR}/runtime/jar
)
add_custom_command(
	OUTPUT 
	${PROJECT_BINARY_DIR}/runtime/BGEUnitTests.config  ${PROJECT_BINARY_DIR}/runtime/DebugPlots.config            ${PROJECT_BINARY_DIR}/runtime/LongVerificationScenarios.config  ${PROJECT_BINARY_DIR}/runtime/ValidationPatients.config  ${PROJECT_BINARY_DIR}/runtime/VerificationScenarios.config
	${PROJECT_BINARY_DIR}/runtime/CDMUnitTests.config  ${PROJECT_BINARY_DIR}/runtime/DebugRun.config              ${PROJECT_BINARY_DIR}/runtime/PlotRun.config                    ${PROJECT_BINARY_DIR}/runtime/ValidationSystems.config
	${PROJECT_BINARY_DIR}/runtime/Characterize.config  ${PROJECT_BINARY_DIR}/runtime/InitialPatientStates.config  ${PROJECT_BINARY_DIR}/runtime/ValidationDrugs.config            ${PROJECT_BINARY_DIR}/runtime/ValidationTables.config
	${PROJECT_BINARY_DIR}/runtime/ConfigEmailList.txt
	DEPENDS ${PROJECT_BINARY_DIR}/runtime 
    ${PROJECT_SOURCE_DIR}/runtime/BGEUnitTests.config  ${PROJECT_SOURCE_DIR}/runtime/DebugPlots.config            ${PROJECT_SOURCE_DIR}/runtime/LongVerificationScenarios.config  ${PROJECT_SOURCE_DIR}/runtime/ValidationPatients.config  ${PROJECT_SOURCE_DIR}/runtime/VerificationScenarios.config
	${PROJECT_SOURCE_DIR}/runtime/CDMUnitTests.config  ${PROJECT_SOURCE_DIR}/runtime/DebugRun.config              ${PROJECT_SOURCE_DIR}/runtime/PlotRun.config                    ${PROJECT_SOURCE_DIR}/runtime/ValidationSystems.config
	${PROJECT_SOURCE_DIR}/runtime/Characterize.config  ${PROJECT_SOURCE_DIR}/runtime/InitialPatientStates.config  ${PROJECT_SOURCE_DIR}/runtime/ValidationDrugs.config            ${PROJECT_SOURCE_DIR}/runtime/ValidationTables.config
	${PROJECT_SOURCE_DIR}/runtime/ConfigEmailList.txt
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/share/data ${PROJECT_BINARY_DIR}/runtime
)
add_custom_command(
	OUTPUT ${PROJECT_BINARY_DIR}/runtime/UCEDefs.conf
	DEPENDS ${PROJECT_BINARY_DIR}/runtime 
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/share/etc  ${PROJECT_BINARY_DIR}/runtime
)
add_custom_command(
	OUTPUT ${PROJECT_BINARY_DIR}/runtime/doc
	DEPENDS ${PROJECT_BINARY_DIR}/runtime 
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/share/doc  ${PROJECT_BINARY_DIR}/runtime/doc
)
add_custom_command(
	OUTPUT ${PROJECT_BINARY_DIR}/runtime/Scenarios
	DEPENDS ${PROJECT_BINARY_DIR}/runtime 
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/share/Scenarios ${PROJECT_BINARY_DIR}/runtime/Scenarios
)

add_custom_target( stage_runtime ALL
    DEPENDS 
	${PROJECT_BINARY_DIR}/runtime
	${PROJECT_BINARY_DIR}/runtime/Scenarios
	${PROJECT_BINARY_DIR}/runtime/xsd 
	${PROJECT_BINARY_DIR}/runtime/jar
	${PROJECT_BINARY_DIR}/runtime/doc
	${PROJECT_BINARY_DIR}/runtime/UCEDefs.conf
	${PROJECT_BINARY_DIR}/runtime/BGEUnitTests.config  ${PROJECT_BINARY_DIR}/runtime/DebugPlots.config            ${PROJECT_BINARY_DIR}/runtime/LongVerificationScenarios.config  ${PROJECT_BINARY_DIR}/runtime/ValidationPatients.config  ${PROJECT_BINARY_DIR}/runtime/VerificationScenarios.config
	${PROJECT_BINARY_DIR}/runtime/CDMUnitTests.config  ${PROJECT_BINARY_DIR}/runtime/DebugRun.config              ${PROJECT_BINARY_DIR}/runtime/PlotRun.config                    ${PROJECT_BINARY_DIR}/runtime/ValidationSystems.config
	${PROJECT_BINARY_DIR}/runtime/Characterize.config  ${PROJECT_BINARY_DIR}/runtime/InitialPatientStates.config  ${PROJECT_BINARY_DIR}/runtime/ValidationDrugs.config            ${PROJECT_BINARY_DIR}/runtime/ValidationTables.config
    COMMENT "Scanning for need to stage runtime dir"
)