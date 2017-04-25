#pragma once

#include <utils/meta/for_each_argument.hpp>
#include <utils/reflection/meta/templater.hpp>
#include <utils/reflection/lua/writing_visitor.hpp>
#include <utils/reflection/lua/reading_visitor.hpp>
#include <utils/reflection/execute.hpp>

extern "C" {
#include <lua.h>
#include <lauxlib.h>
}

namespace execute {
namespace lua {

    int put_function(lua_State* L, char const* name) noexcept;

    template <typename... Inputs>
    inline int execute_raw(lua_State* L, char const* function_name, Inputs&&... inputs) {

        int result;

        int start_top        = lua_gettop(L);
        int objects_in_stack = put_function(L, function_name);
        // if function return error use it as result
        if (objects_in_stack < 0)
            return objects_in_stack;

        // put objects onto stack
        utils::for_each_argument(
            [=](auto&& obj) {
                ::lua::writing_visitor visitor(L);
                templater::serialize_object(&visitor, obj);
            },
            std::forward<Inputs>(inputs)...);

        result = lua_pcall(L, sizeof...(Inputs), LUA_MULTRET, 0);
        if (result != 0) {
            return result;
            // result =  lua_tostring(L, -1);
        }

        result = lua_gettop(L) - start_top - objects_in_stack;

        return result;
    }

//    template<typename FunctionType, std::index_sequence<Is...>

    template <typename FunctionType, typename... Arguments, std::size_t... Is>
    inline void for_each_argument_result(FunctionType&& function,
                                         std::tuple<int, Arguments...>& result,
                                         std::index_sequence<Is...>) {
        using index_sequence = std::make_index_sequence<sizeof...(Arguments)>;
        using swallow = int[];
        (void)swallow{(function(std::get<Is+1>(result)), 0)...};
    }

    template <typename... Outputs, typename... Inputs>
    inline std::tuple<int, Outputs...> execute(lua_State* L, char const* function_name, Inputs&&... inputs) {

        std::tuple<int, Outputs...> result;

        // stack stk(L);
        int count_on_stack = execute_raw(L, function_name, std::forward<Inputs>(inputs)...);
        if (count_on_stack <= 0) {
            std::get<0>(result) = count_on_stack;
            return result;
        }

        // return count
        if (sizeof...(Outputs) != count_on_stack) {
            std::get<0>(result) = LUA_DIFFERENT_RETURN_VALUES_COUNT;
            return result;
        }

        int idx = lua_gettop(L) - sizeof...(Outputs) + 1;
        for_each_argument_result(
            [&](auto& output) {
                ::lua::reading_visitor visitor(L, idx++);
                templater::deserialize_object(&visitor, output);
            },
            result, std::index_sequence_for<Outputs...>{});

        return result;
    }
}
}
