cmake_minimum_required(VERSION 3.11) # FetchContent is available in 3.11+

# Dependencies
set(RAYLIB_VERSION 5.5)
find_package(raylib ${RAYLIB_VERSION} QUIET) # QUIET or REQUIRED

include(FetchContent)

if (NOT raylib_FOUND) # If there's none, fetch and build raylib

	FetchContent_Declare(
			raylib
			GIT_REPOSITORY https://github.com/raysan5/raylib.git
			GIT_TAG "5.5"
	)

	FetchContent_GetProperties(raylib)
	if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
		set(FETCHCONTENT_QUIET NO)
		FetchContent_MakeAvailable(raylib)
		set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
	endif()

endif()

FetchContent_Declare(
		glm
		GIT_REPOSITORY https://github.com/icaven/glm.git
		GIT_TAG master
)

set(GLM_ENABLE_CXX_20 ON)

FetchContent_MakeAvailable(glm)

add_subdirectory(ImGui-RayLib)