set(PLUGIN_DIR DocumentViewer)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

#add the sources to compile
set(fileqmlplugin_SRCS
	backend.cpp
	documentmodel.cpp
        fswatcher.cpp
	docviewerfile.cpp
)

add_library(fileqmlplugin MODULE
    ${fileqmlplugin_SRCS}
)

qt5_use_modules(fileqmlplugin Qml Quick)

# Copy the plugin, the qmldir file and other assets to the build dir for running in QtCreator
if(NOT "${CMAKE_CURRENT_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_BINARY_DIR}")
    add_custom_command(TARGET fileqmlplugin POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/../${PLUGIN_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/qmldir ${CMAKE_CURRENT_BINARY_DIR}/../${PLUGIN_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:fileqmlplugin> ${CMAKE_CURRENT_BINARY_DIR}/../${PLUGIN_DIR}
    )
endif(NOT "${CMAKE_CURRENT_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_BINARY_DIR}")

# Install plugin file
install(TARGETS fileqmlplugin DESTINATION ${QT_IMPORTS_DIR}/${PLUGIN_DIR})
install(FILES qmldir DESTINATION ${QT_IMPORTS_DIR}/${PLUGIN_DIR})
