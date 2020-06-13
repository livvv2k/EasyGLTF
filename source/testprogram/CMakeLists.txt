project(testprogram)

set(INCLUDE_PATH_LIST
    ${HEADER_PATH}
    ${EXTERNAL_PATH}/rapidjson/include
    )
include_directories(${INCLUDE_PATH_LIST})

set(EXECUTABLE_OUTPUT_PATH ${OUT_PATH}/testprogram)
add_executable(testprogram testprogram.cpp)
target_link_libraries(testprogram easygltf)
