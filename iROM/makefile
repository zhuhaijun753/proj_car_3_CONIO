################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src_BSW/startup/subdir.mk
-include src_BSW/srv/DataHandling/subdir.mk
-include src_BSW/srv/subdir.mk
-include src_BSW/msg_config/subdir.mk
-include src_BSW/msg/subdir.mk
-include src_BSW/drv_config/UART/subdir.mk
-include src_BSW/drv_config/QSPI/subdir.mk
-include src_BSW/drv_config/Port/subdir.mk
-include src_BSW/drv_config/Interrupts/subdir.mk
-include src_BSW/drv_config/GPT/subdir.mk
-include src_BSW/drv_config/CONIO/VIEW/subdir.mk
-include src_BSW/drv_config/CAN/subdir.mk
-include src_BSW/drv/system/subdir.mk
-include src_BSW/drv/UART/subdir.mk
-include src_BSW/drv/Touch/subdir.mk
-include src_BSW/drv/TFT/subdir.mk
-include src_BSW/drv/QSPI/subdir.mk
-include src_BSW/drv/Port/subdir.mk
-include src_BSW/drv/Interrupts/subdir.mk
-include src_BSW/drv/GPT/subdir.mk
-include src_BSW/drv/DTS/subdir.mk
-include src_BSW/drv/CONIO/GFX/Adafruit_GFX/subdir.mk
-include src_BSW/drv/CONIO/GFX/subdir.mk
-include src_BSW/drv/CONIO/BASIC/subdir.mk
-include src_BSW/drv/CONIO/subdir.mk
-include src_BSW/drv/CAN/subdir.mk
-include src_ASW/tasks/system/subdir.mk
-include src_ASW/tasks/init/subdir.mk
-include src_ASW/tasks/application/subdir.mk
-include src_ASW/state/subdir.mk
-include src_ASW/runnables/subdir.mk
-include src_ASW/rte/subdir.mk
-include src_ASW/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

#rebuild target
rebuild: clean all

# All Target
all: PXROS_TC297.elf

# Tool invocations
PXROS_TC297.elf: $(OBJS) $(USER_OBJS) $(ASM)
	@echo 'Building target: $@'
	@echo 'Invoking: TriCore C++ Linker'
	"$(TRICORE_TOOLS)/bin/tricore-g++" -o  "PXROS_TC297.elf" -nocrt0 -T"..\ld\main.ld" @iROM.objectlist -Wl,--gc-sections -mcpu=tc29xx -Wl,--warn-orphan -Wl,--fatal-warnings -L"E:\Repos\CONIO_Project\proj_car_3\src_BSW\os" -lpxname -lpxrosdbg -lpxcon -lpxdbg -Wl,-Map="$(basename $(notdir $@)).map" -Wl,--cref -fshort-double -Wl,--extmap="a"
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(CC_DEPS)$(C++_DEPS)$(PCP_S)$(EXECUTABLES)$(OBJS)$(C_UPPER_DEPS)$(CXX_DEPS)$(C_DEPS)$(CPP_DEPS)$(CPCP_DEPS) PXROS_TC297.elf
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
