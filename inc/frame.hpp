/*
 * frame.hpp
 *
 *  Created on: Aug 25, 2017
 *      Author: mayfa
 */

#ifndef FRAME_HPP_
#define FRAME_HPP_

struct IFrame {
	virtual ~IFrame() = default;
	virtual void passControl() =0;
};

#endif /* FRAME_HPP_ */
