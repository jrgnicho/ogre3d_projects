cmake_minimum_required(VERSION 2.4.6)
include($ENV{ROS_ROOT}/core/rosbuild/rosbuild.cmake)

# Set the build type.  Options are:
#  Coverage       : w/ debug symbols, w/o optimization, w/ code-coverage
#  Debug          : w/ debug symbols, w/o optimization
#  Release        : w/o debug symbols, w/ optimization
#  RelWithDebInfo : w/ debug symbols, w/ optimization
#  MinSizeRel     : w/o debug symbols, w/ optimization, stripped binaries
#set(ROS_BUILD_TYPE RelWithDebInfo)

rosbuild_init()

# start of ogre setup
project(OGRE_BASIC)

if(UNIX)
	if(EXISTS "/usr/local/lib/OGRE/cmake")

	  set(CMAKE_MODULE_PATH "/usr/local/lib/OGRE/cmake/;${CMAKE_MODULE_PATH}")
	  set(OGRE_SAMPLES_INCLUDEPATH "/usr/local/share/OGRE/samples/Common/include/") # We could just *assume* that developers uses this basepath : /usr/local
	  set(OGRE_MEDIA_PATH "/usr/share/OGRE/")

	elseif(EXISTS "/usr/lib/OGRE/cmake")

	  set(CMAKE_MODULE_PATH "/usr/lib/OGRE/cmake/;${CMAKE_MODULE_PATH}")
	  set(OGRE_SAMPLES_INCLUDEPATH "/usr/share/OGRE/Samples/Common/include/") # Otherwise, this one
  	  set(OGRE_MEDIA_PATH "/usr/share/OGRE/")
	  message(STATUS "Found Ogre module path .")

	else ()
	  message(SEND_ERROR "Failed to find module path.")
	endif(EXISTS "/usr/local/lib/OGRE")


	############ adding bullet physics paths ############
	if(EXISTS "/usr/local/include/bullet")
	  set(BULLET_PHYSICS_INCLUDE_DIRS "/usr/local/include/bullet")
	  set(BULLET_PHYSICS_LIBRARIES "/usr/local/lib/")
	else()
	  message(SEND_ERROR "Failed to find bullet path")
	endif(EXISTS "/usr/local/include/bullet")
	############ end of bullet physics paths ############

endif(UNIX)

#if (CMAKE_BUILD_TYPE STREQUAL "")
  # CMake defaults to leaving CMAKE_BUILD_TYPE empty. This screws up
  # differentiation between debug and release builds.
#  set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug #Release RelWithDebInfo MinSizeRel." FORCE)
#endif ()

# Find Boost
if (NOT OGRE_BUILD_PLATFORM_IPHONE)
	if (WIN32 OR APPLE)
		set(Boost_USE_STATIC_LIBS TRUE)
	else ()
		# Statically linking boost to a dynamic Ogre build doesn't work on Linux 64bit
		set(Boost_USE_STATIC_LIBS ${OGRE_STATIC})
	endif ()
	if (MINGW)
		# this is probably a bug in CMake: the boost find module tries to look for
		# boost libraries with name libboost_*, but CMake already prefixes library
		# search names with "lib". This is the workaround.
		set(CMAKE_FIND_LIBRARY_PREFIXES ${CMAKE_FIND_LIBRARY_PREFIXES} "")
	endif ()
	set(Boost_ADDITIONAL_VERSIONS "1.44" "1.44.0" "1.42" "1.42.0" "1.41.0" "1.41" "1.40.0" "1.40" "1.39.0" "1.39" "1.38.0" "1.38" "1.37.0" "1.37" )
	# Components that need linking (NB does not include header-only components like bind)
	set(OGRE_BOOST_COMPONENTS thread date_time)
	find_package(Boost COMPONENTS ${OGRE_BOOST_COMPONENTS} QUIET)
	if (NOT Boost_FOUND)
		# Try again with the other type of libs
		set(Boost_USE_STATIC_LIBS NOT ${Boost_USE_STATIC_LIBS})
		find_package(Boost COMPONENTS ${OGRE_BOOST_COMPONENTS} QUIET)
	endif()
	find_package(Boost QUIET)
 
	# Set up referencing of Boost
	include_directories(${Boost_INCLUDE_DIR})
	add_definitions(-DBOOST_ALL_NO_LIB)
	set(OGRE_LIBRARIES ${OGRE_LIBRARIES} ${Boost_LIBRARIES})
endif()

###################### start of bullet setup ###################### 

