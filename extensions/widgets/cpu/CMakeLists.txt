
set(QT_USE_OPENGL TRUE)
# -> this will cause cmake to include and link against the OpenGL module
include(${QT_USE_FILE})

INCLUDE_DIRECTORIES(
	   ${QT_INCLUDES}
	   ${QT_QTOPENGL_INCLUDE_DIR}
	   ${QT_QTNETWORK_INCLUDE_DIR}
	   ${QT_QTTEST_INCLUDE_DIR}
		 )


SET (plexycpusrc
	cpu.cpp
	cpuwidget.cpp
		)

add_library (plexycpu SHARED ${plexycpusrc})
QT4_AUTOMOC( ${plexycpusrc}) 

#SET_TARGET_PROPERTIES(plexy
#
#	·       ·       PROPERTIES
	#	#	·       ·       COMPILE_FLAGS ${CMAKE_SHARED_LIBRARY_CXX_FLAGS})
#

TARGET_LINK_LIBRARIES(
	       plexycpu
			qtviz
		   ${QT_QTGUI_LIBRARY}
	       ${OPENGL_LIBRARIES}
	       ${QT_QTCORE_LIBRARY}
	       ${QT_QTOPENGL_LIBRARY}
       )

INSTALL(TARGETS plexycpu DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/plexyext/)
install(FILES cpu.desktop DESTINATION ${CMAKE_INSTALL_PREFIX}/ext/groups/)

add_subdirectory(cpu)
