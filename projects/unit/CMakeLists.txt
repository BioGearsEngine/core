CMAKE_MINIMUM_REQUIRED(VERSION 3.8.0) 
CMAKE_POLICY(VERSION 3.8.0) 

enable_language(CXX)

 option(${PROJECT_NAME}_BUILD_TEST        "Select to Build Tests." OFF)

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
  find_package(GTest)
  
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
  
  
list(APPEND UNIT_TEST_LIBRARIES
  ${GTEST_BOTH_LIBRARIES}
  
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
  if(UNIT_TEST_biogears-common)
	option(UNIT_TEST_${PROJECT_NAME}_biogears-common_Angles "Enable test_core_angles" ON)
	option(UNIT_TEST_${PROJECT_NAME}_biogears-common_Billboard "Enable test_core_Billboard" ON)
	option(UNIT_TEST_${PROJECT_NAME}_biogears-common_ConcurrentBoundedQueue "Enable test_core_ConcurrentBoundedQueue" ON)
	option(UNIT_TEST_${PROJECT_NAME}_biogears-common_ConcurrentQueue "Enable test_core_ConcurrentQueue" ON)
	option(UNIT_TEST_${PROJECT_NAME}_biogears-common_ConcurrentRingbuffer "Enable test_core_ConcurrentRingbuffer" ON)
	option(UNIT_TEST_${PROJECT_NAME}_biogears-common_Option "Enable test_core_Option" ON)
	option(UNIT_TEST_${PROJECT_NAME}_biogears-common_OwningMutex "Enable test_core_OwningMutex" ON)
	option(UNIT_TEST_${PROJECT_NAME}_biogears-common_Result "Enable test_core_Result"  ON)
	option(UNIT_TEST_${PROJECT_NAME}_biogears-common_scmp_channel "Enable test_core_scmp_channel"  ON)
	option(UNIT_TEST_${PROJECT_NAME}_biogears-common_scsp_channel "Enable test_core_scsp_channel"  ON)
	option(UNIT_TEST_${PROJECT_NAME}_biogears-common_StopWatch "Enable test_core_StopWatch"  ON)
	option(UNIT_TEST_${PROJECT_NAME}_biogears-common_StopWatchf "Enable test_core_StopWatchf"  ON)
	option(UNIT_TEST_${PROJECT_NAME}_biogears-common_string_manipulation "Enable test_core_string_manipulation"  ON)
	option(UNIT_TEST_${PROJECT_NAME}_biogears-common_ThreadPoll "Enable test_core_ThreadPoll"  ON)
	option(UNIT_TEST_${PROJECT_NAME}_biogears-common_WorkChannel "Enable  test_core_WorkChannel" ON )
    
	if ( NOT UNIT_TEST_${PROJECT_NAME}_biogears-common_Angles)
		add_compile_definitions( DISABLE_BIOGEARS_Angles_TEST )
	endif()
	if ( NOT UNIT_TEST_${PROJECT_NAME}_biogears-common_Billboard)
		add_compile_definitions( DISABLE_BIOGEARS_Billboard_TEST )
	endif()
	if ( NOT UNIT_TEST_${PROJECT_NAME}_biogears-common_ConcurrentBoundedQueu)
		add_compile_definitions( DISABLE_BIOGEARS_ConcurrentBoundedQueue_TEST )
	endif()
	if ( NOT UNIT_TEST_${PROJECT_NAME}_biogears-common_ConcurrentQueue)
		add_compile_definitions( DISABLE_BIOGEARS_ConcurrentQueue_TEST )
	endif()
	if ( NOT UNIT_TEST_${PROJECT_NAME}_biogears-common_ConcurrentRingbuffer)
		add_compile_definitions( DISABLE_BIOGEARS_Option_TEST )
	endif()
	if ( NOT UNIT_TEST_${PROJECT_NAME}_biogears-common_Option)
		add_compile_definitions( DISABLE_BIOGEARS_Option_TEST )
	endif()
		if ( NOT UNIT_TEST_${PROJECT_NAME}_biogears-common_OwningMutex)
		add_compile_definitions( DISABLE_BIOGEARS_OwningMutex_TEST )
	endif()
	if ( NOT UNIT_TEST_${PROJECT_NAME}_biogears-common_Result)
		add_compile_definitions( DISABLE_BIOGEARS_Result_TEST )
	endif()
 endif()




  if(${PROJECT_NAME}_TEST_biogears)
    if(NOT TEST_${PROJECT_NAME}_Image)
      add_definitions(-DDISABLE_${PROJECT_NAME}_IMAGE_TEST)
    endif(NOT TEST_${PROJECT_NAME}_Image)
  endif(${PROJECT_NAME}_TEST_biogears)

  set_target_properties(unittest PROPERTIES
                        DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX}
                        CXX_STANDARD 14
                        FOLDER Other
                        PROJECT_LABEL "Unit Test")

  target_include_directories(unittest
      PRIVATE
      ${CMAKE_CURRENT_SOURCE_DIR}/src
      #${GTEST_INCLUDE_DIR}
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
