cmake_minimum_required(VERSION 2.8.11)

project(easygltf)

set(ROOT_PATH "${easygltf_SOURCE_DIR}")
set(HEADER_PATH "${ROOT_PATH}/include")
set(SOURCE_PATH "${ROOT_PATH}/source")
set(OUT_PATH "${ROOT_PATH}/output")
set(OUT_LIB_PATH "${OUT_PATH}/lib/")
set(EXTERNAL_PATH "${ROOT_PATH}/external")

add_subdirectory(source)
