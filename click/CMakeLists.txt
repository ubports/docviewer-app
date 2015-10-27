if(CLICK_MODE)
  if(NOT BZR_REVNO)
    set(BZR_REVNO "latest")
  endif(NOT BZR_REVNO)

  configure_file(manifest.json.in ${CMAKE_CURRENT_BINARY_DIR}/manifest.json)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/manifest.json DESTINATION ${CMAKE_INSTALL_PREFIX})
  install(FILES docviewer.apparmor docviewer-content.json DESTINATION ${CMAKE_INSTALL_PREFIX})

  # If running in CLICK_MODE, include binary dependencies of docviewer
  set(GET_CLICK_DEPS_TOOL ${CMAKE_SOURCE_DIR}/tools/get-click-deps)
  set(DEPS_MANIFEST ${CMAKE_CURRENT_SOURCE_DIR}/docviewer-libs.json)
  set(UPSTREAM_LIBS_DIR ${CMAKE_BINARY_DIR}/upstream-libs)

  MESSAGE("Grabbing upstream libs to ${UPSTREAM_LIBS_DIR}")
  execute_process(
    COMMAND mkdir ${UPSTREAM_LIBS_DIR}
    COMMAND ${GET_CLICK_DEPS_TOOL} -d ${DEPS_MANIFEST} ${CLICK_ARCH} ${UPSTREAM_LIBS_DIR}
  )

  MESSAGE("Installing upstream libs from ${UPSTREAM_LIBS_DIR}/usr/lib/${ARCH_TRIPLET}/ to ${DATA_DIR}lib/${ARCH_TRIPLET}")
  file(GLOB_RECURSE UPSTREAM_LIBS "${UPSTREAM_LIBS_DIR}/usr/lib/${ARCH_TRIPLET}/*")
  foreach(ITEM ${UPSTREAM_LIBS})
    IF( IS_DIRECTORY "${ITEM}" )
      LIST( APPEND DIRS_TO_DEPLOY "${ITEM}" )
    ELSE()
      LIST( APPEND FILES_TO_DEPLOY "${ITEM}" )
    ENDIF()
  endforeach()
  MESSAGE("Following files to install:- ${FILES_TO_DEPLOY}")
  INSTALL( FILES ${FILES_TO_DEPLOY} DESTINATION ${DATA_DIR}lib/${ARCH_TRIPLET} )

  MESSAGE("Installing LibreOffice from ${UPSTREAM_LIBS_DIR}/opt/libreoffice/lib/libreoffice to ${DATA_DIR}lib/${ARCH_TRIPLET}/libreoffice")
  INSTALL( DIRECTORY ${UPSTREAM_LIBS_DIR}/opt/libreoffice/lib/libreoffice/ DESTINATION ${DATA_DIR}lib/${ARCH_TRIPLET}/libreoffice )
endif(CLICK_MODE)


# Make the click files visible in Qt Creator
file(GLOB CLICK_FILES
  RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  *.json *.json.in *.apparmor
)

add_custom_target(com_ubuntu_docviewer_CLICKFiles ALL SOURCES ${CLICK_FILES})
