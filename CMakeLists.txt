cmake_minimum_required(VERSION 3.1)
include(cmake/arm.cmake)
enable_language(ASM)  # needed for startup_*.s files
project(stm32)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)  # optional, creates compile_commands.json


# Linker
#set(LINKER_SPECS "--specs=nano.specs --specs=nosys.specs")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${LINKER_SPECS}")
set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} ${LINKER_SPECS}")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${LINKER_SPECS}")

# Compiler
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions -fno-rtti")

set(STM32CUBEMX_GENERATED_FILES
        Cube/HAL/Drivers/src/stm32f4xx_hal_tim.c
        Cube/HAL/Drivers/src/stm32f4xx_hal_tim_ex.c
        Cube/HAL/Drivers/src/stm32f4xx_hal_rcc.c
        Cube/HAL/Drivers/src/stm32f4xx_hal_rcc_ex.c
        Cube/HAL/Drivers/src/stm32f4xx_hal_flash.c
        Cube/HAL/Drivers/src/stm32f4xx_hal_flash_ex.c
        Cube/HAL/Drivers/src/stm32f4xx_hal_flash_ramfunc.c
        Cube/HAL/Drivers/src/stm32f4xx_hal_gpio.c
        Cube/HAL/Drivers/src/stm32f4xx_hal_dma_ex.c
        Cube/HAL/Drivers/src/stm32f4xx_hal_dma.c
        Cube/HAL/Drivers/src/stm32f4xx_hal_pwr.c
        Cube/HAL/Drivers/src/stm32f4xx_hal_pwr_ex.c
        Cube/HAL/Drivers/src/stm32f4xx_hal_cortex.c
        Cube/HAL/Drivers/src/stm32f4xx_hal.c
        Cube/HAL/Drivers/src/stm32f4xx_hal_exti.c
        Cube/HAL/Drivers/src/stm32f4xx_hal_pcd.c
        Cube/HAL/Drivers/src/stm32f4xx_hal_pcd_ex.c
        Cube/HAL/Drivers/src/stm32f4xx_hal_eth.c
        Cube/HAL/Drivers/src/stm32f4xx_hal_rng.c
        Cube/HAL/Drivers/src/stm32f4xx_hal_rtc.c
        Cube/HAL/Drivers/src/stm32f4xx_ll_usb.c
        Cube/HAL/Core/startup_stm32f407igtx.s
        Cube/HAL/Core/src/freertos.c
        Cube/HAL/Core/src/stm32f4xx_hal_msp.c
        Cube/HAL/Core/src/stm32f4xx_hal_timebase_tim.c
        Cube/HAL/Core/src/stm32f4xx_it.c
        Cube/HAL/Core/src/syscalls.c
        Cube/HAL/Core/src/sysmem.c
        Cube/HAL/Core/src/system_stm32f4xx.c
        Cube/FreeRTOS/src/heap_4.c
        Cube/FreeRTOS/src/port.c
        Cube/FreeRTOS/src/tasks.c
        Cube/FreeRTOS/src/list.c
        Cube/FreeRTOS/src/queue.c
        Cube/FreeRTOS/src/timers.c
        Cube/FreeRTOS/src/cmsis_os2.c
        Cube/Middlewares/usb/src/usbd_cdc.c
        Cube/Middlewares/usb/src/usbd_cdc_if.c
        Cube/Middlewares/usb/src/usbd_conf.c
        Cube/Middlewares/usb/src/usbd_core.c
        Cube/Middlewares/usb/src/usbd_ctlreq.c
        Cube/Middlewares/usb/src/usbd_desc.c
        Cube/Middlewares/usb/src/usb_device.c
        Cube/Middlewares/usb/src/usbd_ioreq.c
        src/mainloop.cpp
)

set(EXECUTABLE ${PROJECT_NAME}.out)
add_executable(${EXECUTABLE} ${STM32CUBEMX_GENERATED_FILES})

target_compile_definitions(${EXECUTABLE} PRIVATE
        -DUSE_HAL_DRIVER
        -DSTM32F407xx
        )

target_include_directories(${EXECUTABLE} PRIVATE Cube/HAL/Drivers/include Cube/CMSIS/include Cube/Core/include Cube/FreeRTOS/include Cube/Middlewares/usb/include include)

target_compile_options(${EXECUTABLE} PRIVATE
        -mcpu=cortex-m4
        -mthumb
        -mfpu=fpv4-sp-d16
        -mfloat-abi=hard
        -fdata-sections
        -ffunction-sections
        -Wall
        $<$<CONFIG:Debug>:-Og>
        )

target_link_options(${EXECUTABLE} PRIVATE
        -T${CMAKE_SOURCE_DIR}/Cube/HAL/STM32F407IGTX_FLASH.ld
        -mcpu=cortex-m4
        -mthumb
        -mfpu=fpv4-sp-d16
        -mfloat-abi=hard
        -specs=nano.specs
        -lc
        -lm
        -lnosys
        -Wl,-Map=${PROJECT_NAME}.map,--cref
        -Wl,--gc-sections
        )

# Print executable size
add_custom_command(TARGET ${EXECUTABLE}
        POST_BUILD
        COMMAND arm-none-eabi-size ${EXECUTABLE})

# Create hex file
add_custom_command(TARGET ${EXECUTABLE}
        POST_BUILD
        COMMAND arm-none-eabi-objcopy -O ihex ${EXECUTABLE} ${PROJECT_NAME}.hex
        COMMAND arm-none-eabi-objcopy -O binary ${EXECUTABLE} ${PROJECT_NAME}.bin)
