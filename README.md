# Overview
This project builds into firmware for [STM3210C Eval board](http://www.st.com/en/evaluation-tools/stm3210c-eval.html). It works as a simple heating - there is [DS18B20 temperature sensor](https://www.maximintegrated.com/en/products/analog/sensors-and-sensor-interface/DS18B20.html) that periodically measures temperature.

There is a possibility for the user to set certain temperature for a time interval, and currently measured temperature is compared to this preset temperature. Depending on result of that comparison, the relay that should be connected to a boiler is switched on or off.

Project is written in C++ and uses [HAL library](http://www.st.com/en/embedded-software/stm32cubef1.html).

# Requirements

## Hardware requirements
- [STM3210C Eval board](http://www.st.com/en/evaluation-tools/stm3210c-eval.html)
- [ST-LINK](http://www.st.com/en/development-tools/st-link-v2.html) as debugger
  - I use [STM32F4 Discovery Board]() for that purpose, because it contains st-link.
- [DS18B20 temperature sensor](https://www.maximintegrated.com/en/products/analog/sensors-and-sensor-interface/DS18B20.html)
- 5V relay

## Software requirements
- arm-none-eabi-gcc - Current version (6.3.1) is used.  
- openocd - Current version (0.9.0) is used.  
  - There is a config file stm3210c_eval.cfg for openocd. This file sets openocd to flash data through st-link debugger.
- HAL library included in [STM32 Cube F1](http://www.st.com/en/embedded-software/stm32cubef1.html).

For convenience there are .project and .cproject Eclipse CDT files included, there is also a launch file for Eclipse CDT (but you need ARM MCU plugin to use this launch file for debugging).

# Build

There is just one Makefile that builds all the necessary HAL sources along with sources from this repository and links them together. It is sufficient to adjust tool paths in Makefile and run debug or flash.

# Architecture

![](https://github.com/Akirathan/STM32-smart-heating/blob/master/arch_pic.png)

Directly above hardware layer there are classes or namespaces that controlls the underlying hardware either directly or through HAL. For consistency purposes use of HAL is discouraged on places with appropriate controller.
Frames and windows (described in GUI section) are in the top most layer. Besides `IO` and `LCD` controllers, windows can use other controllers - for example `StaticPresetTempWindow` uses `TempController` and `RTCController`.

### GUI
The only GUI elements are windows that are used to display a text information. They are divided into two categories: static and dynamic. Static windows just displays some information whereas dynamic windows can be focused (by pressing right/left joystick buttons) and the values they store can be changed (by pressing up/down joystick buttons). There is just one focused window at a time and its content is displayed in different (red) color.

Every dynamic window should inherit from `IControlWindow` and implement `eventHandler` method accordingly. The functionality of `eventHandler` is straightforward: control window returns `Message` telling whether the focuse should be put onto left or right neighbor window, or the whole frame should be exitted.

**Frames**
Frame stands for a display view eg. what is displayed in current time. It is basically just a container that holds windows and is responsible for their displaying. There is no restriction on controlling the windows, however ther is a prefered way - to initialize all the windows and pass them to `WindowSystem` that reads user input and cycles through the windows accordingly until `Message::EXIT` is returned from one of the windows. One can then simply investigate windows' values and end the whole frame. Note that this all is done in `passControl` method.


### (Second) interrupts
Class can get (second or minute) notifications (implemented as interrupts) if `ISecCallback` or `IMinCallback` interface is implemented. Implementation of one of those interfaces forces one to register the class for (second or minute) callbacks to `RTCController`.

# Possible improvements

There are many places where some error handler would be appropriate - for example when initializing RTC peripheral. Note that many of there places are annotated with TODOs.
