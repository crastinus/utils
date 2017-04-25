#include "execute.hpp"
#include <utils/string/string.hpp>
#include <utils/reflection/execute.hpp>

namespace execute {
namespace lua {
    int put_function(lua_State* L, char const* name) noexcept {
        int top_before = lua_gettop(L);

        auto tok = utils::split(name, '.');

        auto it = tok.begin();
        lua_getglobal(L, it->data());

        for (++it; it != tok.end(); ++it)
            lua_getfield(L, -1, it->data());

        int top_after = lua_gettop(L);

        if (!lua_isfunction(L, -1)) 
            return FUNCTION_DOES_NOT_EXIST;

        // objects_count=stack_count-functions_count
        return (top_after - top_before) - 1;
    }
}
}
