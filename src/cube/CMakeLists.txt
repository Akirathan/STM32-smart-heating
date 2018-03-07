# Creates static library.
# TODO: Note that static library would not work because functions defined as weak
# in the static library could not be redefined elsewhere (weak functions
# are for example interrupt service routines from cube-mx project)

project(cube ASM C CXX)

set(CUBEMX_PATH "cube-mx")
set(HAL_PATH "STM32F1xx_HAL_Driver")
set(FATFS_PATH "Middlewares/Third_Party/fatfs")

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
	"${HAL_PATH}/Src/stm32f1xx_hal.c"
	"${HAL_PATH}/Src/stm32f1xx_hal_gpio.c"
	"${HAL_PATH}/Src/stm32f1xx_hal_tim.c"
	"${HAL_PATH}/Src/stm32f1xx_hal_tim_ex.c"
	"${HAL_PATH}/Src/stm32f1xx_hal_rcc.c"
	"${HAL_PATH}/Src/stm32f1xx_hal_rcc_ex.c"
	"${HAL_PATH}/Src/stm32f1xx_hal_rtc.c"
	"${HAL_PATH}/Src/stm32f1xx_hal_rtc_ex.c"
	"${HAL_PATH}/Src/stm32f1xx_hal_dma.c"
	"${HAL_PATH}/Src/stm32f1xx_hal_spi.c"
	"${HAL_PATH}/Src/stm32f1xx_hal_i2c.c"
	"${HAL_PATH}/Src/stm32f1xx_hal_i2s.c"
	"${HAL_PATH}/Src/stm32f1xx_hal_cortex.c"
	"${HAL_PATH}/Src/stm32f1xx_hal_eth.c"
	"${HAL_PATH}/Src/stm32f1xx_hal_pwr.c"
)

set(CUBEMX_SOURCES
	"${CUBEMX_PATH}/Src/main.c"
	"${CUBEMX_PATH}/Src/stm32f1xx_hal_msp.c"
	"${CUBEMX_PATH}/Src/stm32f1xx_it.c"
	"${CUBEMX_PATH}/Src/system_stm32f1xx.c"
	"${CUBEMX_PATH}/startup_stm32f107xc.s"
)

set(FATFS_SOURCES
	"${FATFS_PATH}/src/drivers/sd_diskio.c"
	"${FATFS_PATH}/src/diskio.c"
	"${FATFS_PATH}/src/ff.c"
	"${FATFS_PATH}/src/ff_gen_drv.c"
)

add_library(${PROJECT_NAME} OBJECT
	"${BSP_SOURCES}"
	"${HAL_SOURCES}"
	"${CUBEMX_SOURCES}"
	"${FATFS_SOURCES}"
)

# TODO: fix this include (neccessary for ISR)
include_directories("${MAIN_DIR}/impl")

target_include_directories(${PROJECT_NAME} PUBLIC
	"${HAL_PATH}/Inc"
	"BSP/STM3210C_EVAL"
	"BSP/Components/ili9325"
	"BSP/Components/ili9320"
	"Utilities/Fonts"
	"CMSIS/Device/ST/STM32F1xx/Include"
	"CMSIS/Include"
	"${CUBEMX_PATH}/Inc"
	"${FATFS_PATH}/src"
	"${FATFS_PATH}/src/drivers"
)
