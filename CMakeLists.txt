
cmake_policy(SET CMP0052 NEW)
include(cmake/DefineForFiles.cmake)

list(APPEND UtilsLibraries "rapidjson|RAPIDJSON")
list(APPEND UtilsLibraries "lua.h|LUA")
list(APPEND UtilsLibraries "blockingconcurrentqueue.h|ATOMIC_QUEUE")

DefineForFiles(UtilsLibraries)

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
    )

add_library(utils 
            iostream/datetime.hpp
            iostream/datetime.cpp 
            ${STRING_SRC}
            ${RAPIDJSON_GENERATOR}
            ${LUA_CODE}
)
