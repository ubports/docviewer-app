cmake_minimum_required(VERSION 2.8.9)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fno-permissive -pedantic -Wall -Wextra -fPIC")

#add the sources to compile
set(FILE_QML_PLUGIN_SOURCES
	backend.cpp
	backend.h
	docviewerFile.cpp
	docviewerFile.h
	fileReader.cpp
	fileReader.h
	)

#qt5_use_modules(Qml Quick)

# Copy qmldir file to build dir for running in QtCreator
if(NOT "${CMAKE_CURRENT_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_BINARY_DIR}")
add_custom_target(file-qml-plugin ALL
    COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/qmldir ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS ${QMLFILES}
)
endif(NOT "${CMAKE_CURRENT_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_BINARY_DIR}")
