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
  set(DEPS_MANIFEST ${CMAKE_CURRENT_SOURCE_DIR}/click/docviewer-libs.json)
  set(CUSTOM_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/click/disable-file-locking.sh)
  set(UPSTREAM_LIBS_DIR ${CMAKE_BINARY_DIR}/upstream-libs)

  MESSAGE("Grabbing upstream libs to ${UPSTREAM_LIBS_DIR}") 

  get_filename_component(BLD_CONFIGURATION_NAME ${CMAKE_BINARY_DIR} NAME)
  set(UPSTREAM_CACHE $ENV{HOME}/dev/upstream-libs-docviewer/${BLD_CONFIGURATION_NAME})
  MESSAGE("Upstream libs cache path: ${UPSTREAM_CACHE}")  

  if(EXISTS "${UPSTREAM_CACHE}")
    MESSAGE("Upstream libs cache exists.") 
    file(COPY ${UPSTREAM_CACHE}/upstream-libs/ DESTINATION ${UPSTREAM_LIBS_DIR} PATTERN * )
  else()
    MESSAGE("Cache miss, downloading from network.") 
    execute_process(
      COMMAND mkdir ${UPSTREAM_LIBS_DIR}
      COMMAND ${GET_CLICK_DEPS_TOOL} -d ${DEPS_MANIFEST} -c ${CUSTOM_SCRIPT} ${CLICK_ARCH} ${UPSTREAM_LIBS_DIR}
    )
    # Cache for next usage.
    file(COPY ${UPSTREAM_LIBS_DIR} DESTINATION ${UPSTREAM_CACHE} )
  endif()

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