#include_directories(
#${BULLET_PHYSICS_SOURCE_DIR}/Extras  
#${BULLET_PHYSICS_SOURCE_DIR}/Extras/GIMPACTUtils  
#${BULLET_PHYSICS_SOURCE_DIR}/Extras/HACD
#${BULLET_PHYSICS_SOURCE_DIR}/Extras/ConvexDecomposition  
#${BULLET_PHYSICS_SOURCE_DIR}/Extras/LibXML 
#${BULLET_PHYSICS_SOURCE_DIR}/Extras/LibXML/include 
#${BULLET_PHYSICS_SOURCE_DIR}/src 
#${BULLET_PHYSICS_SOURCE_DIR}/Demos/OpenGL 
#${BULLET_PHYSICS_SOURCE_DIR}/Extras/Serialize/BulletFileLoader
#${BULLET_PHYSICS_SOURCE_DIR}/Extras/Serialize/BulletWorldImporter
#${VECTOR_MATH_INCLUDE})

#link_directories(${BULLET_PHYSICS_LIBRARIES})
#add_definitions(${BULLET_PHYSICS_DEFINITIONS})
######################  end of bullet setup ###################### 

#set the default path for built executables to the "bin" directory
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#set the default path for built libraries to the "lib" directory
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
# sets the binary directory bin as dist
#set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/dist")

#uncomment if you have defined messages
#rosbuild_genmsg()
#uncomment if you have defined services
#rosbuild_gensrv()

#common commands for building c++ executables and libraries
#rosbuild_add_library(${PROJECT_NAME} src/example.cpp)
#target_link_libraries(${PROJECT_NAME} another_library)
#rosbuild_add_boost_directories()
#rosbuild_link_boost(${PROJECT_NAME} thread)
#rosbuild_add_executable(example examples/example.cpp)
#target_link_libraries(example ${PROJECT_NAME})

find_package(OGRE REQUIRED)
find_package(OIS REQUIRED)

include_directories(${BULLET_PHYSICS_INCLUDE_DIRS})
link_directories(${BULLET_PHYSICS_LIBRARIES})

include_directories(${OGRE_INCLUDE_DIRS})
link_directories(${OGRE_LIBRARIES})
add_definitions(${OGRE_DEFINITIONS})

include_directories(${OIS_INCLUDE_DIRS})
link_directories( ${OIS_LIBRARIES} )
add_definitions(${OGRE_DEFINITIONS})

add_library(StateManager src/StateManager.cpp src/TestState.cpp src/TestDisplayImageState.cpp src/InputManager.cpp src/CameraController.cpp)
target_link_libraries(StateManager ${OIS_LIBRARIES} ${OGRE_LIBRARIES} ${BULLET_PHYSICS_LIBRARIES})

add_executable(test_class src/test_class.cpp)
target_link_libraries(test_class ${OIS_LIBRARIES} ${OGRE_LIBRARIES}  ${BULLET_PHYSICS_LIBRARIES} StateManager)

#post build
#file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/dist/bin)
#file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/dist/media)
#file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/dist/bin)
file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/media)
 
# post-build copy for win32
if(WIN32 AND NOT MINGW)
	add_custom_command( TARGET OgreApp PRE_BUILD
		COMMAND if not exist .\\dist\\bin mkdir .\\dist\\bin )
	add_custom_command( TARGET OgreApp POST_BUILD
		COMMAND copy \"$(TargetPath)\" .\\dist\\bin )
endif(WIN32 AND NOT MINGW)

if(MINGW OR UNIX)
	#set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/dist/bin)
endif(MINGW OR UNIX)

if(UNIX)
	message(STATUS "Adding files and directories")	

#	install(TARGETS test_class
#		RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin
#		CONFIGURATIONS All)

	configure_file(${OGRE_MEDIA_PATH}/plugins.cfg ${EXECUTABLE_OUTPUT_PATH}/plugins.cfg COPYONLY)
	configure_file(${OGRE_MEDIA_PATH}/resources.cfg ${EXECUTABLE_OUTPUT_PATH}/resources.cfg COPYONLY)
	configure_file(${OGRE_MEDIA_PATH}/samples.cfg ${EXECUTABLE_OUTPUT_PATH}/samples.cfg COPYONLY)
	configure_file(${OGRE_MEDIA_PATH}/quakemap.cfg ${EXECUTABLE_OUTPUT_PATH}/quakemap.cfg COPYONLY)
	execute_process(COMMAND cp -R ${OGRE_MEDIA_PATH}/media/ ${PROJECT_SOURCE_DIR})
 	
#	install(DIRECTORY ${OGRE_MEDIA_PATH}/media
#		DESTINATION ./
#		CONFIGURATIONS Release RelWithDebInfo Debug
#	)
 
#	install(FILES ${OGRE_MEDIA_PATH}/bin/plugins.cfg
#		${OGRE_MEDIA_PATH}/bin/resources.cfg
#		DESTINATION bin
#		CONFIGURATIONS Release RelWithDebInfo Debug
#	)
 
endif(UNIX)
