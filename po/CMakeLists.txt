include(FindGettext)

# Find the translation tools
find_program(INTLTOOL_MERGE intltool-merge)
if(NOT INTLTOOL_MERGE)
  message(FATAL_ERROR "Could not find intltool-merge, please install the intltool package")
endif()

find_program(INTLTOOL_EXTRACT intltool-extract)
if(NOT INTLTOOL_EXTRACT)
  message(FATAL_ERROR "Could not find intltool-extract, please install the intltool package")
endif()

find_program(GETTEXT_XGETTEXT_EXECUTABLE xgettext)

set(DOMAIN ${PROJECT_NAME})
set(POT_FILE ${DOMAIN}.pot)
file(GLOB PO_FILES *.po)

file(GLOB_RECURSE I18N_SRC_FILES
  RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/*.qml
  ${CMAKE_SOURCE_DIR}/*.js
  ${CMAKE_SOURCE_DIR}/*.cpp)
list(APPEND I18N_SRC_FILES ${CMAKE_CURRENT_BINARY_DIR}/${DESKTOP_FILE}.in.in.h)
list(SORT I18N_SRC_FILES)

message("Found ${I18N_SRC_FILES}")

# Creates the .pot file containing the translations template
add_custom_target(${POT_FILE} ALL
  COMMENT "Generating translation template"
  # Extract the translatable messages from the desktop file 
  COMMAND ${INTLTOOL_EXTRACT} --update --type=gettext/ini
          --srcdir=${CMAKE_SOURCE_DIR}/data ${DESKTOP_FILE}.in.in
  # Update the translation file
  COMMAND ${GETTEXT_XGETTEXT_EXECUTABLE} -o ${POT_FILE}
          --from-code=UTF-8
          --c++ --qt --add-comments=TRANSLATORS
          --keyword=tr --keyword=tr:1,2 --keyword=N_ --keyword=_
          --package-name='${PROJECT}'
          --copyright-holder='Canonical Ltd.'
          --directory ${CMAKE_CURRENT_SOURCE_DIR} 
          ${I18N_SRC_FILES}
  # Copy the up2date translation file to the source directory
  COMMAND ${CMAKE_COMMAND} -E copy ${POT_FILE} ${CMAKE_CURRENT_SOURCE_DIR}
)

# Builds the binary translations catalog. For each language
# it finds source translations (*.po) for generating mo files
foreach(PO_FILE ${PO_FILES})
  get_filename_component(LANG ${PO_FILE} NAME_WE)
  gettext_process_po_files(${LANG} ALL PO_FILES ${PO_FILE})
  set(INSTALL_DIR ${CMAKE_INSTALL_LOCALEDIR}/${LANG}/LC_MESSAGES)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${LANG}.gmo
  DESTINATION ${INSTALL_DIR}
  RENAME ${DOMAIN}.mo)
endforeach(PO_FILE)

# make the translation files visible on qtcreator
file(GLOB TRANSLATION_FILES
     RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
     *.po *.pot)

add_custom_target(com_ubuntu_docviewer_translation_files ALL SOURCES ${TRANSLATION_FILES})

