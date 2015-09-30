PROJECT(dtests-cpp-contrib)

include_directories(
	optparse
)


add_library(
	dtests-cpp-contrib
	
	optparse/OptionParser.cpp
)


install(TARGETS dtests-cpp-contrib
	LIBRARY DESTINATION ${CPP_LIBRARY_DIR}
	RUNTIME DESTINATION ${CPP_LIBRARY_DIR}
        ARCHIVE DESTINATION ${CPP_LIBRARY_DIR}
)


InstallHeaders(src/contrib/optparse contrib/optparse)