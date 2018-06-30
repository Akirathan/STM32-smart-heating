/**
 * @file response_buffer_test.hpp
 * @author Pavel Marek
 * @date Jun 30, 2018
 */

#include "i_test.hpp"

#ifndef RESPONSE_BUFFER_TEST_HPP_
#define RESPONSE_BUFFER_TEST_HPP_

// ResponseBufferTest needs to be in namespace because it is declared as
// friend in ResponseBuffer.
namespace http {

class ResponseBufferTest : public ITest {
public:
	ResponseBufferTest();
	virtual void runAll() override;
	virtual bool passed() override;
private:
	bool pass;
};

} // namespace http

#endif /* RESPONSE_BUFFER_TEST_HPP_ */
