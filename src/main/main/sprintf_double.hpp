/**
 * @file sprintf_double.hpp
 * @author Pavel Marek
 * @date Jun 27, 2018
 */

#ifndef SPRINTF_DOUBLE_HPP_
#define SPRINTF_DOUBLE_HPP_

#include <cstdio> // For std::sprintf
#include "rt_assert.h"

/**
 * Sprintf for double with fixed 4 precision.
 * Note that this is necessary because there is no support for printing floats
 * in nano lib.
 * @param str       ... this string will be filled with @p d converted to string.
 * @param d         ... value to convert.
 * @param precision ... precision of fraction part - max 5.
 */
inline int sprintf_double(char str[], double d, int precision)
{
	rt_assert(precision > 0 && precision <= 5, "Wrong precision");

	const char *sign = (d < 0) ? "-" : "";
	d = (d < 0) ? -d : d;

	int int_part = static_cast<int>(d);
	double frac_part = d - int_part;
	double frac_part_mult = frac_part;

	for (int i = 0; i < precision; i++) {
		frac_part_mult *= 10;
	}

	return std::sprintf(str, "%s%d.%d", sign, int_part, static_cast<int>(frac_part_mult));
}

#endif /* SPRINTF_DOUBLE_HPP_ */
