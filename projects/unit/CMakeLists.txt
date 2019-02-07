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
   add_source_files(BIOGEARS_COMMON_UNITTEST_HEADERS "${PROJECT_SOURCE_DIR}/projects/biogears-common/unit/" "test_common*.h"  "common\\")
   add_source_files(BIOGEARS_COMMON_UNITTEST_SOURCES "${PROJECT_SOURCE_DIR}/projects/biogears-common/unit/" "test_common*.cpp"  "common\\")
  endif()
  if(UNITTEST_biogears-core)
   add_source_files(BIOGEARS_CORE_UNITTEST_HEADERS "${PROJECT_SOURCE_DIR}/projects/biogears/unit/" "test_core*.h"  "core\\")
   add_source_files(BIOGEARS_CORE_UNITTEST_SOURCES "${PROJECT_SOURCE_DIR}/projects/biogears/unit/" "test_core*.cpp"  "core\\")
  endif()
  if(UNITTEST_biogears-cmd)
   add_source_files(BIOGEARS_CMDLINE_UNITTEST_HEADERS "${PROJECT_SOURCE_DIR}/projects/cmd_bio/unit/" "test_cmdline*.h"  "cmdline\\" )
   add_source_files(BIOGEARS_CMDLINE_UNITTEST_SOURCES "${PROJECT_SOURCE_DIR}/projects/cmd_bio/unit/" "test_cmdline*.cpp"  "cmdline\\")
  endif()
  if(UNITTEST_biogears-circuit)
   add_source_files(BIOGEARS_CIRCUIT_UNITTEST_HEADERS "${PROJECT_SOURCE_DIR}/projects/biogears/unit/" "test_circuit*.h"  "circuit\\" )
   add_source_files(BIOGEARS_CIRCUIT_UNITTEST_SOURCES "${PROJECT_SOURCE_DIR}/projects/biogears/unit/" "test_circuit*.cpp"  "circuit\\")
  endif()  
  
list(APPEND UNITTEST_LIBRARIES GTest::GTest GTest::Main
  
  )

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
                        "GROUP"
                        "TESTS"
                        ${ARGN}
                        )

  if(UNITTEST_${_l_GROUP})
  	  foreach ( test_name IN LISTS _l_TESTS )
	  	option(UNITTEST_${_l_GROUP}_${test_name} "Enable test_core_angles" ON)
	  	mark_as_advanced(UNITTEST_${_l_GROUP}_${test_name})
	  	if (  NOT UNITTEST_${_l_GROUP}_${test_name})
		  add_compile_definitions( DISABLE_BIOGEARS_${test_name}_TEST )
	endif()
	  endforeach()
 endif()
  endfunction() 

  set(COMMON_TEST_LIST ${BIOGEARS_COMMON_UNITTEST_SOURCES})
  list(TRANSFORM COMMON_TEST_LIST REPLACE ".*\\/test_common_(.*).cpp" "\\1")
  setup_unittest( GROUP biogears-common 
                  TESTS 
                    ${COMMON_TEST_LIST})
  set(CORE_TEST_LIST ${BIOGEARS_CORE_UNITTEST_SOURCES})
  list(TRANSFORM CORE_TEST_LIST REPLACE ".*\\/test_core_(.*).cpp" "\\1")
  setup_unittest( GROUP biogears-core
                  TESTS 
                    ${CORE_TEST_LIST})
  set(CMDLINE_TEST_LIST ${BIOGEARS_CMDLINE_UNITTEST_SOURCES})
  list(TRANSFORM CMDLINE_TEST_LIST REPLACE ".*\\/test_cmdline_(.*).cpp" "\\1")
  setup_unittest( GROUP biogears-cmd 
                  TESTS 
                    ${CMDLINE_TEST_LIST})
 
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

  if(WIN32)
  target_compile_options(unittest
      PRIVATE "-D_CRT_SECURE_NO_WARNINGS"
  )
  endif()

  if (${PROJECT_NAME}_BUILD_STATIC)
    add_definitions("-D${PROJECT_NAME}_BUILT_AS_STATIC")
  endif()

  target_link_libraries(unittest
    ${UNITTEST_LIBRARIES}
    ${bio_LIBRARIES}
    libbiogears
  )
endif(${PROJECT_NAME}_BUILD_TEST)
