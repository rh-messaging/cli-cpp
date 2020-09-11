# All cmake-generated files ...
set(cmake_generated CMakeCache.txt
                    cmake_install.cmake  
                    Makefile
                    CMakeFiles
                    CTestTestfile.cmake
)

# In all project subdirectories
set(cmake_dirs ${CMAKE_BINARY_DIR} src/api/proton src/api/qpid src/common src/contrib)

# ... then traverse subdirectories
foreach(subdir ${cmake_dirs}) 

	# ... deleting all the cmake-generated files
	foreach(file ${cmake_generated})

	  if (EXISTS ${subdir}/${file})
		message("Removing ${subdir}/${file}")
	     	file(REMOVE_RECURSE ${subdir}/${file})
	  endif()

	endforeach(file)
endforeach(subdir)
