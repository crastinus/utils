
#include "rj_reading_visitor.hpp"
#include "rapidjson/document.h"
#include "iterator_stream.hpp"
#include <vector>
#include <string>
/*
 *
 * TODO: Make it work
 * I already figure out that every element reference must be input into array.
 * Use FindMember field for the named members. 
 *
 * Values must be input by functions prepare_name, extract_object and extract_element
 * prepare_name just put in stack value for a concrete name
 * extract_member extracts value for the current member_iterator (name can get from an iterator)
 * extract_element extracts value for the current value_iterator (unnamed) 
 */
namespace rj {

struct reading_visitor_impl {

    using allocator_t = rapidjson::MemoryPoolAllocator<>;
    using value_t     = rapidjson::GenericValue<rapidjson::UTF8<>, allocator_t>;
    using document_t  = rapidjson::GenericDocument<rapidjson::UTF8<>, allocator_t>;


    reading_visitor_impl();
    ~reading_visitor_impl();


    char const* read_string() noexcept;
    int read_integer() noexcept;
    float read_float() noexcept;
    double read_double() noexcept;
    bool read_boolean() noexcept;
    void null_value() noexcept; //must not be null value

    templater::cv_type current_value_type() noexcept;
    // prepared visitor to deserialize object and array
    // false if current object type has wrong type
    bool prepare_object() noexcept;
    void end_object() noexcept;
    bool extract_member() noexcept;
    bool extract_member_by_name(char const* name) noexcept;
    char const* member_name() noexcept;

    void end_element() noexcept;

    // prepare arrya for reading
    bool     prepare_array() noexcept;
    void     end_array() noexcept;
    unsigned array_size() noexcept;
    // false  array empty
    bool extract_element() noexcept;

    bool parse(char const* begin, char const* end) noexcept;
    void pop_value() noexcept;
    void push_value() noexcept;

    enum element_state {
        //       OBJECT_VALUE        = 1,
        //        ARRAY_VALUE         = 2,
        ARRAY_ELEMENT_VALUE = 3,
        NAMED_VALUE         = 4,
        ROOT_VALUE          = 5
    };

    allocator_t allocator_;
    document_t  document_;

