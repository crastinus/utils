#include "rj_writing_visitor.hpp"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/prettywriter.h"
#include "std_stream.hpp"
#include <vector>
#include <assert.h>


namespace rj {

struct writing_visitor_impl {
    using allocator_t = rapidjson::MemoryPoolAllocator<>;
    using value_t     = rapidjson::GenericValue<rapidjson::UTF8<>, allocator_t>;

    writing_visitor_impl();
    virtual ~writing_visitor_impl();

    void write_name(char const* name, size_t len) noexcept;

    void start_array() noexcept;
    void end_array() noexcept;
    void prepare_element() noexcept;

    void write_string(char const* value, size_t len) noexcept;
    void write_integer(int value) noexcept;
    void write_float(float value) noexcept;
    void write_double(double value) noexcept;
    void write_boolean(bool value) noexcept;
    void write_none() noexcept;

    void start_object() noexcept;
    void end_object() noexcept;



    allocator_t allocator_;
    value_t result_;

    /*
     * Algorithm must be choose between array element and value with name.
     * This design caused by artificial stack implementation in this class
     */
    enum element_state {
        //       OBJECT_VALUE        = 1,
        //        ARRAY_VALUE         = 2,
        ARRAY_ELEMENT_VALUE = 3,
        NAMED_VALUE         = 4,
        ROOT_VALUE          = 5
    };
    void pop_value() noexcept;
    void push_value() noexcept;

    using string_t = std::pair<char const*, size_t>;

    std::vector<element_state> states_stack_;
    std::vector<string_t> names_stack_;
    std::vector<value_t> values_stack_;

    bool is_array_element_;
};


/*
 * Creates an object
 */
inline void writing_visitor_impl::pop_value() noexcept {
    value_t v           = std::move(values_stack_.back());
    element_state state = states_stack_.back();
    values_stack_.pop_back();
    states_stack_.pop_back();

    if (state == NAMED_VALUE) {
        auto name    = names_stack_.back();
        names_stack_.pop_back();

        assert(values_stack_.back().IsObject());
        values_stack_.back().AddMember(rapidjson::StringRef(name.first, name.second), v,
                                       allocator_);

    } else if (state == ARRAY_ELEMENT_VALUE) { // ARRAY_ELEMENT_VALUE
        values_stack_.back().PushBack(v, allocator_);
    } else if (state == ROOT_VALUE) { // ROOT_VALUE
        result_ = std::move(v);
    } else {
        assert(false);
    }
}

/*
 * chose what type of element put in the stack
 */
inline void writing_visitor_impl::push_value() noexcept {
    // element that starts with
    if (is_array_element_) {
        states_stack_.push_back(ARRAY_ELEMENT_VALUE);
        is_array_element_ = false;
    } else if (!states_stack_.empty())
        // root value (opened { without a name)
        states_stack_.push_back(NAMED_VALUE);
    else
        //most popular
        states_stack_.push_back(ROOT_VALUE);
}

writing_visitor_impl::writing_visitor_impl() : is_array_element_(false) {
    constexpr size_t reserved_elements_count = 16;
    names_stack_.reserve(reserved_elements_count);
    values_stack_.reserve(reserved_elements_count);
    states_stack_.reserve(reserved_elements_count);
}
writing_visitor_impl::~writing_visitor_impl() {}

void writing_visitor_impl::write_name(char const* name, size_t len) noexcept {
    names_stack_.emplace_back(name, len);
}

void writing_visitor_impl::start_array() noexcept {
    push_value();
    values_stack_.emplace_back(rapidjson::kArrayType);
}
void writing_visitor_impl::end_array() noexcept {
    pop_value();
}

void writing_visitor_impl::prepare_element() noexcept {
    is_array_element_ = true;
}

void writing_visitor_impl::write_string(char const* value, size_t len) noexcept {
    push_value();
    values_stack_.emplace_back(value, len);
    pop_value();
}
void writing_visitor_impl::write_integer(int value) noexcept {
    push_value();
    values_stack_.emplace_back(value);
    pop_value();
}
void writing_visitor_impl::write_float(float value) noexcept {
    push_value();
    values_stack_.emplace_back(value);
    pop_value();
}
void writing_visitor_impl::write_double(double value) noexcept {
    push_value();
    values_stack_.emplace_back(value);
    pop_value();
}
void writing_visitor_impl::write_boolean(bool value) noexcept {
    push_value();
    values_stack_.emplace_back(value);
    pop_value();
}
void writing_visitor_impl::write_none() noexcept {
    push_value();
    values_stack_.emplace_back(rapidjson::kNullType);
    pop_value();
}
void writing_visitor_impl::start_object() noexcept {
    push_value();
    values_stack_.emplace_back(rapidjson::kObjectType);
}
void writing_visitor_impl::end_object() noexcept {
    pop_value();
}


//minimal instantiation of pimpl
writing_visitor::writing_visitor() : impl_(nullptr) {
    impl_ = new writing_visitor_impl();
}
writing_visitor::~writing_visitor() {
    if (impl_ != nullptr)
        delete impl_;
}

void writing_visitor::start_array() noexcept { impl_->start_array(); }
void writing_visitor::end_array() noexcept { impl_->end_array(); }
void writing_visitor::prepare_element() noexcept { impl_->prepare_element(); }

void writing_visitor::write_name(char const* name, size_t len) noexcept {
    impl_->write_name(name, len);
}
void writing_visitor::write_string(char const* value, size_t len) noexcept {
    impl_->write_string(value, len);
}
void writing_visitor::write_integer(int value) noexcept { impl_->write_integer(value); }
void writing_visitor::write_float(float value) noexcept { impl_->write_float(value); }
void writing_visitor::write_double(double value) noexcept { impl_->write_double(value); }
void writing_visitor::write_boolean(bool value) noexcept { impl_->write_boolean(value); }
void writing_visitor::write_none() noexcept { impl_->write_none(); }

void writing_visitor::start_object() noexcept { impl_->start_object(); }
void writing_visitor::end_object() noexcept { impl_->end_object(); }


template <typename BufferType>
BufferType writing_visitor::serialize() const {

    using stream_t = rapidjson::std_stream<BufferType>;
    using writer_t = rapidjson::Writer<stream_t>;

    BufferType result;
    result.reserve(1024);

    stream_t stream(result);
    writer_t writer(stream);

    impl_->result_.Accept(writer);

    return result;

}

template <typename BufferType>
BufferType writing_visitor::pretty_serialize() const {

    using stream_t = rapidjson::std_stream<BufferType>;
    using writer_t = rapidjson::PrettyWriter<stream_t>;

    BufferType result;
    result.reserve(2048);

    stream_t stream(result);
    writer_t writer(stream);

    impl_->result_.Accept(writer);

    return result;
}


template std::string writing_visitor::serialize<std::string>() const;
template std::string writing_visitor::pretty_serialize<std::string>() const;

using vector_t = std::vector<char>;
template vector_t writing_visitor::serialize<vector_t>() const;
template vector_t writing_visitor::pretty_serialize<vector_t>() const;


}
