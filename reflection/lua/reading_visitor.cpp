
#include "reading_visitor.hpp"
//#include "rapidjson/document.h"
#include <vector>
#include <string>

extern "C" {
#include <lua.h>
#include <lauxlib.h>
}

#include <assert.h>
#include "helper.hpp"

//#define LUA_STACK(state) stackdump_stderr(__PRETTY_FUNCTION__, state)
#define LUA_STACK(state)

namespace lua {
reading_visitor::reading_visitor(lua_State* s, int stack_pos)
    : inst_(s), lua_stack_pos_(stack_pos) {
    if (lua_stack_pos_ != -1)
        lua_pushvalue(inst_, lua_stack_pos_);
    array_stack_.reserve(10);
}
reading_visitor::~reading_visitor() {
    if (lua_stack_pos_ != -1)
        lua_pop(inst_, 1);
}

bool reading_visitor::extract_member_by_name(char const* name) noexcept {
    LUA_STACK(inst_);
    int type = ::lua_getfield(inst_, -1, name);
    bool extract =  (type != LUA_TNIL);
    if (!extract)
        end_element();
    return extract;
}

char const* reading_visitor::read_string() noexcept {
    LUA_STACK(inst_);
    // if (!::lua_isstring(inst_, -1))
    //    return "";
    auto result = ::lua_tostring(inst_, -1);
    return (result == nullptr ? "" : result);
}

inline LUA_NUMBER read_number(lua_State* s) {
    // TODO: make a way to parse number from string as in previous implementation in qlua
    //       but make in fastiest way without spirit
    //            auto type = lua_type(lua_state_, -1);
    //            switch (type) {
    //                case LUA_TNUMBER:
    //                    result = lua_tonumber(lua_state_, -1);
    //                    break;
    //                case LUA_TSTRING:
    //                    result = spirit::number<NumberType>(lua_tostring(lua_state_, -1));
    //                    break;
    //                case LUA_TBOOLEAN:
    //                    result = lua_toboolean(lua_state_, -1);
    //                    break;
    //                default:
    //                    result = {};
    //                    break;
    //            }
    return lua_tonumber(s, -1);
}

int reading_visitor::read_integer() noexcept {
    LUA_STACK(inst_);
    return static_cast<int>(read_number(inst_));
}
float reading_visitor::read_float() noexcept {
    LUA_STACK(inst_);
    return static_cast<float>(read_number(inst_));
}
double reading_visitor::read_double() noexcept {
    LUA_STACK(inst_);
    return static_cast<double>(read_number(inst_));
}
bool reading_visitor::read_boolean() noexcept {
    LUA_STACK(inst_);
    return ::lua_toboolean(inst_, -1);
}

void reading_visitor::null_value() noexcept { assert(false); }

bool reading_visitor::prepare_object() noexcept {
    LUA_STACK(inst_);
    return lua_istable(inst_, -1) && (luaL_len(inst_, -1) == 0);
}
void reading_visitor::end_object() noexcept { LUA_STACK(inst_); }

bool reading_visitor::extract_member() noexcept {
    LUA_STACK(inst_);
    return lua_next(inst_, -2) != 0;
}

char const* reading_visitor::member_name() noexcept {
    LUA_STACK(inst_);
    // in this place must be a name. It will be error if none
    assert(::lua_isstring(inst_, -2));
    return lua_tostring(inst_, -2);
}

bool reading_visitor::prepare_array() noexcept {
    LUA_STACK(inst_);
    if (!lua_istable(inst_, -1)) {
        lua_pushnil(inst_);
        return false;
    }
    int array_size = luaL_len(inst_, -1);
    if (array_size == 0) {
        lua_pushnil(inst_);
        return false;
    }
    // array_index_ = 0;
    // array_size_ = luaL_len(inst_,-1);
    array_stack_.push_back({0, array_size});
    return true;
}

void reading_visitor::end_array() noexcept {
    LUA_STACK(inst_);
    assert(array_stack_.size() != 0);
    array_stack_.pop_back();
}

// void reading_visitor::end_array() noexcept { return impl_->end_array(); }
bool reading_visitor::extract_element() noexcept {
    LUA_STACK(inst_);
    auto& current = array_stack_.back();
    if (current.array_index_ == current.array_size_)
        return false;
    lua_rawgeti(inst_, -1, ++current.array_index_);
    return true;
}

unsigned reading_visitor::array_size() noexcept {
    LUA_STACK(inst_);
    return luaL_len(inst_, -1);
}
void reading_visitor::end_element() noexcept {
    LUA_STACK(inst_);
    lua_pop(inst_, 1);
}

templater::cv_type reading_visitor::current_value_type() noexcept {
    LUA_STACK(inst_);
    auto t = ::lua_type(inst_, -1);
    switch (t) {
        case LUA_TSTRING:
            return templater::cv_type::STRING;
        case LUA_TNUMBER:
            return templater::cv_type::DOUBLE;
        case LUA_TBOOLEAN:
            return templater::cv_type::BOOLEAN;
        case LUA_TNIL:
            return templater::cv_type::NONE;
        case LUA_TTABLE:
            return (luaL_len(inst_, -1) == 0 ? templater::cv_type::OBJECT
                                             : templater::cv_type::ARRAY);
        default:
            return templater::cv_type::NONE;
    }
}
}
