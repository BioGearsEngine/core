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
  option(UNIT_TEST_biogears "Enable libBiogears UnitTest " ON)
  option(UNIT_TEST_biogears-common "Enable Biogears-Common UnitTest " ON)

  ###############################################################################
  # Requirments
  ###############################################################################
  find_package(GTest REQUIRED)
  
  file(GLOB_RECURSE UNITTEST_HEADERS "src/*.h")
  file(GLOB_RECURSE UNITTEST_SOURCES "src/*.cpp")
  
  if(UNIT_TEST_biogears-common)
    file(GLOB_RECURSE BIOGEARS_COMMON_UNITTEST_HEADERS "${PROJECT_SOURCE_DIR}/projects/biogears-common/unit/*.h")
    file(GLOB_RECURSE BIOGEARS_COMMON_UNITTEST_SOURCES "${PROJECT_SOURCE_DIR}/projects/biogears-common/unit/*.cpp")
  endif()
  ##################################################################V#############
  # Step 2:     Assign source files based on control variables
  #             TODO: Create a Macro to control this
  #                   import unit test from Project/cmake/unit.cmake
  ##################################################################V#############
  if(${PROJECT_NAME}_TEST_biogears)
    file(GLOB_RECURSE UNITTEST_${PROJECT_NAME}_HEADERS "${bio_UNIT_DIR}/test_bio_*.h")
    file(GLOB_RECURSE UNITTEST_${PROJECT_NAME}_SOURCES "${bio_UNIT_DIR}/test_bio_*.cpp")
    list(APPEND UNIT_TEST_LIBRARIES )
  endif(${PROJECT_NAME}_TEST_biogears)
  
  
list(APPEND UNIT_TEST_LIBRARIES GTest::GTest GTest::Main
  
  )

  add_executable(unittest
    ${UNITTEST_SOURCES}
    ${UNITTEST_HEADERS}
    ${UNITTEST_${PROJECT_NAME}_HEADERS}
    ${UNITTEST_${PROJECT_NAME}_SOURCES}
	${BIOGEARS_COMMON_UNITTEST_HEADERS}
	${BIOGEARS_COMMON_UNITTEST_SOURCES}
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

  if(UNIT_TEST_${_l_GROUP})
  	  foreach ( test_name IN LISTS _l_TESTS )
		option(UNIT_TEST_${_l_GROUP}_${test_name} "Enable test_core_angles" ON)
		if (  NOT UNIT_TEST_${_l_GROUP}_${test_name})
		  add_compile_definitions( DISABLE_BIOGEARS_${test_name}_TEST )
	endif()
	  endforeach()
 endif()
  endfunction() 

  setup_unittest( GROUP biogears-common 
                  TESTS 
                     "Angles"
                     "Billboard"
                     "ConcurrentBoundedQueue"
                     "ConcurrentQueue"
                     "ConcurrentRingbuffer"
                     "Option"
                     "OwningMutex"
                     "Result"
                     "scmp_channel"
                     "scsp_channel"
                     "StopWatch"
                     "string_manipulation"
                     "ThreadPoll"
                     "WorkChannel")


  set_target_properties(unittest PROPERTIES
                        DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX}
                        CXX_STANDARD 14
                        FOLDER Other
                        PROJECT_LABEL "Unit Test")

  target_include_directories(unittest
      PRIVATE
      ${CMAKE_CURRENT_SOURCE_DIR}/src
<<<<<<< origin/master
      #${GTEST_INCLUDE_DIR}
=======
>>>>>>> HEAD~32
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
    ${UNIT_TEST_LIBRARIES}
    ${bio_LIBRARIES}
    libbiogears
  )
endif(${PROJECT_NAME}_BUILD_TEST)
