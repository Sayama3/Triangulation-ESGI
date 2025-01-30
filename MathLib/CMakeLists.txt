cmake_minimum_required(VERSION 3.16)

set(TRG_SRC_FILES
		include/TRG/Math.hpp
		src/Math.cpp
		include/TRG/Math/Raycast.hpp
		include/TRG/Math/Basics.hpp
		include/TRG/Math/Geometry.hpp
		include/TRG/Math/Types.hpp
		include/TRG/Math/Shells.hpp
)

add_library(MathLib STATIC ${TRG_SRC_FILES})

target_include_directories(MathLib PUBLIC include)
target_include_directories(MathLib PRIVATE src)

target_precompile_headers(MathLib PUBLIC
		# Multithreading headers
		<thread>
		<atomic>
		<future>
		<condition_variable>
		<mutex>
		<shared_mutex>
		<latch>
		<barrier>

		# Utility stuff
		<functional>
		<utility>
		<algorithm>
		<memory>
		<source_location>
		<chrono>
		<coroutine>
		<bitset>
		<execution>

		# Exception related stuff
		<exception>
		<stdexcept>

		# String & IO manipulation
		<filesystem>
		<string>
		<string_view>

		# Containers
		<array>
		<vector>
		<list>
		<unordered_set>
		<set>
		<unordered_map>
		<map>
		<any>
		<variant>
		<optional>
		<tuple>

		# C-Types Helpers
		<cstdint>
		<cstring>
		<cmath>
)

if(CMAKE_BUILD_TYPE MATCHES "[Dd][Ee][Bb][Uu][Gg]")
	target_compile_definitions(MathLib PUBLIC TRG_DEBUG)
elseif(CMAKE_BUILD_TYPE MATCHES "[Rr][Ee][Ll][Ee][Aa][Ss][Ee]")
	target_compile_definitions(MathLib PUBLIC TRG_RELEASE)
elseif(CMAKE_BUILD_TYPE MATCHES "[Dd][Ii][Ss][Tt]")
	target_compile_definitions(MathLib PUBLIC TRG_DIST)
endif()

target_compile_features(MathLib PUBLIC cxx_std_20)

if(TRG_USE_DOUBLE)
	message(STATUS "MathLib: using Double as the main floating number system.")
	target_compile_definitions(MathLib PUBLIC TRG_DOUBLE)
else()
	message(STATUS "MathLib: using Float as the main floating number system.")
	target_compile_definitions(MathLib PUBLIC TRG_FLOAT)
endif()

target_compile_definitions(MathLib PUBLIC GLM_ENABLE_EXPERIMENTAL=1)
target_link_libraries(MathLib PUBLIC glm)