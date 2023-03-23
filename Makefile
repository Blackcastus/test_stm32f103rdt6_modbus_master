#
# @Author: nhantt
# @Date:   2020-02-08
# @Last Modified by:   DucHien
# @Last Modified time: 2022-01-05
#

######################################
# target
######################################
TARGET = ph_environmental_cabinet_controller_io_fw

#######################################
# check platform and add DIR
#######################################
# /d/2_Tool/OpenOCD-20211118-0.11.0
# /c/Users/Black_Life/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.4

ifeq ($(OS),Windows_NT)
	OPENOCD_DIR ?= /d/2_Tool/OpenOCD-20211118-0.11.0
	LIB_DIR ?= /c/Users/Black_Life/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.4
	CC_PATH ?= "/tools/stm32/gcc/bin"
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Darwin)
		OPENOCD_DIR ?= /usr/local
		LIB_DIR ?= /home/smartth/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.4/
		CC_PATH ?= /Users/nhantt/tools/stm32/gcc-arm-none-eabi-10-2020-q4-major/bin/arm-none-eabi-gcc
	endif
	ifeq ($(UNAME_S),Linux)
		OPENOCD_DIR ?= /usr/local
		LIB_DIR ?= /home/smartth/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.4/
		CC_PATH ?= /Users/nhantt/tools/stm32/gcc-arm-none-eabi-10-2020-q4-major/bin/arm-none-eabi-gcc
	endif
endif

#Check openocd install global
ISOCD_EXIST := $(shell command -v openocd 2> /dev/null)
ifndef ISOCD_EXIST
	OPENOCD    = $(OPENOCD_DIR)/bin/openocd
else
	OPENOCD    = openocd
endif


######################################
# building variables
######################################
# debug build?
DEBUG = 1
# optimization
OPT = -Og


#######################################
# paths
#######################################
# Build path
BUILD_DIR = build
REALESE_DIR = realeses
######################################
# source
######################################
# C sources
C_SOURCES =  \
Src/main.c \
Src/gpio.c \
Src/stm32f1xx_it.c \
Src/stm32f1xx_hal_msp.c \
Src/system_stm32f1xx.c \
$(LIB_DIR)/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal.c \
$(LIB_DIR)/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_pcd.c \
$(LIB_DIR)/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_pcd_ex.c \
$(LIB_DIR)/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_adc.c \
$(LIB_DIR)/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_adc_ex.c \
$(LIB_DIR)/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_iwdg.c \
$(LIB_DIR)/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rtc.c \
$(LIB_DIR)/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rtc_ex.c \
$(LIB_DIR)/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_spi.c \
$(LIB_DIR)/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim.c \
$(LIB_DIR)/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim_ex.c \
$(LIB_DIR)/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_uart.c \
$(LIB_DIR)/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc.c \
$(LIB_DIR)/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc_ex.c \
$(LIB_DIR)/Drivers/STM32F1xx_HAL_Driver/Src/Src/stm32f1xx_hal_gpio.c \
$(LIB_DIR)/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_dma.c \
$(LIB_DIR)/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_cortex.c \
$(LIB_DIR)/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_pwr.c \
$(LIB_DIR)/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash.c \
$(LIB_DIR)/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash_ex.c \
$(LIB_DIR)/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_ll_usb.c \
$(LIB_DIR)/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_ll_usart.c \
$(LIB_DIR)/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_core.c \
$(LIB_DIR)/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.c \
$(LIB_DIR)/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ioreq.c \
$(LIB_DIR)/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Src/usbd_cdc.c \
User/Modbus/fifo.c \
User/Modbus/modbus_crc.c \
User/Modbus/modbus_data.c \
User/Modbus/usart.c \
User/USB/usbd_conf.c \
User/USB/usb_device.c \
User/USB/usbd_desc.c \
User/USB/usbd_cdc_if.c \

# ASM sources
ASM_SOURCES =  \
startup_stm32f103xe.s


#######################################
# binaries
#######################################
PREFIX = arm-none-eabi-
# The gcc compiler bin path can be either defined in make command via GCC_PATH variable (> make GCC_PATH=xxx)
# either it can be added to the PATH environment variable.
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
else
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S
 
#######################################
# CFLAGS
#######################################
# cpu
CPU = -mcpu=cortex-m3

# fpu
# NONE for Cortex-M0/M0+/M3

# float-abi


# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS = 

# C defines
C_DEFS =  \
-DUSE_HAL_DRIVER \
-DSTM32F103xE \
-DUSE_HAL_DRIVER \
-DSTM32F103xE


# AS includes
AS_INCLUDES = 

# C includes
C_INCLUDES =  \
-IInc \
-I$(LIB_DIR)/Drivers/STM32F1xx_HAL_Driver/Inc \
-I$(LIB_DIR)/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy \
-I$(LIB_DIR)/Middlewares/ST/STM32_USB_Device_Library/Core/Inc \
-I$(LIB_DIR)/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc \
-I$(LIB_DIR)/Drivers/CMSIS/Device/ST/STM32F1xx/Include \
-I$(LIB_DIR)/Drivers/CMSIS/Include \
-IUser/Modbus \
-IUser/USB	\


# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif


# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"


#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = STM32F103RDTx_FLASH.ld

# libraries
LIBS = -lc -lm -lnosys -u _printf_float
LIBDIR = 
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin


#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	@$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	@$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@	
	
$(BUILD_DIR):
	mkdir $@

#######################################
# flash
#######################################

flash:
	openocd 	-f interface/stlink-v2.cfg \
				-f target/stm32f1x.cfg \
		        -c init -c targets -c "reset halt" \
		        -c "flash write_image erase $(BUILD_DIR)/$(TARGET).hex" \
		        -c "verify_image $(BUILD_DIR)/$(TARGET).hex" \
		        -c "reset run" -c shutdown

				
# flash:
# 	$(OPENOCD) 	-f interface/stlink-v2.cfg \
# 				-f target/stm32f1x.cfg \
# 		        -c init -c targets -c "reset halt" \
# 		        -c "flash write_image erase $(BUILD_DIR)/$(TARGET).hex" \
# 		        -c "verify_image $(BUILD_DIR)/$(TARGET).hex" \
# 		        -c "reset run" -c shutdown

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)
  
#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)

# *** EOF ***