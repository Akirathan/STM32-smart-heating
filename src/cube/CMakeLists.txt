# Creates static library.
# TODO: Note that static library would not work because functions defined as weak
# in the static library could not be redefined elsewhere (weak functions
# are for example interrupt service routines from cube-mx project)

project(cube ASM C CXX)

message("Running cube cmake, USE_CUBE_MX=${USE_CUBE_MX}")

set(CUBEMX_PATH "cube-mx")
set(HW_PATH "hardware")
set(HAL_PATH "STM32F1xx_HAL_Driver")
set(FATFS_PATH "Middlewares/Third_Party/fatfs")
set(LWIP_PATH "Middlewares/Third_Party/LwIP")
set(CRYPTO_PATH "Middlewares/ST/STM32_Cryptographic")

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
	"${HAL_PATH}/Src/stm32f1xx_hal_crc.c"
)

if(USE_CUBE_MX)
	set(CUBEMX_SOURCES
		"${CUBEMX_PATH}/startup_stm32f107xc.s"
		"${CUBEMX_PATH}/Src/system_stm32f1xx.c"
		"${CUBEMX_PATH}/Src/stm32f1xx_it.c"
		"${CUBEMX_PATH}/Src/stm32f1xx_hal_msp.c"
		"${CUBEMX_PATH}/Src/main.c"
	)
	
	set_source_files_properties(
		"${CUBEMX_PATH}/Src/main.c"
		PROPERTIES COMPILE_FLAGS
		"-Wno-return-type"
	)
else()
	set(HW_SOURCES
		"${HW_PATH}/startup_stm32f107xc.s"
		"${HW_PATH}/system_stm32f1xx.c"
		"${HW_PATH}/stm32f1xx_it.cpp"
		"${HW_PATH}/stm32f1xx_hal_msp.c"
	)
endif()

set(FATFS_SOURCES
	"${FATFS_PATH}/src/drivers/sd_diskio.c"
	"${FATFS_PATH}/src/diskio.c"
	"${FATFS_PATH}/src/ff.c"
	"${FATFS_PATH}/src/ff_gen_drv.c"
)

set(LWIP_SOURCES
	"${LWIP_PATH}/src/netif/ethernetif.c"
	"${LWIP_PATH}/src/netif/etharp.c"
	"${LWIP_PATH}/src/core/def.c"
	"${LWIP_PATH}/src/core/init.c"
	"${LWIP_PATH}/src/core/lwip_timers.c"
	"${LWIP_PATH}/src/core/mem.c"
	"${LWIP_PATH}/src/core/memp.c"
	"${LWIP_PATH}/src/core/netif.c"
	"${LWIP_PATH}/src/core/pbuf.c"
	"${LWIP_PATH}/src/core/raw.c"
	"${LWIP_PATH}/src/core/tcp.c"
	"${LWIP_PATH}/src/core/tcp_in.c"
	"${LWIP_PATH}/src/core/tcp_out.c"
	"${LWIP_PATH}/src/core/ipv4/icmp.c"
	"${LWIP_PATH}/src/core/ipv4/inet.c"
	"${LWIP_PATH}/src/core/ipv4/ip.c"
	"${LWIP_PATH}/src/core/ipv4/ip_addr.c"
	"${LWIP_PATH}/src/core/ipv4/ip_frag.c"
	"${LWIP_PATH}/src/core/ipv6/inet6.c"
)

set_source_files_properties(
	"${LWIP_PATH}/src/core/tcp.c"
	"${LWIP_PATH}/src/core/tcp_in.c"
	"${LWIP_PATH}/src/core/tcp_out.c"
	"${LWIP_PATH}/src/core/ipv4/icmp.c"
	"${LWIP_PATH}/src/core/ipv4/ip.c"
	PROPERTIES COMPILE_FLAGS
	"-Wno-format"
)

set(SOURCES
	"${BSP_SOURCES}"
	"${HAL_SOURCES}"
	"${FATFS_SOURCES}"
	"${LWIP_SOURCES}"
)
if(USE_CUBE_MX)
	set(SOURCES ${SOURCES} ${CUBEMX_SOURCES})
else()
	set(SOURCES ${SOURCES} ${HW_SOURCES})
endif()

add_library(${PROJECT_NAME} OBJECT
	${SOURCES}
)

# TODO: fix this include (neccessary for ISR)
include_directories("${MAIN_DIR}/main") # For timer_events.h
include_directories("${MAIN_DIR}/lwip_conf") # For LwIP options and ethernetif.h

target_include_directories(${PROJECT_NAME} PUBLIC
	"${HAL_PATH}/Inc"
	"BSP/STM3210C_EVAL"
	"BSP/Components/ili9325"
	"BSP/Components/ili9320"
	"Utilities/Fonts"
	"CMSIS/Device/ST/STM32F1xx/Include"
	"CMSIS/Include"

	# FATFS includes
	"${FATFS_PATH}/src"
	"${FATFS_PATH}/src/drivers"

	# LWIP includes
	"${LWIP_PATH}/src/include"
	"${LWIP_PATH}/src/include/lwip"
	"${LWIP_PATH}/src/include/ipv4"
	"${LWIP_PATH}/system"
	
	# Cryptographic includes
	"${CRYPTO_PATH}/Inc"
)

if(USE_CUBE_MX)
	target_include_directories(${PROJECT_NAME} PUBLIC ${CUBEMX_PATH}/Inc)
else()
	target_include_directories(${PROJECT_NAME} PUBLIC ${HW_PATH})
endif()
