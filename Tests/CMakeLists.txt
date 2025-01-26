cmake_minimum_required(VERSION 3.16)

include(FetchContent)
FetchContent_Declare( googletest
		GIT_REPOSITORY https://github.com/google/googletest.git
		GIT_TAG "v1.15.2"
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable( TRG_Tests
		src/test_math.cpp
)

target_link_libraries( TRG_Tests PUBLIC
		MathLib
		GTest::gtest_main
)

target_include_directories(TRG_Tests PUBLIC include)
target_include_directories(TRG_Tests PRIVATE src)
target_precompile_headers(TRG_Tests REUSE_FROM MathLib)

include(GoogleTest)
gtest_discover_tests(TRG_Tests)

