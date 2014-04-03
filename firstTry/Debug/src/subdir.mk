################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/blinkLed.c \
../src/delay.c \
../src/gpio.c \
../src/interrupts.c \
../src/led.c \
../src/sysclk.c 

S_UPPER_SRCS += \
../src/init.S 

OBJS += \
./src/blinkLed.o \
./src/delay.o \
./src/gpio.o \
./src/init.o \
./src/interrupts.o \
./src/led.o \
./src/sysclk.o 

C_DEPS += \
./src/blinkLed.d \
./src/delay.d \
./src/gpio.d \
./src/interrupts.d \
./src/led.d \
./src/sysclk.d 

S_UPPER_DEPS += \
./src/init.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=arm7tdmi -marm -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -I/home/user/Dropbox/SE1/includes -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU Assembler'
	arm-none-eabi-gcc -mcpu=arm7tdmi -marm -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


