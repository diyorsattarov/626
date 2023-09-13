cmake_minimum_required(VERSION 3.2)

project(test)

set(CMAKE_CXX_STANDARD 11)  # Set your desired C++ standard here

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/test/bin)

add_executable(test_main
	test_main.cpp
)

find_package(Boost REQUIRED COMPONENTS system)
find_package(spdlog REQUIRED)
find_package(nlohmann_json)
find_package(OpenSSL REQUIRED)

target_link_libraries(test_main PRIVATE
	Boost::system 
    OpenSSL::SSL 
	OpenSSL::Crypto
	ws2_32
	spdlog::spdlog
    nlohmann_json::nlohmann_json
)

add_custom_target(clean_test
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/test/bin
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/test/CMakeFiles
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/test/CMakeCache.txt
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/test/Makefile
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/test/cmake_install.cmake
	COMMENT "Cleaning build files"
)