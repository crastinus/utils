#include "helper.hpp"
#include <utils/string/build.hpp>
#include <iostream>

namespace lua {

template <typename... Arguments>
inline std::string stackdump_impl(lua_State* l, Arguments&&... args) {

    /*
     * DUMP Header
     */
    std::string result;
    unsigned result_size = 1024;
    // compute output string size
    using swallow = int[];
    (void)swallow{(result_size += utils::size_for_output(args), 0)...};

    result.reserve(result_size);

    utils::string_streambuf sb(result);
    std::ostream os(&sb);

    (void)swallow{(os << std::forward<Arguments>(args), 0)...};
    os << '\n';

    /*
     * DUMP STACK
     */

    int i;
    int top = lua_gettop(l);
    for (i = 1; i <= top; i++) {
        /* repeat for each level */
        int t = lua_type(l, i);
        switch (t) {
            case LUA_TSTRING: /* strings */
                os << "string: '" << lua_tostring(l, i) << "'\n";
                break;
            case LUA_TBOOLEAN: /* booleans */
                os << "boolean: " << (bool)lua_toboolean(l, i) << "\n";
                break;
            case LUA_TNUMBER: /* numbers */
                os << "number: " << lua_tonumber(l, i) << "\n";
                break;
            default: /* other values */
                os << lua_typename(l, t) << '\n';
                break;
        }
        os << "  ";
        /* put a separator */
    }
    os << "\n";

    return result;
}

std::string stackdump(char const* function, lua_State* l) {

    return stackdump_impl(l, "Function: ", function);
    /* end the listing */
}


std::string stackdump(lua_State* l)  {
    return stackdump_impl(l);
}


void stackdump_stderr(char const* function, lua_State* l)  {
    auto result = stackdump(function,l);
    std::cerr << result;
}

}

