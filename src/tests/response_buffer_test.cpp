/**
 * @file response_buffer_test.cpp
 * @author Pavel Marek
 * @date Jun 30, 2018
 */

#include "response_buffer_test.hpp"
#include "response_buffer.hpp"

namespace http {

ResponseBufferTest::ResponseBufferTest() :
	pass(true)
{ }

void ResponseBufferTest::runAll()
{
	const char *buffer = "HTTP/1.1 200 OK\r\nContent-Length:0\r\n\r\n";
	Response response;

	pass = ResponseBuffer::parse(buffer, std::strlen(buffer), &response);
}

bool ResponseBufferTest::passed()
{
	return pass;
}

} // namespace http
