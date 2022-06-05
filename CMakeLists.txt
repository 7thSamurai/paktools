cmake_minimum_required(VERSION 3.8)

project(paktools)

add_executable(pakinfo src/pakinfo.cpp src/pakfile.cpp)
target_compile_features(pakinfo PRIVATE cxx_std_17)

add_executable(unpak src/unpak.cpp src/pakfile.cpp)
target_compile_features(unpak PRIVATE cxx_std_17)
