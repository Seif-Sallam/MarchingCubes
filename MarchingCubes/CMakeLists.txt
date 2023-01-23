cmake_minimum_required(VERSION 3.16)

add_executable(MarchingCubes
	src/MarchingCubes/main.cpp
)

target_link_libraries(MarchingCubes
	PUBLIC
	opengl32
	sfml-graphics
	sfml-window
	sfml-system
	sfml-audio
)

target_include_directories(MarchingCubes
	PRIVATE
	./include/
)
