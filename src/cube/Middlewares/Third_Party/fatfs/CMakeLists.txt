project(fatfs C)

set(SOURCES
	"src/drivers/sd_diskio.c"
	"src/diskio.c"
	"src/ff.c"
	"src/ff_gen_drv.c"
)

include_directories("${CUBE_DIR}/STM32F1xx_HAL_Driver/Inc")

add_library(${PROJECT_NAME} STATIC ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC "src")