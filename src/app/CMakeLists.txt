file(GLOB_RECURSE QML_SRCS *.qml *.js)

set(docviewer_SRCS
    main.cpp
    ${QML_SRCS}
)

add_executable(docviewer ${docviewer_SRCS})

qt5_use_modules(docviewer Gui Qml Quick)

if(NOT "${CMAKE_CURRENT_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_BINARY_DIR}")
add_custom_target(docviewer-qmlfiles ALL
    COMMAND cp -r ${CMAKE_CURRENT_SOURCE_DIR}/qml ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS ${QMLFILES}
)
endif(NOT "${CMAKE_CURRENT_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_BINARY_DIR}")

install(DIRECTORY qml DESTINATION ${DATA_DIR})
if(CLICK_MODE)
  install(TARGETS docviewer DESTINATION ${BIN_DIR})
else()
  install(TARGETS docviewer RUNTIME DESTINATION bin)
endif()