    std::vector<value_t::MemberIterator> mem_it_stack_;
    std::vector<value_t::ValueIterator>  val_it_stack_;
    std::vector<value_t*>                values_stack_;
    std::vector<element_state> states_stack_;  // probably doesn't needed
    std::vector<char const*>   names_stack_;
    ;
};

reading_visitor_impl::reading_visitor_impl() {
    constexpr size_t reserved_elements_count = 16;
    names_stack_.reserve(reserved_elements_count);
    values_stack_.reserve(reserved_elements_count);
    states_stack_.reserve(reserved_elements_count);
    mem_it_stack_.reserve(reserved_elements_count);
    val_it_stack_.reserve(reserved_elements_count);
}

reading_visitor_impl::~reading_visitor_impl() {}

constexpr char const* default_string = "";
constexpr int default_integer        = 0;
constexpr float default_float        = 0;
constexpr double default_double      = 0;
constexpr bool default_boolean       = false;

//struct //operation_scope {
//    //operation_scope(reading_visitor_impl& instance) : instance_(instance) { }
//
//    ~//operation_scope(){
//        instance_.pop_value();
//    }
//
//    reading_visitor_impl& instance_;
//};

bool reading_visitor_impl::parse(char const* begin, char const* end) noexcept {

    rapidjson::iterator_stream<char const*> stream(begin,end);
    document_.ParseStream(stream);

    assert(values_stack_.empty() && states_stack_.empty());
    values_stack_.push_back(&document_);
    //states_stack_.push_back(ROOT_VALUE);

    return document_.HasParseError();
}

//function pops resgular values
//void reading_visitor_impl::pop_value() noexcept {
//    //values and names 
//    //doesn't have a value for an empty string
//    //if (names_stack_.back() != nullptr) 
//    //    values_stack_.pop_back();
//    names_stack_.pop_back();    
//    values_stack_.pop_back();
//}

void reading_visitor_impl::end_element() noexcept {
    names_stack_.pop_back();
    values_stack_.pop_back();
}

void reading_visitor_impl::push_value() noexcept {
}

bool reading_visitor_impl::extract_member_by_name(char const* name) noexcept {
    //not null previous node must be exists
    auto last_value = values_stack_.back();

    //last value must be present and have an object type
    assert(last_value != nullptr);
    assert(last_value->IsObject());

    if (!last_value->IsObject())
        return false;

    auto position  = last_value->FindMember(name);
    //if member doesn't present cannot make any actions with them
    if (position == last_value->MemberEnd())
        return false;

    names_stack_.push_back(name);
    values_stack_.push_back(&position->value);


    return true;
} 

char const* reading_visitor_impl::read_string() noexcept {
//    //operation_scope scope(*this);

    //extract value part
    value_t& value = (*values_stack_.back());
    if (value.IsString())
        return value.GetString();

    return default_string;
}

int reading_visitor_impl::read_integer() noexcept {

    //operation_scope scope(*this);

    //extract value part
    value_t& value = (*values_stack_.back());
    if (value.IsInt())
        return value.GetInt();

    return default_integer;
}

float reading_visitor_impl::read_float() noexcept {

    //operation_scope scope(*this);

    //extract value part
    value_t& value = (*values_stack_.back());
    if (value.IsFloat())
        return value.GetFloat();

    return default_float;
}

double reading_visitor_impl::read_double() noexcept {

    //operation_scope scope(*this);

    //extract value part
    value_t& value = (*values_stack_.back());
    if (value.IsDouble())
        return value.GetDouble();

    return default_double;
}
bool reading_visitor_impl::read_boolean() noexcept {

    //operation_scope scope(*this);

    //extract value part
    value_t& value = (*values_stack_.back());
    if (value.IsBool())
        return value.GetBool();

    return default_boolean;
}

templater::cv_type reading_visitor_impl::current_value_type() noexcept {
    // use value from back but not pop_back it
    value_t& value = (*values_stack_.back());

    if (value.IsString())
        return templater::cv_type::STRING;
    if (value.IsDouble())
        return templater::cv_type::DOUBLE;
    if (value.IsFloat())
        return templater::cv_type::FLOAT;
    if (value.IsInt())
        return templater::cv_type::INTEGER;
    if (value.IsBool())
        return templater::cv_type::BOOLEAN;
    if (value.IsArray())
        return templater::cv_type::ARRAY;
    if (value.IsObject()) 
        return templater::cv_type::OBJECT;

    return templater::cv_type::NONE;
}

// prepared visitor to deserialize object and array
// false if current object type has wrong type
bool reading_visitor_impl::prepare_object() noexcept {
    //must be present any element 
    assert(!values_stack_.empty());

    //init object
    bool is_object = values_stack_.back()->IsObject();
    if (is_object)
        mem_it_stack_.push_back(values_stack_.back()->MemberBegin());

    return is_object;
}

void reading_visitor_impl::end_object() noexcept {
//    mem_it_stack_.pop_back();
}

//member must be present in stack of values
//every extracting operation is making reference into 
bool reading_visitor_impl::extract_member() noexcept {

    auto memberEnd = values_stack_.back()->MemberEnd();
    auto& back = mem_it_stack_.back();
    if (back == memberEnd) {
        mem_it_stack_.pop_back();
        return false;
    }

    auto value = &back->value;
    auto key = &back->name;

    values_stack_.push_back(value);
    names_stack_.push_back(key->GetString());

    ++back;
    return true;
}

//member name contains into names stack
char const* reading_visitor_impl::member_name() noexcept {
    assert(!mem_it_stack_.empty());
    return names_stack_.back();// mem_it_stack_.back()->name.GetString();
}

// prepare arrya for reading
bool reading_visitor_impl::prepare_array() noexcept {
    assert(!values_stack_.empty());
    
    //everithing the same
    auto last_value = values_stack_.back();
    auto is_array   = last_value->IsArray();
    if (is_array)
       val_it_stack_.push_back(last_value->Begin());

    return is_array;
    
}

void reading_visitor_impl::end_array() noexcept {
    assert(!val_it_stack_.empty());
    val_it_stack_.pop_back(); //array iterator
}

unsigned reading_visitor_impl::array_size() noexcept {
    assert(values_stack_.back()->IsArray());
    return values_stack_.back()->Capacity();
}

bool reading_visitor_impl::extract_element() noexcept {

    assert(!val_it_stack_.empty());
    if (val_it_stack_.back() == values_stack_.back()->End())
        return false;

    values_stack_.push_back(&*val_it_stack_.back());
    ++val_it_stack_.back();
    names_stack_.push_back(nullptr);
    return true;
}


//original class
reading_visitor::reading_visitor() : impl_(nullptr) {
    impl_ = new reading_visitor_impl();
}

reading_visitor::~reading_visitor() {
    if (impl_ != nullptr)
        delete impl_;
}

template <typename BufferType>
bool reading_visitor::parse(BufferType const& buffer) {
    return impl_->parse(&*buffer.begin(), &*buffer.end());
}

template bool reading_visitor::parse<std::string>(std::string const& buffer);
template bool reading_visitor::parse<std::vector<char>>(std::vector<char> const& buffer);

bool reading_visitor::extract_member_by_name(char const* name) noexcept { return impl_->extract_member_by_name(name); }
char const* reading_visitor::read_string() noexcept { return impl_->read_string(); }
int reading_visitor::read_integer() noexcept { return impl_->read_integer(); }
float reading_visitor::read_float() noexcept { return impl_->read_float(); }
double reading_visitor::read_double() noexcept { return impl_->read_double(); }
bool reading_visitor::read_boolean() noexcept { return impl_->read_boolean(); }
void reading_visitor::null_value() noexcept { assert(false); }
bool reading_visitor::prepare_object() noexcept { return impl_->prepare_object(); }
//void reading_visitor::end_object() noexcept { return impl_->end_object(); }
bool reading_visitor::extract_member() noexcept { return impl_->extract_member(); }
char const* reading_visitor::member_name() noexcept { return impl_->member_name(); }
bool reading_visitor::prepare_array() noexcept { return impl_->prepare_array(); }
//void reading_visitor::end_array() noexcept { return impl_->end_array(); }
bool reading_visitor::extract_element() noexcept { return impl_->extract_element(); }
unsigned reading_visitor::array_size() noexcept { return impl_->array_size();}
void     reading_visitor::end_element() noexcept { impl_->end_element(); }

void reading_visitor::end_array() noexcept {}
void reading_visitor::end_object() noexcept {}

templater::cv_type reading_visitor::current_value_type() noexcept {
    return impl_->current_value_type();
}
}
