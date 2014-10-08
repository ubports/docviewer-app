cmake_minimum_required(VERSION 2.8.9)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

#set cpp flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fno-permissive -pedantic -Wall -Wextra -fPIC")

# Standard install paths
include(GNUInstallDirs)

#add the sources to compile
set(FILE_QML_PLUGIN_SOURCES
	backend.cpp
	backend.h
	docviewerFile.cpp
	docviewerFile.h
	fileReader.cpp
	fileReader.h
	)

#should be deprecated in current cmake version
#qt5_use_modules(Qml Quick)

if(CLICK_MODE)
	#none
else(CLICK_MODE)
	#creates QT_IMPORTS_DIR and olds the qml plugin path
	execute_process(
            COMMAND qmake -query QT_INSTALL_QML
            OUTPUT_VARIABLE QT_IMPORTS_DIR
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )
endif(CLICK_MODE)


# Copy qmldir file to build dir for running in QtCreator
if(NOT "${CMAKE_CURRENT_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_BINARY_DIR}")
add_custom_target(file-qml-plugin ALL
    COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/qmldir ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS ${QMLFILES}
)
endif(NOT "${CMAKE_CURRENT_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_BINARY_DIR}")

# Install plugin file under org.docviewer.file
install(TARGETS file-qml-plugin DESTINATION ${QT_IMPORTS_DIR}/org.docviewer.file/)
install(FILES qmldir DESTINATION ${QT_IMPORTS_DIR}/org.docviewer.file/)
