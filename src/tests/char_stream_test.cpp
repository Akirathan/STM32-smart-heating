/**
 * @file char_stream_test.cpp
 * @author mayfa
 * @date Jun 30, 2018
 */

#include "char_stream_test.hpp"
#include "char_stream.hpp"

void CharStreamTest::runAll()
{
	const char *buffer = "Content-Length: 0\r\n\r\n";
	char words[2][20];
	size_t word_count = 0;

	CharStream char_stream(buffer, std::strlen(buffer));
	char_stream.readLine(words, &word_count, ':');
}

bool CharStreamTest::passed()
{
	return false;
}
