#pragma once

#include <string>
#include <vector>


namespace utils {
    // cli_arguments
struct cli_arguments {

    // not iterator at all.
    // class created for easiest reading
    struct iterator {

        iterator(cli_arguments const& args, size_t idx = 0);

        // just postfix operators
        iterator& operator++();
        iterator& operator--();
        bool      operator==(iterator const& other);
        bool      operator!=(iterator const& other);
        char const* operator*();

        cli_arguments const& args_;
        size_t               idx_;
    };

    // initialization from command
    cli_arguments(std::string const& command);
    // initialization from command line
    cli_arguments(int argc, char* argv[]);
    cli_arguments(cli_arguments const&) = default;
    cli_arguments(cli_arguments&&)      = default;
    ~cli_arguments();


    // not use any map for an arguments
    // -f and --file for example allways will be different
    std::string value(std::string const& argument) const;
    bool has_key(std::string const& argument) const;

    void add(std::vector<std::string> values);
    // space separated string
    void add(std::string const& values);

    std::vector<char*> argv();
    std::vector<char const*> argv() const;

    size_t argc() const;

    char const* operator[](unsigned ix);

    std::string to_string() const;

    iterator begin() const;
    iterator end() const;


  private:
    
    void init(size_t size);

    void add_single_argument(char const* argument);

   // argv null-terminated array
    // size = argc + 1
    std::vector<size_t>  args_;

    // buffer of the arguments 
    std::vector<char>   buffer_;
    
};
}
