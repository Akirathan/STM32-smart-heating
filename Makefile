# Project (adjust to match your needs)
PROJECT = generic_eval
ELF = $(PROJECT).elf

# Library paths (adjust to match your needs)
STM32F1CUBE = /home/mayfa/Dev/STM/STM32Cube_FW_F1_V1.3.0
CMSIS = $(STM32F1CUBE)/Drivers/CMSIS
BSP = $(STM32F1CUBE)/Drivers/BSP
HAL = $(STM32F1CUBE)/Drivers/STM32F1xx_HAL_Driver
UTILITIES = $(STM32F1CUBE)/Utilities

# Toolchain paths (adjust to match your needs)
TOOLCHAIN_PREFIX:=arm-none-eabi-
CC = $(TOOLCHAIN_PREFIX)gcc
CXX = $(TOOLCHAIN_PREFIX)g++
LD = $(TOOLCHAIN_PREFIX)ld
OBJCOPY = $(TOOLCHAIN_PREFIX)objcopy
SIZE = $(TOOLCHAIN_PREFIX)size
GDB = $(TOOLCHAIN_PREFIX)gdb-py
AS = $(TOOLCHAIN_PREFIX)as
OPENOCD = openocd
OPENOCD_SCRIPTS = /usr/share/openocd/scripts

# Compiler and linker options
CFLAGS = -mcpu=cortex-m3 -mthumb -mfloat-abi=soft
CFLAGS += -specs=nano.specs
CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += -DSTM32F107VCTx -DUSE_HAL_DRIVER -DUSE_STM3210C_EVAL -DSTM3210C_EVAL
CFLAGS += -g -Wall 
#CFLAGS += -Os

CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-threadsafe-statics -fno-rtti
CXXFLAGS += -std=c++14 

LDFLAGS = -mcpu=cortex-m3 -mthumb -mfloat-abi=soft 
LDFLAGS += -Wl,-Map=output.map -Wl,--gc-sections -fno-exceptions
LDFLAGS += -Wl,--start-group -lc -lm -Wl,--end-group
LDFLAGS += -specs=nano.specs 
#LDFLAGS += -specs=nosys.specs
LDFLAGS += -static
LDFLAGS += -T"./STM32F107XC_FLASH.ld"
LDFLAGS += -Wl,-u,Reset_Handler -Wl,--defsym=malloc_getpagesize_P=0x1000 

# Includes including library includes
INCLUDES=\
-I./inc \
-I./hardware \
-I./tests \
-I$(HAL)/Inc \
-I$(BSP)/STM3210C_EVAL \
-I$(BSP)/Components/ili9325 \
-I$(BSP)/Components/ili9320 \
-I$(UTILITIES)/Fonts \
-I$(CMSIS)/Device/ST/STM32F1xx/Include \
-I$(CMSIS)/Include

# Application objects
APP_OBJECTS += hardware/stm32f1xx_it.o 
APP_OBJECTS += hardware/system_stm32f1xx.o
APP_OBJECTS += hardware/startup_stm32f107xc.o
APP_OBJECTS += hardware/syscalls.o

APP_OBJECTS += src/window.o
APP_OBJECTS += src/rtc_controller.o
APP_OBJECTS += src/time_window.o
APP_OBJECTS += src/static_time_window.o
APP_OBJECTS += src/temp_window.o
APP_OBJECTS += src/static_temp_window.o
APP_OBJECTS += src/static_measure_temp_window.o
APP_OBJECTS += src/static_preset_temp_window.o
APP_OBJECTS += src/button.o

APP_OBJECTS += src/lcd.o
APP_OBJECTS += src/window_system.o
APP_OBJECTS += src/clk_frame.o
APP_OBJECTS += src/main_frame.o
APP_OBJECTS += src/interval_frame_data.o
APP_OBJECTS += src/interval_frame.o

APP_OBJECTS += src/eeprom.o
APP_OBJECTS += src/input.o
APP_OBJECTS += src/temp_sensor.o
APP_OBJECTS += src/main.o 
APP_OBJECTS += src/one_wire.o
APP_OBJECTS += src/time.o
APP_OBJECTS += src/temp_controller.o
APP_OBJECTS += src/relay.o

# Test objects
TESTS_OBJECTS += tests/temp_sensor_tests.o

