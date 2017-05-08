#pragma once
#include <ostream>
#include <iterator>

#include <utils/string/size_for_output.hpp>

namespace utils {

template <typename Iterator>
struct ostream_range {

    template <typename ValueType>
    using it_type = std::is_same<typename std::iterator_traits<Iterator>::value_type, ValueType>;

    ostream_range(Iterator begin, Iterator end) : begin_(begin), end_(end) {
        static_assert(it_type<char>::value || it_type<unsigned char>::value, "Type must be char");
    }
    ostream_range(ostream_range const& ) = default;
    ~ostream_range() {}

    Iterator begin_;
    Iterator end_;
};

template<typename Iterator>
std::ostream& operator << (std::ostream& os, ostream_range<Iterator> const& v){
    std::copy(v.begin_, v.end_, std::ostreambuf_iterator<char>(os));
    return os;
}

template<typename Iterator>
inline unsigned size_for_output(ostream_range<Iterator> const& rng) {
    return std::distance(rng.begin_, rng.end_) + 1;
}

template<typename Iterator>
inline ostream_range<Iterator> os_rng(Iterator begin, Iterator end){
    return ostream_range<Iterator>(begin, end);
}

}
