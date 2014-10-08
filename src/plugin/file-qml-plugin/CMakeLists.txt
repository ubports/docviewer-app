cmake_minimum_required(VERSION 2.8.9)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(PLUGIN_DIR com/ubuntu/fileqmlplugin)

#set cpp flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fno-permissive -pedantic -Wall -Wextra -fPIC")

# Standard install paths
include(GNUInstallDirs)

#add the sources to compile
set(fileqmlplugin_SRCS
	backend.cpp
	backend.h
	docviewerFile.cpp
	docviewerFile.h
	fileReader.cpp
	fileReader.h
	)

#should be deprecated in current cmake version
#qt5_use_modules(Qml Quick)

add_library(fileqmlplugin MODULE
    ${fileqmlplugin_SRCS}
)

qt5_use_modules(fileqmlplugin Gui Qml Quick)

# Copy the plugin, the qmldir file and other assets to the build dir for running in QtCreator
if(NOT "${CMAKE_CURRENT_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_BINARY_DIR}")
    add_custom_command(TARGET fileqmlplugin POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/../${PLUGIN_DIR}
        COMMENT "Creating plugin directory layout in the build directory"
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/qmldir ${CMAKE_CURRENT_BINARY_DIR}/../${PLUGIN_DIR}
        COMMENT "Copying the qmldir file to the build directory"
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:fileqmlplugin> ${CMAKE_CURRENT_BINARY_DIR}/../${PLUGIN_DIR}
        COMMENT "Copying the plugin binary to the build directory"
    )
endif(NOT "${CMAKE_CURRENT_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_BINARY_DIR}")

# Install plugin file
install(TARGETS fileqmlplugin DESTINATION ${QT_IMPORTS_DIR}/com/ubuntu/fileqmlplugin/)
install(FILES qmldir DESTINATION ${QT_IMPORTS_DIR}/com/ubuntu/fileqmlplugin/)
