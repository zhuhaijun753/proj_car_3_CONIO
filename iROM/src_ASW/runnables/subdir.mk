################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src_ASW/runnables/runnable_table.cpp \
../src_ASW/runnables/runnable_table_config.cpp \
../src_ASW/runnables/swc_control.cpp \
../src_ASW/runnables/swc_control_0_manual.cpp \
../src_ASW/runnables/swc_dead_reackoning.cpp \
../src_ASW/runnables/swc_engine.cpp \
../src_ASW/runnables/swc_gfx_diagnostic.cpp \
../src_ASW/runnables/swc_lidar.cpp \
../src_ASW/runnables/swc_remote.cpp \
../src_ASW/runnables/swc_touch_controller.cpp \
../src_ASW/runnables/swc_ui.cpp \
../src_ASW/runnables/swc_view_controller.cpp 

OBJS += \
./src_ASW/runnables/runnable_table.o \
./src_ASW/runnables/runnable_table_config.o \
./src_ASW/runnables/swc_control.o \
./src_ASW/runnables/swc_control_0_manual.o \
./src_ASW/runnables/swc_dead_reackoning.o \
./src_ASW/runnables/swc_engine.o \
./src_ASW/runnables/swc_gfx_diagnostic.o \
./src_ASW/runnables/swc_lidar.o \
./src_ASW/runnables/swc_remote.o \
./src_ASW/runnables/swc_touch_controller.o \
./src_ASW/runnables/swc_ui.o \
./src_ASW/runnables/swc_view_controller.o 

CPP_DEPS += \
./src_ASW/runnables/runnable_table.d \
./src_ASW/runnables/runnable_table_config.d \
./src_ASW/runnables/swc_control.d \
./src_ASW/runnables/swc_control_0_manual.d \
./src_ASW/runnables/swc_dead_reackoning.d \
./src_ASW/runnables/swc_engine.d \
./src_ASW/runnables/swc_gfx_diagnostic.d \
./src_ASW/runnables/swc_lidar.d \
./src_ASW/runnables/swc_remote.d \
./src_ASW/runnables/swc_touch_controller.d \
./src_ASW/runnables/swc_ui.d \
./src_ASW/runnables/swc_view_controller.d 


# Each subdirectory must supply rules for building sources it contributes
src_ASW/runnables/%.o: ../src_ASW/runnables/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: TriCore C++ Compiler'
	"$(TRICORE_TOOLS)/bin/tricore-gcc" -c -I"E:\Repos\CONIO_Project\proj_car_3\src_ASW\tasks\application" -I"E:\Repos\CONIO_Project\proj_car_3\src_BSW\msg" -I"E:\Repos\CONIO_Project\proj_car_3\src_BSW\msg_config" -I"E:\Repos\CONIO_Project\proj_car_3\src_ASW" -I"E:\Repos\CONIO_Project\proj_car_3\src_BSW\drv\CONIO" -I"E:\Repos\CONIO_Project\proj_car_3\src_BSW\drv\TFT" -I"E:\Repos\CONIO_Project\proj_car_3\src_BSW\drv\Touch" -I"E:\Repos\CONIO_Project\proj_car_3\src_BSW\drv_config\CONIO" -I"E:\Repos\CONIO_Project\proj_car_3\src_BSW\drv\QSPI" -I"E:\Repos\CONIO_Project\proj_car_3\src_BSW\drv_config\QSPI" -I"E:\Repos\CONIO_Project\proj_car_3\src_ASW\state" -I"E:\Repos\CONIO_Project\proj_car_3\src_BSW\drv_config\Interrupts" -I"E:\Repos\CONIO_Project\proj_car_3\src_BSW\drv\Interrupts" -I"E:\Repos\CONIO_Project\proj_car_3\src_BSW\drv_config\GPT" -I"E:\Repos\CONIO_Project\proj_car_3\src_BSW\drv\GPT" -I"E:\Repos\CONIO_Project\proj_car_3\src_BSW\drv_config\System" -I"E:\Repos\CONIO_Project\proj_car_3\src_ASW\tasks" -I"E:\Repos\CONIO_Project\proj_car_3\src_ASW\tasks\system" -I"E:\Repos\CONIO_Project\proj_car_3\src_BSW\os" -I"E:\Repos\CONIO_Project\proj_car_3\src_BSW\srv" -I"E:\Repos\CONIO_Project\proj_car_3\src_BSW\drv\system" -I"E:\Repos\CONIO_Project\proj_car_3\src_BSW\drv\Port" -I"E:\Repos\CONIO_Project\proj_car_3\src_BSW\drv_config\Port" -I"E:\Repos\CONIO_Project\proj_car_3\src_ASW\tasks\init" -I"E:\Repos\CONIO_Project\proj_car_3\src_BSW\drv\CAN" -I"E:\Repos\CONIO_Project\proj_car_3\src_BSW\drv\DTS" -I"E:\Repos\CONIO_Project\proj_car_3\src_BSW\drv\UART" -I"E:\Repos\CONIO_Project\proj_car_3\src_BSW\drv_config\CAN" -I"E:\Repos\CONIO_Project\proj_car_3\src_BSW\drv_config\UART" -I"E:\Repos\CONIO_Project\proj_car_3\src_BSW\srv\DataHandling" -I"E:\Repos\CONIO_Project\proj_car_3\src_ASW\runnables" -I"E:\Repos\CONIO_Project\proj_car_3\src_ASW\rte" -fno-common -Os -g3 -W -Wall -Wextra -Wdiv-by-zero -Warray-bounds -Wcast-align -Wignored-qualifiers -Wformat -Wformat-security -DPXROS -D__TC161__ -fshort-double -mcpu=tc29xx -mversion-info -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


