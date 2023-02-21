# All cmake-generated files ...
set(cmake_generated CMakeCache.txt
                    cmake_install.cmake  
                    Makefile
                    CMakeFiles
                    CTestTestfile.cmake
                    Testing 
)

# In all project subdirectories
set(cmake_dirs ${CMAKE_BINARY_DIR} src/api/proton src/common src/contrib)

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

if (EXISTS ${CMAKE_BINARY_DIR}/target/doc)
    file(REMOVE_RECURSE ${CMAKE_BINARY_DIR}/target/doc)
endif (EXISTS ${CMAKE_BINARY_DIR}/target/doc)
