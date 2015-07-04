set(PLUGIN_DIR DocumentViewer/LibreOffice)

# FIXME: QML files not imported during the build.
file(GLOB_RECURSE QML_SRCS *.qml *.js)
include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
        /usr/include/LibreOfficeKit/
)

#add the sources to compile
set(libreofficetoolkitqmlplugin_SRCS
    plugin.cpp
    lodocument.cpp
    loview.cpp
    tileitem.cpp
    ${QML_SRCS}
)

#add the headers to compile
set(libreofficetoolkitqmlplugin_HDRS
    twips.h
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
)
add_custom_target(libreofficetoolkitqmlplugin_QMlFiles ALL SOURCES ${QML_SRCS})
endif(NOT "${CMAKE_CURRENT_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_BINARY_DIR}")

# Install plugin file
install(TARGETS libreofficetoolkitqmlplugin DESTINATION ${QT_IMPORTS_DIR}/${PLUGIN_DIR})
install(FILES qmldir DESTINATION ${QT_IMPORTS_DIR}/${PLUGIN_DIR})
install(FILES ${QML_SRCS} DESTINATION ${QT_IMPORTS_DIR}/${PLUGIN_DIR})
