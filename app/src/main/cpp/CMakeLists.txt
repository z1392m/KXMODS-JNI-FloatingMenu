cmake_minimum_required(VERSION 3.22.1)
project(KXModsMenu)

add_library(KXModsMenu SHARED native-lib.cpp)
target_compile_features(KXModsMenu PRIVATE cxx_std_17)
target_compile_options(KXModsMenu PRIVATE -Wall -Wextra)
