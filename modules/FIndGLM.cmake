message("Attempting to find GLM")

if(WIN32)
	set("GLM_INCLUDE_DIRS" ${CMAKE_SOURCE_DIR}/extern/glm/)
endif()