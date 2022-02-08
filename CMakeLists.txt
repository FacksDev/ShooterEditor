cmake_minimum_required(VERSION 3.10)

project(ShooterEditor)

set(CMAKE_CXX_STANDARD_REQUIRED 17)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(StraitXPackages/base)
add_subdirectory(StraitXPackages/imgui)
add_subdirectory(StraitXPackages/2d)

set(EDITOR_SOURCES
	
)

add_executable(ShooterEditor ${RED_SOURCES})
target_link_libraries(ShooterEditor StraitXBase StraitXImGui StraitX2D)