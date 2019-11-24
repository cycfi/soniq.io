SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_VERSION 1)
cmake_minimum_required(VERSION 3.7)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

###############################################################################
# specify cross compilers and tools

SET(CMAKE_C_COMPILER_WORKS 1)
SET(CMAKE_C_COMPILER arm-none-eabi-gcc)
SET(CMAKE_CXX_COMPILER_WORKS 1)
SET(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER  arm-none-eabi-gcc)
set(CMAKE_AR arm-none-eabi-ar)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP arm-none-eabi-objdump)
set(SIZE arm-none-eabi-size)

SET(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/STM32H743ZITx_FLASH.ld)

SET(FPU_FLAGS "-mfloat-abi=hard -mfpu=fpv4-sp-d16")

add_definitions(
   -DARM_MATH_CM4
   -DARM_MATH_MATRIX_CHECK
   -DARM_MATH_ROUNDING
   -DNUCLEO_H743ZI
   -DQ_DONT_USE_THREADS
   -D__FPU_PRESENT=1
   -DHSE_VALUE=24576000
)

SET(COMMON_FLAGS
   "-mcpu=cortex-m7 ${FPU_FLAGS} -mthumb -mthumb-interwork -ffunction-sections -fdata-sections \
   -g -fno-common -fmessage-length=0 -specs=nosys.specs -specs=nano.specs")

SET(CMAKE_CXX_FLAGS_INIT "${COMMON_FLAGS} -std=c++17")
SET(CMAKE_C_FLAGS_INIT "${COMMON_FLAGS} -std=gnu99")
SET(CMAKE_EXE_LINKER_FLAGS_INIT "-Wl,-gc-sections,--print-memory-usage -T ${LINKER_SCRIPT}")

###############################################################################
# project

PROJECT(soniq C CXX ASM)

add_definitions(
   -D__weak=__attribute__\(\(weak\)\)
   -D__packed=__attribute__\(\(__packed__\)\)
   -DUSE_HAL_DRIVER
   -DSTM32H743xx
   -DSTM32H7
)

###############################################################################
# infra
if (NOT DEFINED CYCFI_INFRA_ROOT)
  set(CYCFI_INFRA_ROOT ../../infra)
endif()

add_subdirectory(${CYCFI_INFRA_ROOT} "${CMAKE_CURRENT_BINARY_DIR}/infra")

###############################################################################
# q_lib
if (NOT DEFINED Q_LIB_ROOT)
  set(Q_LIB_ROOT ../../q/q_lib)
endif()

add_subdirectory(${Q_LIB_ROOT} "${CMAKE_CURRENT_BINARY_DIR}/q")

###############################################################################
# Soniq sources

file(GLOB_RECURSE SONIQ_SOURCES ../src/*.cpp "Src/*.*" "Inc/*.*")
list(FILTER SONIQ_SOURCES EXCLUDE REGEX ".*f4.cpp")

include_directories(
   ../include
   ${Q_HEADER}
   ${INFRA_HEADER}
   Inc
   SYSTEM
   Middlewares/Third_Party/LwIP/src/include Middlewares/Third_Party/LwIP/system
   Middlewares/Third_Party/LwIP/src/include/netif/ppp
   Middlewares/Third_Party/LwIP/src/include/lwip
   Middlewares/Third_Party/LwIP/src/include/lwip/apps
   Middlewares/Third_Party/LwIP/src/include/lwip/priv
   Middlewares/Third_Party/LwIP/src/include/lwip/prot
   Middlewares/Third_Party/LwIP/src/include/netif
   Middlewares/Third_Party/LwIP/src/include/posix
   Middlewares/Third_Party/LwIP/src/include/posix/sys
   Middlewares/Third_Party/LwIP/system/arch
   Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2
   Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F
   Drivers/CMSIS/Include Middlewares/Third_Party/FreeRTOS/Source/include
   Drivers/BSP/Components/lan8742
   Drivers/STM32H7xx_HAL_Driver/Inc
   Drivers/STM32H7xx_HAL_Driver/Inc/Legacy
   Drivers/CMSIS/Device/ST/STM32H7xx/Include
   Drivers/CMSIS/Include
)

###############################################################################
# Drivers

file(GLOB_RECURSE STM32_SOURCES "startup/*.*" "Middlewares/*.*" "Drivers/*.*")

add_library(stm32_drivers STATIC
   ${STM32_SOURCES}
)

###############################################################################
# Tests

function(add_test name)
   set(test_target test.soniq.${name})

   add_executable(
      ${test_target}.elf
      ${SONIQ_SOURCES}
      ../tests/${name}.cpp
      ${LINKER_SCRIPT}
   )

   target_link_libraries(${test_target}.elf libq infra stm32_drivers)

   set(CMAKE_EXE_LINKER_FLAGS
      "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Map=${PROJECT_BINARY_DIR}/${test_target}.map")

   set(HEX_FILE ${PROJECT_BINARY_DIR}/${test_target}.hex)
   set(BIN_FILE ${PROJECT_BINARY_DIR}/${test_target}.bin)

   add_custom_command(TARGET ${test_target}.elf POST_BUILD
      COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${test_target}.elf> ${HEX_FILE}
      COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${test_target}.elf> ${BIN_FILE}
      COMMENT "Building ${HEX_FILE}
      Building ${BIN_FILE}"
   )

   add_custom_target(upload_${test_target}
      arm-none-eabi-gdb -iex "target remote tcp:127.0.0.1:3333"
      -iex "monitor program $<TARGET_FILE:${test_target}.elf>"
      -iex "monitor reset init"
      -iex "disconnect" -iex "quit"
   )
endfunction()

add_test(blinky)
add_test(button_test)
add_test(button_interrupt_test)
add_test(thread_test)
add_test(udp_echo_test)


