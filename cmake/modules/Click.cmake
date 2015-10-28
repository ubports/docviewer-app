if(CLICK_MODE)
  if(NOT DEFINED BZR_SOURCE)
    set(BZR_SOURCE "lp:${LP_PROJECT}")
    message("-- Setting BZR_SOURCE to ${BZR_SOURCE}")
  endif(NOT DEFINED BZR_SOURCE)

  set(QT_IMPORTS_DIR "/lib/${ARCH_TRIPLET}")
  set(CMAKE_INSTALL_PREFIX /)
  set(DATA_DIR /)

  # Path for ubuntu-docviewer-app executable
  set(BIN_DIR ${DATA_DIR}lib/${ARCH_TRIPLET}/bin)

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
else(CLICK_MODE)
  execute_process(
    COMMAND qmake -query QT_INSTALL_QML
    OUTPUT_VARIABLE QT_IMPORTS_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )

  set(DATA_DIR ${CMAKE_INSTALL_DATADIR}/${APP_NAME})
endif(CLICK_MODE)
