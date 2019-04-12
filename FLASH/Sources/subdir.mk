################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/IL.c" \
"../Sources/MCU_init.c" \
"../Sources/Timer.c" \
"../Sources/main.c" \

C_SRCS += \
../Sources/IL.c \
../Sources/MCU_init.c \
../Sources/Timer.c \
../Sources/main.c \

OBJS += \
./Sources/IL_c.obj \
./Sources/MCU_init_c.obj \
./Sources/Timer_c.obj \
./Sources/main_c.obj \

OBJS_QUOTED += \
"./Sources/IL_c.obj" \
"./Sources/MCU_init_c.obj" \
"./Sources/Timer_c.obj" \
"./Sources/main_c.obj" \

C_DEPS += \
./Sources/IL_c.d \
./Sources/MCU_init_c.d \
./Sources/Timer_c.d \
./Sources/main_c.d \

C_DEPS_QUOTED += \
"./Sources/IL_c.d" \
"./Sources/MCU_init_c.d" \
"./Sources/Timer_c.d" \
"./Sources/main_c.d" \

OBJS_OS_FORMAT += \
./Sources/IL_c.obj \
./Sources/MCU_init_c.obj \
./Sources/Timer_c.obj \
./Sources/main_c.obj \


# Each subdirectory must supply rules for building sources it contributes
Sources/IL_c.obj: ../Sources/IL.c
	@echo 'Building file: $<'
	@echo 'Executing target #1 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Sources/IL.args" -ObjN="Sources/IL_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

Sources/%.d: ../Sources/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/MCU_init_c.obj: ../Sources/MCU_init.c
	@echo 'Building file: $<'
	@echo 'Executing target #2 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Sources/MCU_init.args" -ObjN="Sources/MCU_init_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

Sources/Timer_c.obj: ../Sources/Timer.c
	@echo 'Building file: $<'
	@echo 'Executing target #3 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Sources/Timer.args" -ObjN="Sources/Timer_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

Sources/main_c.obj: ../Sources/main.c
	@echo 'Building file: $<'
	@echo 'Executing target #4 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Sources/main.args" -ObjN="Sources/main_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '


