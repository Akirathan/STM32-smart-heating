/**
 * @file i_test.hpp
 * @author Pavel Marek
 * @date Jun 30, 2018
 */

#ifndef I_TEST_HPP_
#define I_TEST_HPP_

class ITest {
public:
	virtual void runAll() = 0;
	virtual bool passed() = 0;
};


#endif /* I_TEST_HPP_ */
