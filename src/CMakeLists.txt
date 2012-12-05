include_directories(${QT_INCLUDES})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(APPNAME "version-ctl")

set(SOURCES
	versionheaderwriter.cpp
	versionfilewriter.cpp
	versionheaderreader.cpp
	versionfilereader.cpp
	versionwriter.cpp
	versionreader.cpp
	main.cpp
)

set(HEADERS
)

set(FORMS
)

set(RESOURCES
)

qt4_wrap_cpp(MOC_SOURCES ${HEADERS})
qt4_wrap_ui(FORM_SOURCES ${FORMS})
qt4_add_resources(RES_SOURCES ${RESOURCES})

# Create the actual executable
add_executable("${APPNAME}" ${SOURCES} ${MOC_SOURCES} ${FORM_SOURCES} ${RES_SOURCES})
target_link_libraries("${APPNAME}" ${QT_QTCORE_LIBRARY}
	#${QT_QTGUI_LIBRARY}
	#${QT_QTSQL_LIBRARY}
	#${QT_QTNETWORK_LIBRARY}
)


install(TARGETS "${APPNAME}"
	RUNTIME DESTINATION ${EXECUTABLE_INSTALL_PATH}
	LIBRARY DESTINATION ${LIBRARY_INSTALL_PATH}
	RUNTIME DESTINATION ${EXECUTABLE_INSTALL_PATH}
)


# Add unit tests
# add_subdirectory(unittests)
