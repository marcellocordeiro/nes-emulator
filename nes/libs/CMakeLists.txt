cmake_minimum_required(VERSION 3.14)

project(nes-libs)

# Setting up the target

add_library(nes-libs INTERFACE)

target_include_directories(nes-libs INTERFACE "include")
