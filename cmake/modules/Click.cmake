if(CLICK_MODE)
  set(QT_IMPORTS_DIR "/lib/${ARCH_TRIPLET}")
  set(CMAKE_INSTALL_PREFIX /)
  set(DATA_DIR /)

  # Path for ubuntu-docviewer-app executable
  set(BIN_DIR ${DATA_DIR}lib/${ARCH_TRIPLET}/bin)

  # If running in CLICK_MODE, include binary dependencies of docviewer
  set(GET_CLICK_DEPS_TOOL ${CMAKE_SOURCE_DIR}/tools/get-click-deps)
  set(DEPS_MANIFEST ${CMAKE_CURRENT_SOURCE_DIR}/click/docviewer-libs.json)
  set(CUSTOM_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/click/disable-lo-features.sh)
  set(UPSTREAM_LIBS_DIR ${CMAKE_SOURCE_DIR}/upstream-libs-${CLICK_ARCH})

  MESSAGE("Grabbing upstream libs to ${UPSTREAM_LIBS_DIR}")

  if(NO_CACHE)
    # It has been specified not to cache .click dependencies on the machine.
    # This is meant to be used for automatic builds (e.g. Jenkins Bot).
    execute_process(
      COMMAND mkdir ${UPSTREAM_LIBS_DIR}
      COMMAND ${GET_CLICK_DEPS_TOOL} -d -c ${CUSTOM_SCRIPT} ${DEPS_MANIFEST} ${CLICK_ARCH} ${UPSTREAM_LIBS_DIR}
    )
  else(NO_CACHE)
    # Cache the .click dependencies for next usage. (Default)
    # Useful on developer machine.
    file(MAKE_DIRECTORY ${UPSTREAM_LIBS_DIR})
    execute_process(
      COMMAND ${GET_CLICK_DEPS_TOOL} -c ${CUSTOM_SCRIPT} -t ${CMAKE_SOURCE_DIR}/.tmp-click-deps-${CLICK_ARCH} ${DEPS_MANIFEST} ${CLICK_ARCH} ${UPSTREAM_LIBS_DIR}
    )
  endif() #NO_CACHE

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
  
  MESSAGE("Installing LibreOffice from ${UPSTREAM_LIBS_DIR}/usr/lib/libreoffice to ${DATA_DIR}lib/libreoffice")
  INSTALL( DIRECTORY ${UPSTREAM_LIBS_DIR}/usr/lib/libreoffice/ DESTINATION ${DATA_DIR}lib/libreoffice )

else(CLICK_MODE)
  execute_process(
    COMMAND qmake -query QT_INSTALL_QML
    OUTPUT_VARIABLE QT_IMPORTS_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )

  if (QT_IMPORTS_DIR STREQUAL "")
     set(QT_IMPORTS_DIR "${CMAKE_INSTALL_FULL_LIBDIR}/qt5/qml")
     message(STATUS "QT_IMPORTS_DIR is empty using default one: ${QT_IMPORTS_DIR}")
  else()
     message(STATUS "QT_IMPORTS_DIR set to ${QT_IMPORTS_DIR}")
  endif()

  set(DATA_DIR ${CMAKE_INSTALL_DATADIR}/${APP_NAME})
endif(CLICK_MODE)
