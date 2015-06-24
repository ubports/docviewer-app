set(PLUGIN_DIR DocumentViewer/LibreOffice)
include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${Qt5Quick_PRIVATE_INCLUDE_DIRS}
	${Qt5Qml_PRIVATE_INCLUDE_DIRS}
        /usr/include/LibreOfficeKit/
)

#add the sources to compile
set(libreofficetoolkitqmlplugin_SRCS
    plugin.cpp
    lodocument.cpp
    loimageprovider.cpp
    verticalview.cpp
)

add_library(libreofficetoolkitqmlplugin MODULE
    ${libreofficetoolkitqmlplugin_SRCS}
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
endif(NOT "${CMAKE_CURRENT_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_BINARY_DIR}")

# Install plugin file
install(TARGETS libreofficetoolkitqmlplugin DESTINATION ${QT_IMPORTS_DIR}/${PLUGIN_DIR})
install(FILES qmldir DESTINATION ${QT_IMPORTS_DIR}/${PLUGIN_DIR})
