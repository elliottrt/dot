cmake_minimum_required(VERSION 3.31.5)

project(dot VERSION 1.0.0
		DESCRIPTION "A programming language focused on the . operator "
		LANGUAGES CXX)

# generate compile_commands.json for IDE integration
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB dot_source src/*.cpp)
file(GLOB dot_include include/*.h)

add_executable(dot ${dot_source} ${dot_include})

target_compile_features(dot PUBLIC cxx_std_11)
target_include_directories(dot PRIVATE include)

# make sure all errors are known
target_compile_options(dot PRIVATE "-Wall" "-Wextra" "-Wpedantic")

# test support with test.py

include(CTest)

add_test(NAME general COMMAND python3 ../test.py ./dot ../tests)
add_test(NAME examples COMMAND python3 ../test.py ./dot ../examples)

add_custom_target(update
                  COMMAND python3 ../test.py ./dot ../tests --update
				  COMMAND python3 ../test.py ./dot ../examples --update
)
