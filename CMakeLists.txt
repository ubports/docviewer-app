project(com.ubuntu.docviewer C CXX)
cmake_minimum_required(VERSION 2.8.9)

find_program(INTLTOOL_MERGE intltool-merge)
if(NOT INTLTOOL_MERGE)
  message(FATAL_ERROR "Could not find intltool-merge, please install the intltool package")
endif(NOT INTLTOOL_MERGE)

find_program(INTLTOOL_EXTRACT intltool-extract)
if(NOT INTLTOOL_EXTRACT)
  message(FATAL_ERROR "Could not find intltool-extract, please install the intltool package")
endif(NOT INTLTOOL_EXTRACT)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fno-permissive -pedantic -Wall -Wextra -fPIC")

# Debugging purpose. Keep commented unless you need it.
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG}")

include(FindPkgConfig)
# Standard install paths
include(GNUInstallDirs)

option(INSTALL_TESTS "Install the tests on make install" on)
option(CLICK_MODE "Installs to a contained location" on)

set(APP_NAME ubuntu-docviewer-app)
set(DESKTOP_FILE "${PROJECT_NAME}.desktop")
set(URLS_FILE "${PROJECT_NAME}.url-dispatcher")
set(LP_PROJECT ubuntu-docviewer-app)
set(ICON_FILE docviewer@30.png)
set(AUTOPILOT_DIR ubuntu_docviewer_app)
set(EXEC "${APP_NAME}")
set(UBUNTU_MANIFEST_PATH "manifest.json.in" CACHE INTERNAL "Relative path to the manifest file")

# Sets BZR_REVNO
execute_process(
    COMMAND bzr revno ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE BZR_REVNO
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

if(CLICK_MODE)
    if(NOT DEFINED BZR_SOURCE)
        set(BZR_SOURCE "lp:${LP_PROJECT}")
        message("-- Setting BZR_SOURCE to ${BZR_SOURCE}")
    endif(NOT DEFINED BZR_SOURCE)

    if(NOT BZR_REVNO)
        set(BZR_REVNO "latest")
    endif(NOT BZR_REVNO)

    # Find out the architecture for package building
    # to determine the plugin's installation path
    execute_process(
        COMMAND dpkg-architecture -qDEB_HOST_MULTIARCH
            OUTPUT_VARIABLE ARCH_TRIPLET
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    execute_process(
      COMMAND dpkg-architecture -qDEB_HOST_ARCH
      OUTPUT_VARIABLE CLICK_ARCH
      OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    set(CMAKE_INSTALL_PREFIX /)
    set(QT_IMPORTS_DIR "/lib/${ARCH_TRIPLET}")
    set(BIN_DIR /lib/${ARCH_TRIPLET}/bin)
    set(DATA_DIR /)
    set(ICON ${ICON_FILE})
    set(DESKTOP_DIR ${DATA_DIR})
    set(URLS_DIR ${DATA_DIR})
    configure_file(manifest.json.in ${CMAKE_CURRENT_BINARY_DIR}/manifest.json)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/manifest.json docviewer.apparmor docviewer-content.json ${URLS_FILE}
            DESTINATION ${CMAKE_INSTALL_PREFIX})
    # Make the click files visible in Qt Creator
    file(GLOB CLICK_FILES
         RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
         *.json *.json.in *.apparmor)

    add_custom_target(com_ubuntu_docviewer_CLICKFiles ALL SOURCES ${CLICK_FILES})
else(CLICK_MODE)
    # components PATH
    execute_process(
        COMMAND qmake -query QT_INSTALL_QML
            OUTPUT_VARIABLE QT_IMPORTS_DIR
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    set(DATA_DIR ${CMAKE_INSTALL_DATADIR}/${APP_NAME})
    set(ICON "${CMAKE_INSTALL_PREFIX}/${DATA_DIR}/${ICON_FILE}")
    set(DESKTOP_DIR ${CMAKE_INSTALL_DATADIR}/applications)
    set(APPLICATION_DIR ${CMAKE_INSTALL_DATADIR}/accounts/applications)
endif(CLICK_MODE)

file(GLOB_RECURSE I18N_SRC_FILES
     RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/po
     *.qml *.js *.cpp)
list(APPEND I18N_SRC_FILES ${CMAKE_CURRENT_BINARY_DIR}/po/${DESKTOP_FILE}.in.in.h)
list(SORT I18N_SRC_FILES)
message("Found ${I18N_SRC_FILES}")

file(GLOB SRC_FILES
     RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
     *.qml *.js)
add_custom_target(ubuntu-docviewer-app_QMlFiles ALL SOURCES ${SRC_FILES})

file(GLOB RESOURCE_FILES
     RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
     *.png *.svg)
install(FILES ${SRC_FILES} ${RESOURCE_FILES} ${ICON_FILE} DESTINATION ${DATA_DIR})

configure_file(${DESKTOP_FILE}.in.in ${DESKTOP_FILE}.in)

add_custom_target(${DESKTOP_FILE} ALL
  COMMENT "Merging translations into ${DESKTOP_FILE}"
  COMMAND LC_ALL=C ${INTLTOOL_MERGE} -d -u ${CMAKE_SOURCE_DIR}/po ${DESKTOP_FILE}.in ${DESKTOP_FILE}
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${DESKTOP_FILE} 
        DESTINATION ${DESKTOP_DIR})
 
# Tests
enable_testing()

add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(po)

# If running in CLICK_MODE, include binary dependencies of docviewer
if(CLICK_MODE)
    MESSAGE("Grabbing upstream libs to ${CMAKE_CURRENT_BINARY_DIR}/upstream-libs")
    execute_process(
        COMMAND mkdir ${CMAKE_CURRENT_BINARY_DIR}/upstream-libs
        COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/get-click-deps -d ${CMAKE_CURRENT_SOURCE_DIR}/docviewer-libs.json -c ${CMAKE_CURRENT_SOURCE_DIR}/click/disable-file-locking.sh ${CLICK_ARCH} ${CMAKE_CURRENT_BINARY_DIR}/upstream-libs
    )

    MESSAGE("Installing upstream libs from ${CMAKE_CURRENT_BINARY_DIR}/upstream-libs/usr/lib/${ARCH_TRIPLET}/ to ${DATA_DIR}lib/${ARCH_TRIPLET}")
    file(GLOB_RECURSE UPSTREAM_LIBS "${CMAKE_CURRENT_BINARY_DIR}/upstream-libs/usr/lib/${ARCH_TRIPLET}/*")
    foreach(ITEM ${UPSTREAM_LIBS})
        IF( IS_DIRECTORY "${ITEM}" )
            LIST( APPEND DIRS_TO_DEPLOY "${ITEM}" )
        ELSE()
            LIST( APPEND FILES_TO_DEPLOY "${ITEM}" )
        ENDIF()
    endforeach()
    MESSAGE("Following files to install:- ${FILES_TO_DEPLOY}")
    INSTALL( FILES ${FILES_TO_DEPLOY} DESTINATION ${DATA_DIR}lib/${ARCH_TRIPLET} )


    MESSAGE("Installing LibreOffice from ${CMAKE_CURRENT_BINARY_DIR}/upstream-libs/opt/libreoffice/lib/libreoffice to ${DATA_DIR}lib/${ARCH_TRIPLET}/libreoffice")
    INSTALL( DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/upstream-libs/opt/libreoffice/lib/libreoffice/ DESTINATION ${DATA_DIR}lib/${ARCH_TRIPLET}/libreoffice )
endif(CLICK_MODE)
