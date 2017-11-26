#include "bcd.hpp"
#include <assert.h>

bcd bcd::parse(char const* buffer, size_t length){ return bcd{}; }

bcd bcd::parse(char const* buffer) {
	bcd result{};

	auto pointer = buffer;
	if (*pointer == '\0')
		return result;

	if (*pointer == '-') {
		result.sign_ = 1;
		++pointer;
	}

	return bcd{};
}

bcd bcd::parse(std::string const & src)
{
	return parse(src.data(), src.size());
}

bcd operator- (bcd const& lhs, bcd const& rhs) {
	assert(rhs.power_ == lhs.power_);

	bcd newRhs = rhs;
	newRhs.sign_ = (rhs.sign_ == 1 ? 0 : 1);
	return lhs + newRhs;
}

bcd operator+ (bcd const& lhs, bcd const& rhs) {
    assert (rhs.power_ == lhs.power_);

    bcd result{};

    if (lhs.value_ == rhs.value_) {
        return result;
    }

    result.power_ = lhs.power_;
    if (lhs.sign_ != rhs.sign_) {

        if (lhs.value_ > rhs.value_) {
            result.value_ = lhs.value_ - rhs.value_;
            result.sign_  = lhs.sign_;
        } else {
            result.value_ = rhs.value_ - lhs.value_;
            result.sign_  = rhs.sign_;
        }

    } else {
        result.sign_  = lhs.sign_;
        result.value_ = lhs.value_ + rhs.value_;
    }

    return result;
}

bcd operator*(bcd const& lhs, unsigned multiplier) {
	bcd result;
	result.value_ *= multiplier;
	return result;
}
