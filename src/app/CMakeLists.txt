file(GLOB_RECURSE QML_SRCS *.qml *.js)
file(GLOB_RECURSE IMAGE_FILES *.qml *.js)

pkg_check_modules(CONTENTHUB REQUIRED libcontent-hub)

set(docviewer_SRCS
    main.cpp
    content-communicator.cpp
    command-line-parser.cpp
    docviewer-application.cpp
    urlhandler.cpp
    ${QML_SRCS}
)

add_executable(ubuntu-docviewer-app ${docviewer_SRCS})

qt5_use_modules(ubuntu-docviewer-app Widgets Gui Qml Quick DBus)

target_link_libraries( ubuntu-docviewer-app
    ${CONTENTHUB_LIBRARIES}
)

if(NOT "${CMAKE_CURRENT_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_BINARY_DIR}")
add_custom_target(docviewer-qmlfiles ALL
    COMMAND cp -r ${CMAKE_CURRENT_SOURCE_DIR}/qml ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS ${QMLFILES}
)
endif(NOT "${CMAKE_CURRENT_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_BINARY_DIR}")

if(NOT "${CMAKE_CURRENT_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_BINARY_DIR}")
add_custom_target(docviewer-imagefiles ALL
    COMMAND cp -r ${CMAKE_CURRENT_SOURCE_DIR}/graphics ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS ${IMAGE_FILES}
)
endif(NOT "${CMAKE_CURRENT_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_BINARY_DIR}")

install(DIRECTORY qml DESTINATION ${DATA_DIR})
install(DIRECTORY graphics DESTINATION ${DATA_DIR})

if(CLICK_MODE)
  install(TARGETS ubuntu-docviewer-app DESTINATION ${BIN_DIR})
else()
  install(TARGETS ubuntu-docviewer-app RUNTIME DESTINATION bin)
endif()
