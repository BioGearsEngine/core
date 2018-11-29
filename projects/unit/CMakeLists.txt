CMAKE_MINIMUM_REQUIRED(VERSION 3.8.0) 
CMAKE_POLICY(VERSION 3.8.0) 

enable_language(CXX)

 option(ARA_${PROJECT_NAME}_BUILD_TEST        "Select to Build Tests." OFF)

if(ARA_${PROJECT_NAME}_BUILD_TEST)
  set(OUTPUT_PREFIX ${CMAKE_BINARY_DIR}/outputs)
  if(UNIX)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_PREFIX}/unit/lib)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_PREFIX}/unit/lib)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_PREFIX}/unit/bin)
  else()
      foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
          string(TOUPPER _${OUTPUTCONFIG} OUTPUTCONFIG_UPPER)
          set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY${OUTPUTCONFIG_UPPER} "${OUTPUT_PREFIX}/${OUTPUTCONFIG}/unit/lib")
          set(CMAKE_PDB_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} "${OUTPUT_PREFIX}/${OUTPUTCONFIG}/unit/lib")
          set(CMAKE_LIBRARY_OUTPUT_DIRECTORY${OUTPUTCONFIG_UPPER} "${OUTPUT_PREFIX}/${OUTPUTCONFIG}/unit/bin")
          set(CMAKE_RUNTIME_OUTPUT_DIRECTORY${OUTPUTCONFIG_UPPER} "${OUTPUT_PREFIX}/${OUTPUTCONFIG}/unit/bin")
      endforeach(OUTPUTCONFIG)
  endif()
  
  ###############################################################################
  # Unit Test
  # Step 1:     Assign master and sub_test variables to disable unit test
  #             I suggest this for projects with 100s of test. 
  #             TODO: Create a Macro to control this
  #                   import unit test from Project/cmake/unit.cmake
  ##################################################################V#############
  option(ARA_${PROJECT_NAME}_TEST_biogears "Test the biogears " ON)
  if(ARA_${PROJECT_NAME}_TEST_biogears)
    option(TEST_${PROJECT_NAME}_Function "Test the biogears function" ON)
  endif()
  ###############################################################################
  # Requirments
  ###############################################################################
  find_package(GTest)
  
  file(GLOB_RECURSE UNITTEST_HEADERS "src/*.h")
  file(GLOB_RECURSE UNITTEST_SOURCES "src/*.cpp")
  
  ##################################################################V#############
  # Step 2:     Assign source files based on control variables
  #             TODO: Create a Macro to control this
  #                   import unit test from Project/cmake/unit.cmake
  ##################################################################V#############
  if(ARA_${PROJECT_NAME}_TEST_biogears)
    file(GLOB_RECURSE UNITTEST_${PROJECT_NAME}_HEADERS "${bio_UNIT_DIR}/test_bio_*.h")
    file(GLOB_RECURSE UNITTEST_${PROJECT_NAME}_SOURCES "${bio_UNIT_DIR}/test_bio_*.cpp")
    list(APPEND UNIT_TEST_LIBRARIES )
  endif(ARA_${PROJECT_NAME}_TEST_biogears)
  
  
list(APPEND UNIT_TEST_LIBRARIES
  ${GTEST_BOTH_LIBRARIES}
  )

  add_executable(unittest
    ${UNITTEST_SOURCES}
    ${UNITTEST_HEADERS}
    ${UNITTEST_${PROJECT_NAME}_HEADERS}
    ${UNITTEST_${PROJECT_NAME}_SOURCES}
  )
 
  ##################################################################V#############
  # Step 3:     Define Disable Macros based on sub test variables
  #             TODO: Create a Macro to control this
  #                   import unit test from Project/cmake/unit.cmake
  ##################################################################V#############
  if(ARA_${PROJECT_NAME}_TEST_biogears)
    if(NOT TEST_${PROJECT_NAME}_Image)
      add_definitions(-DDISABLE_${PROJECT_NAME}_IMAGE_TEST)
    endif(NOT TEST_${PROJECT_NAME}_Image)
  endif(ARA_${PROJECT_NAME}_TEST_biogears)

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

  if (ARA_${PROJECT_NAME}_BUILD_STATIC)
    add_definitions("-D${PROJECT_NAME}_BUILT_AS_STATIC")
  endif()

  target_link_libraries(unittest
    ${UNIT_TEST_LIBRARIES}
    ${bio_LIBRARIES}
    libbiogears
  )
endif(ARA_${PROJECT_NAME}_BUILD_TEST)
