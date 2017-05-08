#pragma once

namespace templater {

struct writing_visitor;
struct reading_visitor;

//template <typename Class>
//void serialize_object(writing_visitor* visitor, Class const& obj) noexcept;
//template <typename Class>
//void deserialize_object(reading_visitor* visitor, Class& result) noexcept;
template <typename Class>
void serialize(writing_visitor* visitor, Class const& obj) noexcept;
template <typename Class>
void deserialize(reading_visitor* visitor, Class& result) noexcept;



}

