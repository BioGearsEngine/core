cmake_policy(SET CMP0087 NEW)
include(InstallRequiredSystemLibraries)

option (Biogears_BUILD_INSTALLER "Turns on CPack platform specific building logic" OFF)
if(Biogears_BUILD_INSTALLER)

  set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "BioGears")
  set(CPACK_PACKAGE_VENDOR "Applied Resaerch Associates")
  set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.txt")
  set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.txt")
  set(CPACK_PACKAGE_HOMEPAGE_URL "http://biogearsengine.com")
  set(CPACK_PACKAGE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/biogearslogo_XoQ_icon.ico")
  
  set(CPACK_PACKAGE_INSTALL_DIRECTORY "BioGears/${CMAKE_VERSION_MAJOR}.${CMAKE_VERSION_MINOR}")
  set(CPACK_PACKAGE_VERSION_MAJOR ${Biogears_VERSION_MAJOR} )      
  set(CPACK_PACKAGE_VERSION_MINOR ${Biogears_VERSION_MINOR} )
  set(CPACK_PACKAGE_VERSION_PATCH ${Biogears_VERSION_PATCH} )
  set(CPACK_PACKAGE_VERSION_TWEAK ${Biogears_VERSION_TWEAK} )
  
  
  if(WIN32 AND NOT UNIX)
    # There is a bug in NSI that does not handle full UNIX paths properly.
    # Make sure there is at least one set of four backlashes.
    set(CPACK_PACKAGE_ICON "${CMAKE_CURRENT_SOURCE_DIR}\\\\BioGearsLogo.bmp")
    set(CPACK_NSIS_INSTALLED_ICON_NAME "${CMAKE_CURRENT_SOURCE_DIR}/biogearslogo_XoQ_icon.ico")
    set(CPACK_NSIS_DISPLAY_NAME "BioGears")
    set(CPACK_NSIS_HELP_LINK "http:\\\\\\\\www.biogearsengine.com")
    set(CPACK_NSIS_URL_INFO_ABOUT "http:\\\\\\\\www.biogearsengine.com")
    set(CPACK_NSIS_CONTACT "admin@biogearsengine.com")
    set(CPACK_NSIS_MODIFY_PATH ON)
    set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
  
    set(CPACK_WIX_UPGRADE_GUID "54B0EF16-257C-4325-A64B-630FFB302F16")
    set(CPACK_WIX_PRODUCT_ICON "${CMAKE_CURRENT_SOURCE_DIR}/biogearslogo_XoQ_icon.ico")
    set(CPACK_WIX_PROGRAM_MENU_FOLDER "BioGears")
    set(CPACK_WIX_UI_BANNER "${CMAKE_CURRENT_SOURCE_DIR}/WIX_UI_BANNER.png")
    set(CPACK_WIX_UI_DIALOG "${CMAKE_CURRENT_SOURCE_DIR}/WIX_UI_DIALOG.png")
    set(CPACK_WIX_PROPERTY_ARPCOMMENTS "Command line utilities for BioGears ${Biogears_VERSION_TAG}")
    set(CPACK_WIX_PROPERTY_ARPHELPLINK  "https://biogearsengine.com")
    set(ARPURLINFOABOUT  "https://biogearsengine.com")
    if (TARGET bg-cli) 
      install(CODE "
        include(BundleUtilities)
        string(TOUPPER \${CMAKE_INSTALL_CONFIG_NAME} CONFIG)
        if( NOT UNIX )
          set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY \${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_\${CONFIG}} )
          set( CMAKE_LIBRARY_OUTPUT_DIRECTORY \${CMAKE_LIBRARY_OUTPUT_DIRECTORY_\${CONFIG}} )
          set( CMAKE_RUNTIME_OUTPUT_DIRECTORY \${CMAKE_RUNTIME_OUTPUT_DIRECTORY_\${CONFIG}} )
        endif()
        
  
        set(CMAKE_PREFIX_PATH \"${CMAKE_PREFIX_PATH}\")
  
        foreach(_dir IN LISTS CMAKE_PREFIX_PATH)
          list(APPEND THIRD_PARTY  \"\${_dir}\")
          list(APPEND THIRD_PARTY_LIB \"\${_dir}/lib\")
          list(APPEND THIRD_PARTY_BIN \"\${_dir}/bin\")
        endforeach()
        
  
       fixup_bundle(\"\${CMAKE_INSTALL_PREFIX}/bin/$<TARGET_FILE_NAME:bg-cli>\"
                          \"\"
                        \"\${THIRD_PARTY_LIB};\${THIRD_PARTY_BIN}\"
                   )
      "         
      )
    endif() 

    install(DIRECTORY 
        ${PROJECT_SOURCE_DIR}/share/xsd 
        DESTINATION bin/
        FILE_PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
        )
    
    install(DIRECTORY 
        ${PROJECT_SOURCE_DIR}/share/data/
        DESTINATION bin/
        FILE_PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
        FILES_MATCHING 
        PATTERN "*.xml"
        PATTERN "*.xlsx"
        )
    install(DIRECTORY 
        ${PROJECT_SOURCE_DIR}/share/Scenarios
        DESTINATION bin/
        FILE_PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
        FILES_MATCHING 
        PATTERN "*.xml"
        )
  elseif(APPLE)
    set(CPACK_PACKAGE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/BioGearsLogo.bmp")
    set(ARPURLINFOABOUT  "https://biogearsengine.com")
  
    set(CPACK_BUNDLE_NAME "BioGears")
    set(CPACK_BUNDLE_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist")
    set(CPACK_BUNDLE_ICON ${CPACK_PACKAGE_ICON}) 
    
    if(TARGET bg-cli) 
      install(CODE "
        include(BundleUtilities)
        message(STATUS \"Remember to set CMAKE_INSTALL_PATH to something eneding in .app\") 
 
        string(TOUPPER \${CMAKE_INSTALL_CONFIG_NAME} CONFIG)
        if( NOT UNIX )
          set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY \${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_\${CONFIG}} )
          set( CMAKE_LIBRARY_OUTPUT_DIRECTORY \${CMAKE_LIBRARY_OUTPUT_DIRECTORY_\${CONFIG}} )
          set( CMAKE_RUNTIME_OUTPUT_DIRECTORY \${CMAKE_RUNTIME_OUTPUT_DIRECTORY_\${CONFIG}} )
        endif()
        
  
        set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH})
  
        foreach(_dir IN LISTS CMAKE_PREFIX_PATH)
          list(APPEND THIRD_PARTY  \"\${_dir}\")
          list(APPEND THIRD_PARTY_LIB \"\${_dir}/lib\")
          list(APPEND THIRD_PARTY_BIN \"\${_dir}/bin\")
        endforeach()
        message(\"\${CMAKE_INSTALL_PREFIX}/$<TARGET_FILE_NAME:bg-cli>\")
        fixup_bundle(\${CMAKE_INSTALL_PREFIX}/$<TARGET_FILE_NAME:bg-cli>
                    \"\"
                    \"\${THIRD_PARTY_LIB};\${THIRD_PARTY_BIN}\"
        )
      "         
      )
    endif()
  
    install(DIRECTORY 
      ${PROJECT_SOURCE_DIR}/share/xsd 
      DESTINATION bin/
      FILE_PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
      )
  
  install(DIRECTORY 
      ${PROJECT_SOURCE_DIR}/share/data/
      DESTINATION bin/
      FILE_PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
      FILES_MATCHING 
      PATTERN "*.xml"
      PATTERN "*.xlsx"
      )
  install(DIRECTORY 
      ${PROJECT_SOURCE_DIR}/share/Scenarios
      DESTINATION bin/
      FILE_PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
      FILES_MATCHING 
      PATTERN "*.xml"
      )
  
  else()
    set(CPACK_STRIP_FILES "")
    set(CPACK_SOURCE_STRIP_FILES "")
  endif()
  set(CPACK_PACKAGE_EXECUTABLES "bg-cli;Command Line" "bg-scenario;Scenario Driver")
  
  include(CPack)
endif()
