
cmake_policy(SET CMP0052 NEW)
include(cmake/DefineForFiles.cmake)

list(APPEND UtilsLibraries "rapidjson|RAPIDJSON")
list(APPEND UtilsLibraries "lua.h|LUA")
list(APPEND UtilsLibraries "blockingconcurrentqueue.h|ATOMIC_QUEUE")

DefineForFiles(UtilsLibraries)

set(FILESYSTEM_SRC filesystem/filesystem.hpp filesystem/extra.hpp)
set(PROCESS_SRC 
    process/cli_arguments.hpp 
    process/cli_arguments.cpp 
    process/process.hpp
    process/process.cpp
    )


include_directories(. ..)

if (UNIX) 
    set(FILESYSTEM_SRC ${FILESYSTEM_SRC}
        filesystem/platform/linux/filesystem.cpp
        filesystem/extra.cpp
        )
    set(PROCESS_SRC ${PROCESS_SRC}
        process/platform/linux/process.hpp
        process/platform/linux/process.cpp
    )
endif(UNIX)

if (RAPIDJSON)
    message(STATUS "Use Rapidjson")
    set(RAPIDJSON_GENERATOR
        reflection/json/iterator_stream.hpp
        reflection/json/rj_reading_visitor.cpp
        reflection/json/rj_reading_visitor.hpp
        reflection/json/rj_writing_visitor.cpp
        reflection/json/rj_writing_visitor.hpp
        reflection/json/std_stream.hpp
    )
endif(RAPIDJSON)

if (LUA)
    message(STATUS "Use LUA")
    set(LUA_CODE
        reflection/lua/reading_visitor.cpp
        reflection/lua/reading_visitor.hpp
        reflection/lua/writing_visitor.cpp
        reflection/lua/writing_visitor.hpp
        reflection/lua/helper.hpp
        reflection/lua/helper.cpp
        reflection/lua/execute.cpp
        reflection/lua/execute.hpp
    )
endif(LUA)

set(STRING_SRC 
        string/build.hpp
        string/size_for_output_containers.hpp
        string/size_for_output.hpp
        string/string.cpp
        string/string.hpp
        string/string_streambuf.cpp
        string/string_streambuf.hpp
        string/sha1sum.hpp
        string/sha1sum.cpp
    )

add_library(utils 
            iostream/datetime.hpp
            iostream/datetime.cpp 
            ${PROCESS_SRC}
            ${FILESYSTEM_SRC}            
            ${STRING_SRC}
            ${RAPIDJSON_GENERATOR}
            ${LUA_CODE}
)
