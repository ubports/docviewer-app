set(PLUGIN_DIR DocumentViewer/LibreOffice)

file(GLOB_RECURSE QML_SRCS
    qml/*.qml
    qml/*.js
)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/LibreOfficeKit/
)

#add the sources to compile
set(libreofficetoolkitqmlplugin_SRCS
    plugin.cpp
    lodocument.cpp
    loview.cpp
    sgtileitem.cpp
    lopartsimageprovider.cpp
    lopartsmodel.cpp
    renderengine.cpp
    ${QML_SRCS}
)

#add the headers to compile
set(libreofficetoolkitqmlplugin_HDRS
    twips.h
    config.h
)

add_library(libreofficetoolkitqmlplugin MODULE
    ${libreofficetoolkitqmlplugin_SRCS}
    ${libreofficetoolkitqmlplugin_HDRS}
)

#target_link_libraries(libreofficetoolkitqmlplugin dl)
qt5_use_modules(libreofficetoolkitqmlplugin Gui Qml Quick)

# Copy the plugin, the qmldir file and other assets to the build dir for running in QtCreator
if(NOT "${CMAKE_CURRENT_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_BINARY_DIR}")
add_custom_command(TARGET libreofficetoolkitqmlplugin POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/../${PLUGIN_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/qmldir ${CMAKE_CURRENT_BINARY_DIR}/../${PLUGIN_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:libreofficetoolkitqmlplugin> ${CMAKE_CURRENT_BINARY_DIR}/../${PLUGIN_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy ${QML_SRCS} ${CMAKE_CURRENT_BINARY_DIR}/../${PLUGIN_DIR}
)
endif(NOT "${CMAKE_CURRENT_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_BINARY_DIR}")

# Install plugin file
install(TARGETS libreofficetoolkitqmlplugin DESTINATION ${QT_IMPORTS_DIR}/${PLUGIN_DIR})
install(FILES qmldir DESTINATION ${QT_IMPORTS_DIR}/${PLUGIN_DIR})
install(FILES ${QML_SRCS} DESTINATION ${QT_IMPORTS_DIR}/${PLUGIN_DIR})
