#pragma once

#include <string>
#include <vector>


namespace utils {
    // cli_arguments
struct cli_arguments {

    // not iterator at all.
    // class created for easiest reading
    struct iterator {

        iterator(cli_arguments const& args, unsigned idx = 0);

        // just postfix operators
        iterator& operator++();
        iterator& operator--();
        bool      operator==(iterator const& other);
        bool      operator!=(iterator const& other);
        char const* operator*();

        cli_arguments const& args_;
        unsigned    idx_;
    };

    // initialization from command
    cli_arguments(std::string const& command);
    // initialization from command line
    cli_arguments(int argc, char const* argv[]);
    cli_arguments(cli_arguments const&) = default;
    ~cli_arguments();

    void add(std::vector<std::string> values);
    // space separated string
    void add(std::string const& values);

    std::vector<char*> argv();
    std::vector<char const*> argv() const;

    std::string generate() const;

    iterator begin() const;
    iterator end() const;

  private:
    
    void init(int size);

    // rebase args if needed
    void rebase();
    void reserve(unsigned size);

    void write_null_symbol();

    void add_single_argument(char const* argument);

   // argv null-terminated array
    // size = argc + 1
    std::vector<unsigned>  args_;

    // buffer of the arguments 
    std::vector<char>   buffer_;
    
};
}
