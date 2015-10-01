######## 
# This macro is used to install the headers
########
macro(InstallHeaders path dest)
	IF (NOT EXISTS ${CMAKE_BINARY_DIR}/${path})
		MESSAGE(FATAL_ERROR "Directory ${CMAKE_BINARY_DIR}/${path} does not exist")
	ENDIF (NOT EXISTS ${CMAKE_BINARY_DIR}/${path})
	
	MESSAGE(STATUS "Directory ${CMAKE_BINARY_DIR}/${path} added to distribution")
	FILE(GLOB HEADERLIST ${CMAKE_BINARY_DIR}/${path}/*.h*)
	FOREACH(file ${HEADERLIST})
		INSTALL(
			FILES ${file}
			DESTINATION ${CPP_INCLUDE_DIR}/${dest}
		)
	ENDFOREACH(file)
endmacro(InstallHeaders)


######## 
# These macro are used to install regular files
########
MACRO (InstallFile file dest)
	INSTALL(
		FILES ${CMAKE_BINARY_DIR}/${file}
		DESTINATION ${dest}
		PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
	)
ENDMACRO(InstallFile)

MACRO (InstallFileMask source mask dest) 
	FILE(GLOB files RELATIVE source mask)
	INSTALL(
		FILES ${files}
		DESTINATION ${dest}
		PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
	)
ENDMACRO(InstallFileMask)

