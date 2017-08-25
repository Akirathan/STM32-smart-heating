# Overview
This project builds into firmware for [STM3210C Eval board](http://www.st.com/en/evaluation-tools/stm3210c-eval.html).
It works as a simple heating - there is [DS18B20 temperature sensor]() that periodically measures temperature.


There is a possibility for the user to set certain temperature for a time interval, and measured temperature is
compared to this preset temperature. Depending on result of that comparison, the [Relay]()
that should be connected to a boiler is switched on or off.

Project is written in C++ and uses [HAL library](). There is single Makefile that is used to build and deploy the program.

# Requirements

## Hardware requirements
- [STM3210C Eval board](http://www.st.com/en/evaluation-tools/stm3210c-eval.html)
- [ST-LINK](http://www.st.com/en/development-tools/st-link-v2.html) as debugger
  - I use [STM32F4 Discovery Board]() for that purpose, because it contains st-link.

## Software requirements
**arm-none-eabi-gcc** - I use current version (6.3.1)  
**openocd** - current version (0.9.0)  
  There is a config file stm3210c_eval.cfg for openocd. This file sets openocd to flash data through st-link debugger.

You need to download [STM32 Cube](), the _HAL library_ is included in this package.

For convenience there are .project and .cproject Eclipse files included, there is also a launch file for Eclipse CDT (but you need ARM MCU plugin to use this launch file for debugging).

# Build

# Plans for the future
