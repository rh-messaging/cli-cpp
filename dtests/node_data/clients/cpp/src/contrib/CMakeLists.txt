PROJECT(dtests-cpp-contrib)

include_directories(
	optparse
)


add_library(
	dtests-cpp-contrib
	
	optparse/OptionParser.cpp
)