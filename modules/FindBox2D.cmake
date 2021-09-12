if (WIN32)
	message("ATTEMPTING TO FIND Box2D")
	set(BOX2D_LIBS ${CMAKE_SOURCE_DIR}/extern/box2d/box2d.lib)
	set(BOX2D_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/extern/box2d/include)
else()
	set(BOX2D_LIBS "box2d::box2d")
endif()