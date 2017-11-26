#pragma once
#include <stdint.h>
#include <string>
#include <iosfwd>

struct bcd {
	uint64_t	sign_ : 1;
	uint64_t	power_ : 7;
	uint64_t	value_ : 56;

	static bcd parse(char const* buffer, size_t length);
	static bcd parse(char const* buffer);
	static bcd parse(std::string const& src);
	
	std::string	to_string() const;
	float	to_float() const;
	double	to_double() const;
};

//// every operation must be lightweight. No multiply and divide operation must be present
//bcd operator*(bcd const& lhs, bcd const& rhs);
//bcd operator/(bcd const& lhs, bcd const& rhs);
bcd operator-(bcd const& lhs, bcd const& rhs);
bcd operator+(bcd const& lhs, bcd const& rhs);
bcd operator*(bcd const& lhs, unsigned multiplier);


std::ostream& operator << (std::ostream& os, bcd const& printable);