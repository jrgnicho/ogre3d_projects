cmake_minimum_required(VERSION 2.6)

project(TinyXml)

set(LIBRARY_OUTPUT_PATH lib/${CMAKE_BUILD_TYPE})

include_directories(${PROJECT_SOURCE_DIR}/3rdparty)
include_directories(${PROJECT_SOURCE_DIR}/3rdparty/tinyxml)

add_definitions(-TIXML_USE_STL)

file(
	GLOB_RECURSE
	source_files
	*.h
	*.cpp
)

add_library(
	tinyxml
	SHARED
	${source_files}
)



#SET_TARGET_PROPERTIES (tinyxml PROPERTIES DEFINE_SYMBOL  "EXPORT" )

INSTALL( FILES tinyxml.h tinystr.h DESTINATION include/ )
INSTALL( FILES lib/libtinyxml.so DESTINATION lib/ )
