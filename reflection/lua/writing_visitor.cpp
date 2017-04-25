#include <assert.h>
#include "writing_visitor.hpp"
#include "helper.hpp"

extern "C"{
#include <lua.h>
#include <lauxlib.h>
}

//#define LUA_STACK(state) stackdump(__PRETTY_FUNCTION__, state);
#define LUA_STACK(state)

namespace lua {

// minimal instantiation of pimpl
writing_visitor::writing_visitor(lua_State* state) : inst_(state) { array_stack_.reserve(16); }
writing_visitor::~writing_visitor() {}

void writing_visitor::start_array() noexcept {
    LUA_STACK(inst_);
    array_stack_.push_back(0);
    lua_createtable(inst_, 0, 0);
}
void writing_visitor::end_array() noexcept {
    LUA_STACK(inst_);
    lua_rawseti(inst_, -2, array_stack_.back());
    array_stack_.pop_back();
}

void writing_visitor::prepare_element() noexcept { 
    LUA_STACK(inst_);
    // first of all need to serialize element then put it
    auto& counter = array_stack_.back();    
    if (counter != 0)
        lua_rawseti(inst_, -2, counter); 
    ++counter;
}
void writing_visitor::write_string(char const* value, size_t len) noexcept {
    LUA_STACK(inst_);
    lua_pushlstring(inst_, value, len);
}

void writing_visitor::write_integer(int value) noexcept {
    LUA_STACK(inst_);
    ::lua_pushinteger(inst_, value);
}
void writing_visitor::write_float(float value) noexcept {
    LUA_STACK(inst_);
    ::lua_pushnumber(inst_, value);
}
void writing_visitor::write_double(double value) noexcept {
    LUA_STACK(inst_);
    ::lua_pushnumber(inst_, value);
}
void writing_visitor::write_boolean(bool value) noexcept {
    LUA_STACK(inst_);
    ::lua_pushboolean(inst_, value);
}
void writing_visitor::write_none() noexcept {
    LUA_STACK(inst_);
    lua_pushnil(inst_);
}

// wite field of the object
void writing_visitor::write_name(char const* name, size_t len) noexcept {
    LUA_STACK(inst_);

    // logic with setting up previous object into table
    auto& counter = object_stack_.back();
    if (counter != 0)
        lua_settable(inst_,-3);
    ++counter;

    lua_pushlstring(inst_, name, len);
}

void writing_visitor::start_object() noexcept {
    LUA_STACK(inst_);
    object_stack_.push_back(0);
    lua_createtable(inst_, 0, 0 /*size must be here*/);
}

void writing_visitor::end_object() noexcept {
    LUA_STACK(inst_);
    if (object_stack_.back() != 0)
        lua_settable(inst_, -3);
    object_stack_.pop_back();
}
}
