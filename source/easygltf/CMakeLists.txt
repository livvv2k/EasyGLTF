project(easygltf)

set(SOURCE_FILE_PATH ${SOURCE_PATH}/easygltf)

set(INCLUDE_PATH_LIST
    ${HEADER_PATH}/easygltf
    ${SOURCE_FILE_PATH}
    ${EXTERNAL_PATH}/rapidjson/include
    )
include_directories(${INCLUDE_PATH_LIST})

set(CODE_FILE_LIST)

set(HEADER_FILE_LIST
    ${HEADER_PATH}/easygltf/easygltf.h
    )
set(CODE_FILE_LIST
    ${CODE_FILE_LIST}
    ${HEADER_FILE_LIST}
    )
source_group("header" FILES ${HEADER_FILE_LIST})

set(SOURCE_FILE_LIST
    ${SOURCE_FILE_PATH}/easygltf.cpp
    )
set(CODE_FILE_LIST
    ${CODE_FILE_LIST}
    ${SOURCE_FILE_LIST}
    )
source_group("source" FILES ${SOURCE_FILE_LIST})

set(LIBRARY_OUTPUT_PATH ${OUT_LIB_PATH})

add_library(easygltf STATIC ${CODE_FILE_LIST})

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    add_custom_command(TARGET easygltf
        POST_BUILD
        COMMAND xcopy ARGS "/D /S /Y \"${ROOT_PATH}/include\" \"${OUT_PATH}/include\\\""
        COMMENT "sync the head file"
        )
else()
    add_custom_command(TARGET easygltf
        POST_BUILD
        COMMAND rsync -a -delete ${ROOT_PATH}/include ${OUT_PATH}/
        COMMENT "sync the head file"
        )
endif()

set_target_properties(easygltf PROPERTIES FOLDER main)
set_target_properties(easygltf PROPERTIES LIBRARY_OUTPUT_DIRECTORY_RELEASE ${OUT_LIB_PATH})
