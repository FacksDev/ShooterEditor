cmake_minimum_required(VERSION 3.10)

project(ShooterEditor)

set(CMAKE_CXX_STANDARD_REQUIRED 17)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(StraitXPackages/Base)
add_subdirectory(StraitXPackages/ImGui)

set(EDITOR_SOURCES
	
)

add_executable(ShooterEditor ${RED_SOURCES})
target_link_libraries(ShooterEditor StraitXBase StraitXImGui)