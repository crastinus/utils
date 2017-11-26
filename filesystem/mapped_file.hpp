#pragma once

// original https://github.com/d0k/mappedfile

#include "mappedfile.h"



#include <cstddef>
#ifdef __EXCEPTIONS
#include <sstream>
#include <stdexcept>
#else /* __EXCEPTIONS */
#include <cstdlib>
#include <iostream>
#endif /* __EXCEPTIONS */

namespace utils {
namespace fs {

/*!
 * mapped_file allows you to create a simple read-only file mapping in an
 * object-oriented cross-platform way.
 */
class mapped_file {
 private:
    std::size_t size_;
    char*       data_;
    mapped_file(const mapped_file&)
    : size_()
    , data_() {}
    mapped_file& operator=(const mapped_file&) { return *this; }

 public:
    /*!
     * Maps the named file into memory.
     * The file is mapped into memory. All filehandles are closed afterwards.
     * \param path path of the file being mapped
     * \exception IOException the file couldn't be opened
     */
    mapped_file(const char* path)
    : size_()
    , data_() {
        data_ = map_file(path, &size_);
        if (!data_) {
#ifdef __EXCEPTIONS
            std::ostringstream o;
            o << "Couldn't open File \"" << path << "\"";
            throw io_exception(o.str());
#else  /* __EXCEPTIONS */
            std::cerr << "Couldn't open File \"" << path << "\"" << std::endl;
            std::exit(EXIT_FAILURE);
#endif /* __EXCEPTIONS */
        }
    }
    /*!
     * Unmaps the file and releases all memory.
     */
    ~mapped_file() { unmap_file(data_, size_); }
    /*!
     * Get the size of the file in memory.
     */
    std::size_t length() const { return size_; }
    /*!
     * Gets the nth byte from the mapped file.
     */
    char operator[](std::size_t n) const { return data_[n]; }
    /*!
     * Gets a read-only pointer to the mapped data.
     */
    //const char* operator*() const { return data_; }
    const char* data() const { return data_; }
    char*       data() { return data_; }

    void close() { unmap_file(data_, size_);  }


#ifdef __EXCEPTIONS
    struct io_exception : public std::runtime_error {
        io_exception(const std::string& message)
        : std::runtime_error(message) {}
    };
#endif /* __EXCEPTIONS */
};
} // namespace filesystem
} // namespace utils
