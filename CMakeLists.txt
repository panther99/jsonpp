cmake_minimum_required(VERSION "3.27.6")

cmake_path(SET toolchain_root_path "$ENV{VCPKG_ROOT}")
set(CMAKE_TOOLCHAIN_FILE "${toolchain_root_path}/scripts/buildsystems/vcpkg.cmake")

project("jsonpp")

add_executable("${PROJECT_NAME}"
	jsonpp/Parser.cpp
	jsonpp/Main.cpp
)

target_include_directories("${PROJECT_NAME}" PRIVATE "jsonpp")

set_property(TARGET "${PROJECT_NAME}"
	PROPERTY
		CXX_STANDARD 20
)
