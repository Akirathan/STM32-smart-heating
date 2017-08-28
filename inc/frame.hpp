/**
 * @file frame.hpp
 * @author Pavel Marek
 * @date 25.8.2017
 */

#ifndef FRAME_HPP_
#define FRAME_HPP_

/**
 * @brief Base class for every frame in the application.
 */
struct IFrame {
	virtual ~IFrame() = default;
	virtual void passControl() =0;
};

#endif /* FRAME_HPP_ */
