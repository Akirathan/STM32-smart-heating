# Creates static library.
# TODO: Note that static library would not work because functions defined as weak
# in the static library could not be redefined elsewhere (weak functions
# are for example interrupt service routines from cube-mx project)

project(cube ASM C CXX)

set(BSP_SOURCES
	"BSP/STM3210C_EVAL/stm3210c_eval_io.c"
	"BSP/STM3210C_EVAL/stm3210c_eval.c"
	"BSP/STM3210C_EVAL/stm3210c_eval_lcd.c"
	"BSP/STM3210C_EVAL/stm3210c_eval_eeprom.c"
	"BSP/STM3210C_EVAL/stm3210c_eval_sd.c"
	"BSP/Components/ili9325/ili9325.c"
	"BSP/Components/ili9320/ili9320.c"
	"BSP/Components/stmpe811/stmpe811.c"
)

set(HAL_SOURCES
	"STM32F1xx_HAL_Driver/Src/stm32f1xx_hal.c"
	"STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio.c"
	"STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim.c"
	"STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim_ex.c"
	"STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc.c"
	"STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc_ex.c"
	"STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rtc.c"
	"STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rtc_ex.c"
	"STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_dma.c"
	"STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_spi.c"
	"STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_i2c.c"
	"STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_i2s.c"
	"STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_cortex.c"
	"STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_eth.c"
)

set(HW_SOURCES
	"hardware/startup_stm32f107xc.s"
	"hardware/stm32f1xx_hal_msp.c"
	"hardware/stm32f1xx_it.cpp"
	"hardware/syscalls.c"
	"hardware/system_stm32f1xx.c"
)

add_library(${PROJECT_NAME} OBJECT
	"${BSP_SOURCES}"
	"${HAL_SOURCES}"
	"${HW_SOURCES}"
)

# TODO: fix this include (neccessary for ISR)
include_directories("${MAIN_DIR}/impl")

target_include_directories(${PROJECT_NAME} PUBLIC
	"STM32F1xx_HAL_Driver/Inc"
	"BSP/STM3210C_EVAL"
	"BSP/Components/ili9325"
	"BSP/Components/ili9320"
	"Utilities/Fonts"
	"CMSIS/Device/ST/STM32F1xx/Include"
	"CMSIS/Include"
	"hardware"
)
