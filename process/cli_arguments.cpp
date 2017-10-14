#include "cli_arguments.hpp"
#include <utils/string/string.hpp>
#include <utils/string/build.hpp>
#include <assert.h>
#include <algorithm>
#include <iterator>

namespace utils {

constexpr unsigned initial_size = 20;

cli_arguments::cli_arguments(std::string const& command) {
    init(command.size() / 11);
    add(command);
}

cli_arguments::cli_arguments(int argc, char* argv[]) {
    init(argc);
    while (*argv) {
        add_single_argument(*argv);
        ++argv;
    }
}

void cli_arguments::init(int size) {
    buffer_.reserve(initial_size);
    args_.reserve(size);
}

cli_arguments::~cli_arguments() {}

std::string cli_arguments::to_string() const {
    std::string result;
    result.resize(buffer_.size());

    // creating original string
    std::transform(buffer_.begin(), buffer_.end(), result.begin(),
                   [](char c) { return (c != '\0' ? c : ' '); });
    result.back() = '\0';

    return result;
}

std::string cli_arguments::value(std::string const& key) const {

    enum { SEARCH = 1, PARSE_VALUE = 2 } state;

    state = SEARCH;
    for (auto arg : *this) {
        switch (state) {
        case SEARCH: {
            if (key.compare(arg) == 0) {
                state = PARSE_VALUE;
            }
        } break;
        case PARSE_VALUE: {
            return arg;
        } break;
        }
    }

    if (state == PARSE_VALUE) {
        throw std::runtime_error(build_string("Cannot find a value for key ", key,
                                              ". Key sited at the end of the argument sequence."));
    }

    return "";
}

bool cli_arguments::has_key(std::string const& argument) const {
    for (auto arg : *this) {
        if (argument.compare(arg) == 0)
            return true;
    }
    return false;
}

void cli_arguments::add(std::vector<std::string> values) {
    for (auto const& str : values)
        add_single_argument(str.data());
}

// space separated string
void cli_arguments::add(std::string const& values) { add(utils::split(values, ' ')); }

// add single argument at the end of the arguments
void cli_arguments::add_single_argument(char const* argument) {
    auto size = std::char_traits<char>::length(argument);
    //reserve(size + 1);

    // write new position value
    args_.push_back(buffer_.size());

    std::copy(argument, argument + size, std::back_inserter(buffer_));
    buffer_.push_back('\0');
}

template <typename CharType, typename ArgumentsType, typename BufferType>
std::vector<CharType> argv_impl(ArgumentsType& arguments, BufferType& buffer) {
    std::vector<CharType> result;
    result.reserve(arguments.size() + 1);

    std::transform(arguments.begin(), arguments.end(), std::back_inserter(result),
                   [&buffer](unsigned pos) { return buffer.data() + pos; });
    result.push_back(nullptr);

    return result;
}

char const* cli_arguments::operator[](unsigned ix) {
    assert(ix < args_.size());
    return args_[ix] + buffer_.data();
}

std::vector<char*> cli_arguments::argv()  { 
    return argv_impl<char*>(args_,buffer_);
}

std::vector<char const*> cli_arguments::argv() const {
    return argv_impl<char const*>(args_, buffer_);
}

int cli_arguments::argc() const { return args_.size(); }

cli_arguments::iterator cli_arguments::begin() const { return cli_arguments::iterator(*this, 0); }
cli_arguments::iterator cli_arguments::end() const {
    return cli_arguments::iterator(*this, args_.size());
}

cli_arguments::iterator::iterator(cli_arguments const& args, unsigned idx)
    : args_(args), idx_(idx) {}

// just postfix operators
cli_arguments::iterator& cli_arguments::iterator::operator++() {
    ++idx_;
    return *this;
}

cli_arguments::iterator& cli_arguments::iterator::operator--() {
    assert(idx_ != 0);
    --idx_;
    return *this;
}

bool cli_arguments::iterator::operator==(iterator const& other) {
    return (&args_ == &other.args_) && (idx_ == other.idx_);
}

bool cli_arguments::iterator::operator!=(iterator const& other) {
    return !(*this == other);
}

char const* cli_arguments::iterator::operator*() {
    return args_.args_[idx_] + args_.buffer_.data();
}
}

