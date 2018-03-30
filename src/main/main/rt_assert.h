/**
 * @file rt_assert.h
 * @author Pavel Marek
 * @date Mar 29, 2018
 *
 * @brief This file contains definition for run-time assert.
 */

#ifndef RT_ASSERT_H_
#define RT_ASSERT_H_

#include <stdio.h>

#define rt_assert(condition, comment) \
{ \
	if ( !(condition) ) {\
		printf("%s:%d:%s\n", __FILE__, __LINE__, comment); \
		for (;;); \
	}\
}

#endif /* RT_ASSERT_H_ */
