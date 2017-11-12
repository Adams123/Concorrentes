#include directories helps to include header files
include_directories(".")
include_directories(${INCLUDE_DIR})

#get the source filenames to compile
file(GLOB_RECURSE SOURCES *.c)

add_definitions("${CFLAGS}")
link_libraries("${LINK_LIBS}")

#creating object file list from the sources ignoring main file
foreach(filename ${SOURCES})

	if("${filename}" MATCHES "main.c")

	else("${filename}" MATCHES "main.c")
		list(APPEND OBJECTS ${filename})
	endif("${filename}" MATCHES "main.c")

endforeach(filename ${SOURCES})

if(${PRJ_TYPE_EXECUTABLE})
	#create executable if the project type is PRJ_TYPE_EXECUTABLE
	add_executable(${PRJ_NAME} ${SOURCES})

else(${PRJ_TYPE_EXECUTABLE})

	#creating libraries if not a executable project
	add_library(${PRJ_NAME}-static STATIC ${OBJECTS})
	add_library(${PRJ_NAME}-shared SHARED ${OBJECTS})
endif(${PRJ_TYPE_EXECUTABLE})

find_package(MPI REQUIRED)

include_directories(${MPI_INCLUDE_PATH})
target_link_libraries(${PRJ_NAME} ${MPI_LIBRARIES})

if(MPI_COMPILE_FLAGS)
    set_target_properties(${PRJ_NAME} PROPERTIES
    COMPILE_FLAGS "${MPI_COMPILE_FLAGS}")
endif()

if(MPI_LINK_FLAGS)
    set_target_properties(${PRJ_NAME} PROPERTIES
    LINK_FLAGS "${MPI_LINK_FLAGS}")
endif()