# Currenly used HAL module objects
HAL_OBJECTS=\
$(HAL)/Src/stm32f1xx_hal.o \
$(HAL)/Src/stm32f1xx_hal_gpio.o \
$(HAL)/Src/stm32f1xx_hal_tim.o \
$(HAL)/Src/stm32f1xx_hal_tim_ex.o \
$(HAL)/Src/stm32f1xx_hal_rcc.o \
$(HAL)/Src/stm32f1xx_hal_rcc_ex.o \
$(HAL)/Src/stm32f1xx_hal_rtc.o \
$(HAL)/Src/stm32f1xx_hal_rtc_ex.o \
$(HAL)/Src/stm32f1xx_hal_dma.o \
$(HAL)/Src/stm32f1xx_hal_spi.o \
$(HAL)/Src/stm32f1xx_hal_i2c.o \
$(HAL)/Src/stm32f1xx_hal_i2s.o \
$(HAL)/Src/stm32f1xx_hal_cortex.o 

# Available HAL module objects
HAL_OBJECTS_EXTRA=\
$(HAL)/Src/stm32f1xx_hal_wwdg.o \
$(HAL)/Src/stm32f1xx_ll_fsmc.o \
$(HAL)/Src/stm32f1xx_ll_sdmmc.o \
$(HAL)/Src/stm32f1xx_ll_usb.o \
$(HAL)/Src/stm32f1xx_hal_hcd.o \
$(HAL)/Src/stm32f1xx_hal_irda.o \
$(HAL)/Src/stm32f1xx_hal_iwdg.o \
$(HAL)/Src/stm32f1xx_hal_nand.o \
$(HAL)/Src/stm32f1xx_hal_nor.o \
$(HAL)/Src/stm32f1xx_hal_pccard.o \
$(HAL)/Src/stm32f1xx_hal_pcd.o \
$(HAL)/Src/stm32f1xx_hal_pcd_ex.o \
$(HAL)/Src/stm32f1xx_hal_pwr.o \
$(HAL)/Src/stm32f1xx_hal_smartcard.o \
$(HAL)/Src/stm32f1xx_hal_sram.o \
$(HAL)/Src/stm32f1xx_hal_adc.o \
$(HAL)/Src/stm32f1xx_hal_adc_ex.o \
$(HAL)/Src/stm32f1xx_hal_can.o \
$(HAL)/Src/stm32f1xx_hal_cec.o \
$(HAL)/Src/stm32f1xx_hal_crc.o \
$(HAL)/Src/stm32f1xx_hal_dac.o \
$(HAL)/Src/stm32f1xx_hal_dac_ex.o \
$(HAL)/Src/stm32f1xx_hal_eth.o \
$(HAL)/Src/stm32f1xx_hal_flash.o \
$(HAL)/Src/stm32f1xx_hal_flash_ex.o \
$(HAL)/Src/stm32f1xx_hal_uart.o \
$(HAL)/Src/stm32f1xx_hal_usart.o

BSP_OBJECTS = \
$(BSP)/STM3210C_EVAL/stm3210c_eval_io.o \
$(BSP)/STM3210C_EVAL/stm3210c_eval.o \
$(BSP)/STM3210C_EVAL/stm3210c_eval_lcd.o \
$(BSP)/STM3210C_EVAL/stm3210c_eval_eeprom.o \
$(BSP)/Components/ili9325/ili9325.o \
$(BSP)/Components/ili9320/ili9320.o \
$(BSP)/Components/stmpe811/stmpe811.o

OBJECTS = $(HAL_OBJECTS) $(BSP_OBJECTS) $(APP_OBJECTS) $(TESTS_OBJECTS)

DEPENDENCIES=$(OBJECTS:.o=.d)


all: $(ELF)

clean:
	rm -f $(ELF)
	rm -f **/*.o
	rm -f **/*.d
	rm -f $(HAL_OBJECTS)
	rm -f $(BSP_OBJECTS)

# Link final elf
$(ELF): $(OBJECTS)
	@echo "Invoking g++ linker"
	$(CXX) $(LDFLAGS) $^ -o $@
	@echo "Done linking $<"
	@echo "\n"

# Compile C++ source into object
%.o: %.cpp
	@echo "Invoking g++ compiler on $<"
	$(CXX) -c $(CXXFLAGS) $(INCLUDES) $< -o $@
	@echo "Done compiling $<"
	@echo "\n"

# Compile C source into object
%.o: %.c
	@echo "Invoking gcc on $<"
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@
	@echo "Done compiling $<"
	@echo "\n"

# Compile assembler source into object
%.o: %.s
	@echo "Invoking as compiler on $<"
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@
	@echo "Done compiling $<"
	@echo "\n"

# Flash final elf into device
flash: $(ELF)
	${OPENOCD} -s ${OPENOCD_SCRIPTS} -f ./stm3210c_eval.cfg -c "program $< verify reset exit"

# Debug
debug: $(ELF)
	$(GDB) $(ELF) -x gdb_cmds.txt

-include $(DEPENDENCIES)

.PHONY: all flash clean debug
