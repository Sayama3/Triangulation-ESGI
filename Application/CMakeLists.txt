cmake_minimum_required(VERSION 3.16)

set(APP_SRC_FILES
		src/main.cpp
		src/Application.cpp
		include/Application.hpp
		src/Scene.cpp
		include/Scene.hpp
		include/Render/Renderable.hpp
		include/Core/Singleton.hpp
		src/Render/Camera.cpp
		include/Render/Camera.hpp
		src/Render/EditorCamera.cpp
		include/Render/EditorCamera.hpp
		src/ImGuiLib_RaylibInputs.cpp
)

add_executable(${PROJECT_NAME} ${APP_SRC_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC include)
target_include_directories(${PROJECT_NAME} PRIVATE src)

target_link_libraries(${PROJECT_NAME} raylib imgui MathLib)

target_precompile_headers(${PROJECT_NAME} REUSE_FROM MathLib)

if(CMAKE_BUILD_TYPE MATCHES "[Dd][Ee][Bb][Uu][Gg]")
	file(CREATE_LINK ${CMAKE_SOURCE_DIR}/Assets/ ${CMAKE_CURRENT_BINARY_DIR}/Assets/ RESULT copy_result COPY_ON_ERROR SYMBOLIC)
	if(NOT (copy_result EQUAL 0))
		message(WARNING
				"Copy made instead of symlink.\n"
				"${copy_result}"
		)
	endif()
else()
	file(COPY ${CMAKE_SOURCE_DIR}/Assets/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Assets/)
endif()

target_compile_definitions(${PROJECT_NAME} PUBLIC APP_ASSETS=./Assets/)

# Web Configurations
if (${PLATFORM} STREQUAL "Web")
	set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html") # Tell Emscripten to build an example.html file.
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY -s GL_ENABLE_GET_PROC_ADDRESS=1")
endif()

# Checks if OSX and links appropriate frameworks (Only required on MacOS)
if (APPLE)
	target_link_libraries(${PROJECT_NAME} "-framework IOKit")
	target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
	target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()