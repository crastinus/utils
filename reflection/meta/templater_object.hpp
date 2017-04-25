#pragma once

/*
 * Add this include for create definition of the function.
 */

#include "writing_visitor.hpp"
#include "reading_visitor.hpp"
#include <Meta.h>

namespace templater {

template <typename Class>
void serialize_object(writing_visitor* visitor, Class const& obj) noexcept {
    visitor->start_object();
    meta::doForAllMembers<Class>([&obj, visitor](auto& member) {

        // single way to get object
        assert(member.canGetConstRef());

        // TODO: Optimize this function call. Set members with size
        visitor->write_name(member.getName(),
                            std::char_traits<char>::length(member.getName()));
        serialize(visitor, member.get(obj));
    });
    visitor->end_object();
}

template <typename Class>
void deserialize_object(reading_visitor* visitor, Class& obj) noexcept {
    if (!visitor->prepare_object())
        return;

    meta::doForAllMembers<Class>([&obj, visitor](auto& member) {
        assert(member.canGetConstRef());

        if (!visitor->extract_member_by_name(member.getName()))
            return;

        deserialize(visitor, member.getRef(obj));

    });
    visitor->end_object();
}
}
