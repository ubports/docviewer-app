if(CLICK_MODE)
  if(NOT BZR_REVNO)
    set(BZR_REVNO "latest")
  endif(NOT BZR_REVNO)

  configure_file(manifest.json.in ${CMAKE_CURRENT_BINARY_DIR}/manifest.json)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/manifest.json DESTINATION ${CMAKE_INSTALL_PREFIX})
  install(FILES docviewer.apparmor docviewer-content.json DESTINATION ${CMAKE_INSTALL_PREFIX})
endif(CLICK_MODE)


# Make the click files visible in Qt Creator
file(GLOB CLICK_FILES
  RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  *.json *.json.in *.apparmor
)

add_custom_target(com_ubuntu_docviewer_CLICKFiles ALL SOURCES ${CLICK_FILES})
