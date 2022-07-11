CMAKE_MINIMUM_REQUIRED(VERSION 3.8.0) 
CMAKE_POLICY(VERSION 3.8.0) 

enable_language(CXX)
find_package(GTest)
option(Biogears_BUILD_TEST        "Select to Build Tests." ${GTest_FOUND})

if(Biogears_BUILD_TEST)
  
  ###############################################################################
  # Unit Test
  # Step 1:     Assign master and sub_test variables to disable unit test
  #             I suggest this for projects with 100s of test. 
  #             TODO: Create a Macro to control this
  #                   import unit test from Project/cmake/unit.cmake
  ##################################################################V#############
  option(UNITTEST_biogears-common "Enable Biogears-Common UnitTest " ON)
  option(UNITTEST_biogears-core "Enable libBiogears UnitTest " ON)
  option(UNITTEST_biogears-cmd "Enable Biogears command line UnitTest " ON)


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
   LIST(APPEND UNITTEST_HEADERS ${BIOGEARS_COMMON_UNITTEST_HEADERS})
   LIST(APPEND UNITTEST_SOURCES ${BIOGEARS_COMMON_UNITTEST_SOURCES})
  endif()
  if(UNITTEST_biogears-core)
   add_source_files(BIOGEARS_CORE_UNITTEST_HEADERS LOCATION "${PROJECT_SOURCE_DIR}/projects/biogears/libBiogears/unit/" REGEX "test_core*.h" SOURCE_GROUP "core\\")
   add_source_files(BIOGEARS_CORE_UNITTEST_SOURCES LOCATION "${PROJECT_SOURCE_DIR}/projects/biogears/libBiogears/unit/" REGEX "test_core*.cpp" SOURCE_GROUP "core\\")
   LIST(APPEND UNITTEST_HEADERS ${BIOGEARS_CORE_UNITTEST_HEADERS})
   LIST(APPEND UNITTEST_SOURCES ${BIOGEARS_CORE_UNITTEST_SOURCES})
  endif()
  if(UNITTEST_biogears-core_cdm)
   add_source_files(BIOGEARS_CORE_CDM_UNITTEST_HEADERS LOCATION "${PROJECT_SOURCE_DIR}/projects/biogears/libCDM/unit/" REGEX "test_core_cdm*.h" SOURCE_GROUP "core_cdm\\")
   add_source_files(BIOGEARS_CORE_CDM_UNITTEST_SOURCES LOCATION "${PROJECT_SOURCE_DIR}/projects/biogears/libCDM/unit/" REGEX "test_core_cdm*.cpp" SOURCE_GROUP "core_cdm\\")
   LIST(APPEND UNITTEST_HEADERS ${BIOGEARS_CORE_CDM_UNITTEST_HEADERS})
   LIST(APPEND UNITTEST_SOURCES ${BIOGEARS_CORE_CDM_UNITTEST_SOURCES})
  endif()
  if(UNITTEST_biogears-core_io)
   add_source_files(BIOGEARS_CORE_IO_UNITTEST_HEADERS LOCATION "${PROJECT_SOURCE_DIR}/projects/biogears/libIO/unit/" REGEX "test_core_io*.h" SOURCE_GROUP "core_io\\")
   add_source_files(BIOGEARS_CORE_IO_UNITTEST_SOURCES LOCATION "${PROJECT_SOURCE_DIR}/projects/biogears/libIO/unit/" REGEX "test_core_io*.cpp" SOURCE_GROUP "core_io\\")
   LIST(APPEND UNITTEST_HEADERS ${BIOGEARS_CORE_IO_UNITTEST_HEADERS})
   LIST(APPEND UNITTEST_SOURCES ${BIOGEARS_CORE_IO_UNITTEST_SOURCES})
  endif()
  if(UNITTEST_biogears-cmd)
   add_source_files(BIOGEARS_CMDLINE_UNITTEST_HEADERS LOCATION "${PROJECT_SOURCE_DIR}/projects/cli/unit/" REGEX "test_cli*.h" SOURCE_GROUP "cmdline\\" )
   add_source_files(BIOGEARS_CMDLINE_UNITTEST_SOURCES LOCATION "${PROJECT_SOURCE_DIR}/projects/cli/unit/" REGEX "test_cli*.cpp" SOURCE_GROUP "cmdline\\")
   LIST(APPEND UNITTEST_HEADERS ${BIOGEARS_CMDLINE_UNITTEST_HEADERS})
   LIST(APPEND UNITTEST_SOURCES ${BIOGEARS_CMDLINE_UNITTEST_SOURCES})
  endif()
  if(UNITTEST_biogears-circuit)
   add_source_files(BIOGEARS_CIRCUIT_UNITTEST_HEADERS LOCATION "${PROJECT_SOURCE_DIR}/projects/biogears/unit/" REGEX "test_circuit*.h" SOURCE_GROUP "circuit\\" )
   add_source_files(BIOGEARS_CIRCUIT_UNITTEST_SOURCES LOCATION "${PROJECT_SOURCE_DIR}/projects/biogears/unit/" REGEX "test_circuit*.cpp" SOURCE_GROUP "circuit\\")
   LIST(APPEND UNITTEST_HEADERS ${BIOGEARS_CIRCUIT_UNITTEST_HEADERS})
   LIST(APPEND UNITTEST_SOURCES ${BIOGEARS_CIRCUIT_UNITTEST_SOURCES})
  endif()  
  if(UNITTEST_biogears-swig_py)

  endif()
  if(UNITTEST_biogears-swig_csharp)

  endif()

  add_executable(unittest
    ${UNITTEST_SOURCES}
    ${UNITTEST_HEADERS}
  )
 


  ###############################################################################
  # Step 3:     Define Disable Macros based on sub test variables
  #             TO DO: Create a Macro to control this
  #                   import unit test from Project/cmake/unit.cmake
  ###############################################################################
  #
  #  function( setup_unittest )
  #  GROUP  : This Prefix of the option name used to control the Prerpcessor definition for enabling disabling test
  #           I suggest a global option be added to cashe which wraps the call to this function to allow granular control of
  #           groups and individual  fules of unittest. 
  #
  #  PREFIX : A consant string that appears in each file name after test_ and before the test name.
  #           The PREFIX is teh frist glob group of the regex s/test_(.*)_(.*).cpp/ The name of the preprocessor
  #           deffinition for disabiling the unit test will lacks the prefix argument from the filename 
  #
  #  TESTS  : List of files which will be added to as source files. Foreach file we will 
  #           Setup a Cache Option to allow disabling of the unit test
  #           All test files are assumed ot be of the form test_${_l_PREFIX}_(.*).cpp
  #
  #  This function assumes all unittest will start with the following code
  #   
  #  #ifdef DISABLE_BIOGEARS_${test_name}_TEST
  #  #define TEST_FIXTURE_NAME DISABLED_<SOME_NAME>Fixture
  #  #else
  #  #define TEST_FIXTURE_NAME <SOME_NAME>Fixture
  #  #endif
  #  
  #  // The fixture for testing class Foo.
  #  class TEST_FIXTURE_NAME : public ::testing::Test {
  #  protected:
  #    // You can do set-up work for each test here.
  #    TEST_FIXTURE_NAME() = default;
  #  
  #    // You can do clean-up work that doesn't throw exceptions here.
  #    virtual ~TEST_FIXTURE_NAME() = default;
  #  
  #    // If the constructor and destructor are not enough for setting up
  #    // and cleaning up each test, you can define the following methods:
  #  
  #    // Code here will be called immediately after the constructor (right
  #    // before each test).
  #    virtual void SetUp();
  #  
  #    // Code here will be called immediately after each test (right
  #    // before the destructor).
  #    virtual void TearDown();
  #  };
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
                 TESTS ${COMMON_TEST_LIST})
 set(CORE_TEST_LIST ${BIOGEARS_CORE_UNITTEST_SOURCES})
 setup_unittest( GROUP biogears-core PREFIX core
                 TESTS ${CORE_TEST_LIST})
 set(CORE_TEST_LIST ${BIOGEARS_CORE_IO_UNITTEST_SOURCES})
 setup_unittest( GROUP biogears-core_io PREFIX core_io
                  TESTS  ${CORE_TEST_LIST})
 set(CORE_TEST_LIST ${BIOGEARS_CORE_CDM_UNITTEST_SOURCES})
 setup_unittest( GROUP biogears-core_cdm PREFIX core_cdm
                 TESTS   ${CORE_TEST_LIST})
 set(CMDLINE_TEST_LIST ${BIOGEARS_CMDLINE_UNITTEST_SOURCES})
 setup_unittest( GROUP biogears-cmd PREFIX cmd
                 TESTS ${CMDLINE_TEST_LIST})
 set(CIRCUIT_TEST_LIST ${BIOGEARS_CIRCUIT_UNITTEST_SOURCES})
 setup_unittest( GROUP biogears-circuit PREFIX circuit
                 TESTS  ${CIRCUIT_TEST_LIST})
 
  set_target_properties(unittest PROPERTIES
                        DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}"
                        CXX_STANDARD 14
                        FOLDER UnitTest
                        VS_DEBUGGER_WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/runtime
                        VS_DEBUGGER_ENVIRONMENT "${VS_DEBUGGER_ENVIRONMENT_LIST}"
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

  target_compile_definitions(unittest PRIVATE ${UNIT_CPPFLAGS} PRIVATE $<$<PLATFORM_ID:Windows>:"_CRT_SECURE_NO_WARNINGS"> )
     target_compile_options(unittest PRIVATE $<$<PLATFORM_ID:Windows>:/bigobj>    PRIVATE $<$<PLATFORM_ID:Windows>:/MP${Biogears_BUILD_PARALLEL_LEVEL}>)

  target_link_libraries(unittest
    GTest::GTest 
    GTest::Main
    Biogears::libbiogears
  )
endif(Biogears_BUILD_TEST)
