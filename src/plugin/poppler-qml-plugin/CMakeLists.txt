set(PLUGIN_DIR com/ubuntu/popplerqmlplugin)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

#add the sources to compile
set(popplerqmlplugin_SRCS
	plugin.cpp
        pdfdocument.cpp
	pageImageProvider.cpp
	pagesWorkerThread.cpp
	pdfPage.cpp
)

add_library(popplerqmlplugin MODULE
    ${popplerqmlplugin_SRCS}
)

target_link_libraries(popplerqmlplugin poppler-qt5)

qt5_use_modules(popplerqmlplugin Gui Qml Quick)

# Copy the plugin, the qmldir file and other assets to the build dir for running in QtCreator
if(NOT "${CMAKE_CURRENT_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_BINARY_DIR}")
    add_custom_command(TARGET popplerqmlplugin POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/../${PLUGIN_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/qmldir ${CMAKE_CURRENT_BINARY_DIR}/../${PLUGIN_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:popplerqmlplugin> ${CMAKE_CURRENT_BINARY_DIR}/../${PLUGIN_DIR}
    )
endif(NOT "${CMAKE_CURRENT_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_BINARY_DIR}")

# Install plugin file
install(TARGETS popplerqmlplugin DESTINATION ${QT_IMPORTS_DIR}/${PLUGIN_DIR})
install(FILES qmldir DESTINATION ${QT_IMPORTS_DIR}/${PLUGIN_DIR})
