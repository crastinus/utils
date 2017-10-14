#pragma once

#include <tuple>

namespace utils {

template <size_t N>
auto makeIndexSequence() {
    return std::make_index_sequence<N>();
}

// Because VS can't properly make index_sequence out of 1 element...
// Seems to be fixed in VS 2015 Update 3

template <>
inline auto makeIndexSequence<1>() {
    return std::index_sequence<0>();
}

template <typename F, typename Tuple, size_t... I>
void apply_tuple_impl(F&& f, Tuple&& t, std::index_sequence<I...>) {
	using swallow = int[];
	(void)swallow {
		(f(std::get<I>(std::forward<Tuple>(t))), 0)...
	};
}

template <typename F, typename Tuple>
void apply_tuple(F&& f, Tuple&& t) {
    constexpr size_t tupleSize = std::tuple_size<std::decay_t<Tuple>>::value;
    apply_tuple_impl(std::forward<F>(f), std::forward<Tuple>(t), makeIndexSequence<tupleSize>());
}


} // namespace utils