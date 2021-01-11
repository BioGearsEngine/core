CMAKE_MINIMUM_REQUIRED(VERSION 3.8.0) 
CMAKE_POLICY(VERSION 3.8.0) 

enable_language(CXX)
find_package(GTest)
option(${PROJECT_NAME}_BUILD_TEST        "Select to Build Tests." ${GTest_FOUND})

if(${PROJECT_NAME}_BUILD_TEST)
  
  ###############################################################################
  # Unit Test
  # Step 1:     Assign master and sub_test variables to disable unit test
  #             I suggest this for projects with 100s of test. 
  #             TODO: Create a Macro to control this
  #                   import unit test from Project/cmake/unit.cmake
  ##################################################################V#############
  option(UNITTEST_biogears-core "Enable libBiogears UnitTest " ON)
  option(UNITTEST_biogears-common "Enable Biogears-Common UnitTest " ON)
  option(UNITTEST_biogears-cmd "Enable Biogears command line UnitTest " ON)
  option(UNITTEST_biogears-circuit "Enable Biogears circuit UnitTest " ON)


  ###############################################################################
  # Requirments
  ###############################################################################
  find_package(GTest REQUIRED)
  
  file(GLOB_RECURSE UNITTEST_HEADERS "src/*.h")
  file(GLOB_RECURSE UNITTEST_SOURCES "src/*.cpp")
  
  ##################################################################V#############
  # Step 2:     Assign source files based on control variables
  #             TODO: Create a Macro to control this
  #                   import unit test from Project/cmake/unit.cmake
  ##################################################################V#############
  if(UNITTEST_biogears-common)
   add_source_files(BIOGEARS_COMMON_UNITTEST_HEADERS LOCATION "${PROJECT_SOURCE_DIR}/projects/biogears-common/unit/" REGEX "test_common*.h" SOURCE_GROUP "common\\")
   add_source_files(BIOGEARS_COMMON_UNITTEST_SOURCES LOCATION "${PROJECT_SOURCE_DIR}/projects/biogears-common/unit/" REGEX "test_common*.cpp" SOURCE_GROUP "common\\")
  endif()
  if(UNITTEST_biogears-core)
   add_source_files(BIOGEARS_CORE_UNITTEST_HEADERS LOCATION "${PROJECT_SOURCE_DIR}/projects/biogears/unit/" REGEX "test_core*.h" SOURCE_GROUP "core\\")
   add_source_files(BIOGEARS_CORE_UNITTEST_SOURCES LOCATION "${PROJECT_SOURCE_DIR}/projects/biogears/unit/" REGEX "test_core*.cpp" SOURCE_GROUP "core\\")
  endif()
  if(UNITTEST_biogears-cmd)
   add_source_files(BIOGEARS_CMDLINE_UNITTEST_HEADERS LOCATION "${PROJECT_SOURCE_DIR}/projects/cli/unit/" REGEX "test_cli*.h" SOURCE_GROUP "cmdline\\" )
   add_source_files(BIOGEARS_CMDLINE_UNITTEST_SOURCES LOCATION "${PROJECT_SOURCE_DIR}/projects/cli/unit/" REGEX "test_cli*.cpp" SOURCE_GROUP "cmdline\\")
  endif()
  if(UNITTEST_biogears-circuit)
   add_source_files(BIOGEARS_CIRCUIT_UNITTEST_HEADERS LOCATION "${PROJECT_SOURCE_DIR}/projects/biogears/unit/" REGEX "test_circuit*.h" SOURCE_GROUP "circuit\\" )
   add_source_files(BIOGEARS_CIRCUIT_UNITTEST_SOURCES LOCATION "${PROJECT_SOURCE_DIR}/projects/biogears/unit/" REGEX "test_circuit*.cpp" SOURCE_GROUP "circuit\\")
  endif()  
  

  add_executable(unittest
    ${UNITTEST_SOURCES}
    ${UNITTEST_HEADERS}
    ${BIOGEARS_CORE_UNITTEST_HEADERS}
    ${BIOGEARS_CORE_UNITTEST_SOURCES}
    ${BIOGEARS_COMMON_UNITTEST_HEADERS}
    ${BIOGEARS_COMMON_UNITTEST_SOURCES}
    ${BIOGEARS_CMDLINE_UNITTEST_HEADERS}
    ${BIOGEARS_CMDLINE_UNITTEST_SOURCES}
    ${BIOGEARS_CIRCUIT_UNITTEST_HEADERS}
    ${BIOGEARS_CIRCUIT_UNITTEST_SOURCES}
  )
 
  ##################################################################V#############
  # Step 3:     Define Disable Macros based on sub test variables
  #             TODO: Create a Macro to control this
  #                   import unit test from Project/cmake/unit.cmake
  ##################################################################V#############
  function( setup_unittest )
    cmake_parse_arguments( "_l" 
                          ""
                          "GROUP;PREFIX"
                          "TESTS"
                          ${ARGN}
                          )
    if(UNITTEST_${_l_GROUP})
    	  foreach ( filepath IN LISTS _l_TESTS )
          string(REGEX REPLACE ".*\\/test_${_l_PREFIX}_(.*).cpp" "\\1" test_name ${filepath})
	    	  option(UNITTEST_${_l_GROUP}_${test_name} "Enable ${_l_GROUP}_${test_name}" ON)
	    	  mark_as_advanced(UNITTEST_${_l_GROUP}_${test_name})
	    	  if (  NOT UNITTEST_${_l_GROUP}_${test_name})
            set_property(   SOURCE ${filepath}
                            PROPERTY
                            COMPILE_DEFINITIONS DISABLE_BIOGEARS_${test_name}_TEST )
	        endif()
	      endforeach()
    endif()
  endfunction() 

  set(COMMON_TEST_LIST ${BIOGEARS_COMMON_UNITTEST_SOURCES})
  setup_unittest( GROUP biogears-common PREFIX common
                  TESTS 
                    ${COMMON_TEST_LIST})
  set(CORE_TEST_LIST ${BIOGEARS_CORE_UNITTEST_SOURCES})
  setup_unittest( GROUP biogears-core PREFIX core
                  TESTS 
                    ${CORE_TEST_LIST})
  set(CMDLINE_TEST_LIST ${BIOGEARS_CMDLINE_UNITTEST_SOURCES})
  setup_unittest( GROUP biogears-cmd PREFIX cmd
                  TESTS 
                    ${CMDLINE_TEST_LIST})
  set(CIRCUIT_TEST_LIST ${BIOGEARS_CIRCUIT_UNITTEST_SOURCES})
  setup_unittest( GROUP biogears-circuit PREFIX circuit
                  TESTS 
                    ${CIRCUIT_TEST_LIST})
 
  set_target_properties(unittest PROPERTIES
                        DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX}
                        CXX_STANDARD 14
                        FOLDER Other
                        PROJECT_LABEL "Unit Test"
                        VS_DEBUGGER_WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/runtime
                      )

  target_include_directories(unittest
      PRIVATE
      ${CMAKE_CURRENT_SOURCE_DIR}/src
      ${GTEST_INCLUDE_DIR}
      ${bio_INCLUDE_DIR}
  )
  if(UNITTEST_biogears-core)
    target_include_directories(unittest
      PRIVATE
      ${CMAKE_CURRENT_SOURCE_DIR}/../biogears/libBiogears/src
  )
  endif()

     target_compile_definitions(unittest PRIVATE ${${PREFIX}_CPPFLAGS} PRIVATE $<$<PLATFORM_ID:Windows>:"_CRT_SECURE_NO_WARNINGS"> )
     target_compile_options(unittest PRIVATE $<$<PLATFORM_ID:Windows>:/bigobj>    PRIVATE $<$<PLATFORM_ID:Windows>:/MP${${ROOT_PROJECT_NAME}_BUILD_PARALLEL_LEVEL}>)

  target_link_libraries(unittest
    GTest::GTest 
    GTest::Main
    Biogears::libbiogears
  )
endif(${PROJECT_NAME}_BUILD_TEST)
