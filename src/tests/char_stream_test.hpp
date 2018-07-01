/**
 * @file char_stream_test.hpp
 * @author Pavel Marek
 * @date Jun 30, 2018
 */

#include "i_test.hpp"

#ifndef CHAR_STREAM_TEST_HPP_
#define CHAR_STREAM_TEST_HPP_

class CharStreamTest : public ITest {
public:
	void runAll() override;
	bool passed() override;
};


#endif /* CHAR_STREAM_TEST_HPP_ */
