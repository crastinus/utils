#pragma once

extern "C" {
#include <lua.h>
}

#include <string>

namespace lua {
std::string stackdump(char const* function, lua_State* l) ;
std::string stackdump(lua_State* l) ;
void stackdump_stderr(char const* function, lua_State* l) ;
}
