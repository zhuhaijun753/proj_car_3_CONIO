################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src_BSW/srv/det_time.cpp \
../src_BSW/srv/event_driver.cpp \
../src_BSW/srv/rte_time.cpp 

C_SRCS += \
../src_BSW/srv/CFG_check.c \
../src_BSW/srv/det.c \
../src_BSW/srv/sync.c \
../src_BSW/srv/trap_handler_init.c \
../src_BSW/srv/trap_handler_runtime.c 

OBJS += \
./src_BSW/srv/CFG_check.o \
./src_BSW/srv/det.o \
./src_BSW/srv/det_time.o \
./src_BSW/srv/event_driver.o \
./src_BSW/srv/rte_time.o \
./src_BSW/srv/sync.o \
./src_BSW/srv/trap_handler_init.o \
./src_BSW/srv/trap_handler_runtime.o 

C_DEPS += \
./src_BSW/srv/CFG_check.d \
./src_BSW/srv/det.d \
./src_BSW/srv/sync.d \
./src_BSW/srv/trap_handler_init.d \
./src_BSW/srv/trap_handler_runtime.d 

CPP_DEPS += \
./src_BSW/srv/det_time.d \
./src_BSW/srv/event_driver.d \
./src_BSW/srv/rte_time.d 


# Each subdirectory must supply rules for building sources it contributes
src_BSW/srv/%.o: ../src_BSW/srv/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: TriCore C Compiler'
	"$(TRICORE_TOOLS)/bin/tricore-gcc" -c -I"E:\Repos\CONIO_Project\proj_car_3\src_BSW\msg" -I"E:\Repos\CONIO_Project\proj_car_3\src_BSW\msg_config" -I"E:\Repos\CONIO_Project\proj_car_3\src_ASW" -I"E:\Repos\CONIO_Project\proj_car_3\src_BSW\drv\CONIO" -I"E:\Repos\CONIO_Project\proj_car_3\src_BSW\drv\TFT" -I"E:\Repos\CONIO_Project\proj_car_3\src_BSW\drv\Touch" -I"E:\Repos\CONIO_Project\proj_car_3\src_BSW\drv_config\CONIO" -I"E:\Repos\CONIO_Project\proj_car_3\src_BSW\drv_config\Interrupts" -I"E:\Repos\CONIO_Project\proj_car_3\src_BSW\drv\Interrupts" -I"E:\Repos\CONIO_Project\proj_car_3\src_BSW\drv_config\GPT" -I"E:\Repos\CONIO_Project\proj_car_3\src_BSW\drv\GPT" -I"E:\Repos\CONIO_Project\proj_car_3\src_BSW\drv_config\System" -I"E:\Repos\CONIO_Project\proj_car_3\src_ASW\tasks" -I"E:\Repos\CONIO_Project\proj_car_3\src_ASW\tasks\application" -I"E:\Repos\CONIO_Project\proj_car_3\src_ASW\tasks\init" -I"E:\Repos\CONIO_Project\proj_car_3\src_ASW\tasks\system" -I"E:\Repos\CONIO_Project\proj_car_3\src_BSW" -I"E:\Repos\CONIO_Project\proj_car_3\src_BSW\drv" -I"E:\Repos\CONIO_Project\proj_car_3\src_BSW\drv\CAN" -I"E:\Repos\CONIO_Project\proj_car_3\src_BSW\drv\DTS" -I"E:\Repos\CONIO_Project\proj_car_3\src_BSW\drv\Port" -I"E:\Repos\CONIO_Project\proj_car_3\src_BSW\drv\system" -I"E:\Repos\CONIO_Project\proj_car_3\src_BSW\drv\UART" -I"E:\Repos\CONIO_Project\proj_car_3\src_BSW\drv_config\CAN" -I"E:\Repos\CONIO_Project\proj_car_3\src_BSW\drv_config\Port" -I"E:\Repos\CONIO_Project\proj_car_3\src_BSW\drv_config\UART" -I"E:\Repos\CONIO_Project\proj_car_3\src_BSW\os" -I"E:\Repos\CONIO_Project\proj_car_3\src_BSW\srv" -I"E:\Repos\CONIO_Project\proj_car_3\src_BSW\startup" -I"E:\Repos\CONIO_Project\proj_car_3\src_BSW\srv\DataHandling" -I"E:\Repos\CONIO_Project\proj_car_3\src_ASW\runnables" -I"E:\Repos\CONIO_Project\proj_car_3\src_ASW\rte" -I"E:\Repos\CONIO_Project\proj_car_3\src_BSW\drv\QSPI" -I"E:\Repos\CONIO_Project\proj_car_3\src_BSW\drv_config\QSPI" -fno-common -Os -g3 -W -Wall -Wextra -Wdiv-by-zero -Warray-bounds -Wcast-align -Wignored-qualifiers -Wformat -Wformat-security -DPXROS -D__TC161__ -fshort-double -mcpu=tc29xx -mversion-info -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src_BSW/srv/%.o: ../src_BSW/srv/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: TriCore C++ Compiler'
	"$(TRICORE_TOOLS)/bin/tricore-gcc" -c -I"E:\Repos\CONIO_Project\proj_car_3\src_ASW\tasks\application" -I"E:\Repos\CONIO_Project\proj_car_3\src_BSW\msg" -I"E:\Repos\CONIO_Project\proj_car_3\src_BSW\msg_config" -I"E:\Repos\CONIO_Project\proj_car_3\src_ASW" -I"E:\Repos\CONIO_Project\proj_car_3\src_BSW\drv\CONIO" -I"E:\Repos\CONIO_Project\proj_car_3\src_BSW\drv\TFT" -I"E:\Repos\CONIO_Project\proj_car_3\src_BSW\drv\Touch" -I"E:\Repos\CONIO_Project\proj_car_3\src_BSW\drv_config\CONIO" -I"E:\Repos\CONIO_Project\proj_car_3\src_BSW\drv\QSPI" -I"E:\Repos\CONIO_Project\proj_car_3\src_BSW\drv_config\QSPI" -I"E:\Repos\CONIO_Project\proj_car_3\src_ASW\state" -I"E:\Repos\CONIO_Project\proj_car_3\src_BSW\drv_config\Interrupts" -I"E:\Repos\CONIO_Project\proj_car_3\src_BSW\drv\Interrupts" -I"E:\Repos\CONIO_Project\proj_car_3\src_BSW\drv_config\GPT" -I"E:\Repos\CONIO_Project\proj_car_3\src_BSW\drv\GPT" -I"E:\Repos\CONIO_Project\proj_car_3\src_BSW\drv_config\System" -I"E:\Repos\CONIO_Project\proj_car_3\src_ASW\tasks" -I"E:\Repos\CONIO_Project\proj_car_3\src_ASW\tasks\system" -I"E:\Repos\CONIO_Project\proj_car_3\src_BSW\os" -I"E:\Repos\CONIO_Project\proj_car_3\src_BSW\srv" -I"E:\Repos\CONIO_Project\proj_car_3\src_BSW\drv\system" -I"E:\Repos\CONIO_Project\proj_car_3\src_BSW\drv\Port" -I"E:\Repos\CONIO_Project\proj_car_3\src_BSW\drv_config\Port" -I"E:\Repos\CONIO_Project\proj_car_3\src_ASW\tasks\init" -I"E:\Repos\CONIO_Project\proj_car_3\src_BSW\drv\CAN" -I"E:\Repos\CONIO_Project\proj_car_3\src_BSW\drv\DTS" -I"E:\Repos\CONIO_Project\proj_car_3\src_BSW\drv\UART" -I"E:\Repos\CONIO_Project\proj_car_3\src_BSW\drv_config\CAN" -I"E:\Repos\CONIO_Project\proj_car_3\src_BSW\drv_config\UART" -I"E:\Repos\CONIO_Project\proj_car_3\src_BSW\srv\DataHandling" -I"E:\Repos\CONIO_Project\proj_car_3\src_ASW\runnables" -I"E:\Repos\CONIO_Project\proj_car_3\src_ASW\rte" -fno-common -Os -g3 -W -Wall -Wextra -Wdiv-by-zero -Warray-bounds -Wcast-align -Wignored-qualifiers -Wformat -Wformat-security -DPXROS -D__TC161__ -fshort-double -mcpu=tc29xx -mversion-info -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


