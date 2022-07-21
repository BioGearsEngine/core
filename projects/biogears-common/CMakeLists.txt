###############################################################################
# Policy adjustments
###############################################################################

###############################################################################
# Options
###############################################################################

 option(Biogears_COMMON_BUILD_STATIC       "If enabled Biogears biogears_common will be built as a static library"  ON)
 
###############################################################################
# Base Variables
###############################################################################

set(Common_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include" PARENT_SCOPE)
set(Common_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include" )
set(Common_PRIVATE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src" PARENT_SCOPE)
set(Common_PRIVATE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src" )
set(Common_GENERATED_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}" PARENT_SCOPE)
set(Common_GENERATED_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}" )
set(Common_UNIT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/unit" PARENT_SCOPE)

###############################################################################
# Requirments
###############################################################################
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
find_package(Threads REQUIRED)
###############################################################################
#Code Generation
###############################################################################

###############################################################################
#Sorce and Header Defines
###############################################################################
message(STATUS "Configuring biogears_common")

#Scenario Driver

add_source_files(PUBLIC_HDRS LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/include REGEX "*.h" SOURCE_GROUP "Headers\\Public\\")

add_source_files(SRCS LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/src REGEX "*.cpp" SOURCE_GROUP "Sources\\")
add_source_files(SRCS LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/src REGEX "*.c" SOURCE_GROUP "Sources\\")

set(Common_HEADERS ${HDRS} ${PUBLIC_HDRS} ${GEN_HDRS})
set(Common_SOURCES ${SRCS} ${GEN_SRCS})
###############################################################################
#Define Logic
###############################################################################
if(WIN32)
  list(APPEND Common_CPPFLAGS_EXPORT )
  list(APPEND Common_CPPFLAGS "-D_SCL_SECURE_NO_WARNINGS" "-D_CRT_SECURE_NO_WARNINGS"  $ENV{PARALLEL_COMPILE} )
elseif(CMAKE_COMPILER_IS_GNUCXX)
list(APPEND Common_CPPFLAGS_EXPORT )
  list(APPEND Common_CPPFLAGS  ${CodeSynthesis_CPPFLAGS})
  list(APPEND Common_LDFLAGS "-Wl,--no-as-needed" )
endif()


if(Biogears_COMMON_BUILD_STATIC)
  add_library(libbiogears_common STATIC  ${Common_SOURCES} ${Common_HEADERS})
else()
  add_library(libbiogears_common SHARED  ${Common_SOURCES} ${Common_HEADERS})
endif()

add_library(Biogears::libbiogears_common ALIAS libbiogears_common)
set_target_properties(libbiogears_common
  PROPERTIES
  DEFINE_SYMBOL biogears_common_EXPORTS
  FOLDER "Libraries"
  COMPILE_PDB_NAME "biogears_common"
  OUTPUT_NAME "biogears_common"
  PROJECT_LABEL "libbiogears_common"
  DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}"
  CXX_STANDARD 14
  POSITION_INDEPENDENT_CODE TRUE
  VS_DEBUGGER_WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/runtime
  )
target_compile_definitions(libbiogears_common PUBLIC  ${Common_INTERFACE_CPPFLAGS} PRIVATE ${Common_CPPFLAGS} )
if (Biogears_COMMON_BUILD_STATIC)
    target_compile_definitions(libbiogears_common PUBLIC  "BIOGEARS_COMMON_BUILD_STATIC")
    set(LIB_TYPE STATIC)
endif()

target_compile_options(libbiogears_common PRIVATE $<$<PLATFORM_ID:Windows>:/bigobj>    PRIVATE $<$<PLATFORM_ID:Windows>:/MP${Biogears_BUILD_PARALLEL_LEVEL}>)
target_include_directories(libbiogears_common
  PRIVATE  ${Common_INCLUDE_DIR} ${Common_PRIVATE_INCLUDE_DIR} ${Common_GENERATED_INCLUDE_DIR}
  INTERFACE $<BUILD_INTERFACE:${Common_GENERATED_INCLUDE_DIR}> 
      $<BUILD_INTERFACE:${Common_INCLUDE_DIR}>
      $<INSTALL_INTERFACE:include>
)
###############################################################################
# COMPILATION & LINKAGE MODIFICATIONS
###############################################################################

list(APPEND Common_INCLUDES
      Common_INCLUDE_DIR
      Common_PRIVATE_INCLUDE_DIR
      Common_GENERATED_INCLUDE_DIR
      bio_INCLUDE_DIR
)
list(REMOVE_DUPLICATES Common_INCLUDES)

set(Common_LIBS
      ${CMAKE_THREAD_LIBS_INIT}
      ${CMAKE_DL_LIBS}
)

set(Common_LIBS ${Common_LIBS} PARENT_SCOPE)
target_link_libraries(libbiogears_common PUBLIC ${Common_LIBS} )
if(ANDROID)
  target_link_libraries(libbiogears_common PUBLIC android log)
endif() 
if(TARGET prepare_runtime_dir AND Biogears_PREPARE_RUNTIME_DIR) 
	add_dependencies(libbiogears_common prepare_runtime_dir)
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
    set_target_properties(libbiogears_common PROPERTIES LINK_FLAGS ${Common_LDFLAGS})
endif()

install(TARGETS libbiogears_common EXPORT libBiogears
    RUNTIME DESTINATION bin/${Biogears_PLATFORM_PREFIX}$<$<BOOL:${Biogears_BREAKOUT_INSTALL}>:/$<CONFIG>>
    LIBRARY DESTINATION ${LIBRARY_INSTALL_DIR}/${Biogears_PLATFORM_PREFIX}$<$<BOOL:${Biogears_BREAKOUT_INSTALL}>:/$<CONFIG>>
    ARCHIVE DESTINATION lib/${Biogears_PLATFORM_PREFIX}$<$<BOOL:${Biogears_BREAKOUT_INSTALL}>:/$<CONFIG>>
)
 
install(DIRECTORY 
   include/biogears
   DESTINATION include/
   FILE_PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
)

install(FILES 
   "include/units.h"
   DESTINATION include/
)
