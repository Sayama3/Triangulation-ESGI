cmake_minimum_required(VERSION 3.10)

project(ImGui_Raylib VERSION 1.91.6 DESCRIPTION "Bloat-free Graphical User interface for C++ with minimal dependencies.")

include(GNUInstallDirs)

add_library(imgui
		include/extras/FA6FreeSolidFontData.h
		include/extras/IconsFontAwesome6.h
		include/imconfig.h
		include/imgui.h
		include/imgui_impl_raylib.h
		include/imgui_internal.h
		include/imgui_stdlib.h
		include/imstb_rectpack.h
		include/imstb_textedit.h
		include/imstb_truetype.h
		include/rlImGui.h
		include/rlImGuiColors.h
		src/imgui.cpp
		src/imgui_demo.cpp
		src/imgui_draw.cpp
		src/imgui_stdlib.cpp
		src/imgui_tables.cpp
		src/imgui_widgets.cpp
		src/rlImGui.cpp
)


set_target_properties(imgui PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(imgui PROPERTIES SOVERSION 0)

target_include_directories(imgui PUBLIC include)
target_include_directories(imgui PRIVATE src)

target_link_libraries(imgui PUBLIC raylib)

set_property(TARGET imgui PROPERTY POSITION_INDEPENDENT_CODE ON)