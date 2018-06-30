/**
 * @file sprintf_double.hpp
 * @author Pavel Marek
 * @date Jun 27, 2018
 */

#ifndef SPRINTF_DOUBLE_HPP_
#define SPRINTF_DOUBLE_HPP_

#include <cstdio> // For std::sprintf

/**
 * Sprintf for double with fixed 4 precision.
 * Note that this is necessary because there is no support for printing floats
 * in nano lib.
 * @param str ... this string will be filled with @p d converted to string.
 * @param d
 */
inline int sprintf_double(char str[], double d)
{
	const char *sign = (d < 0) ? "-" : "";
	d = (d < 0) ? -d : d;

	int int_part = static_cast<int>(d);
	double frac_part = d - int_part;
	int frac_part_mult = frac_part * 10000;
	return std::sprintf(str, "%s%d.%04d", sign, int_part, frac_part_mult);
}

#endif /* SPRINTF_DOUBLE_HPP_ */
