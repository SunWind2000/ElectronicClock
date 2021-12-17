################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
function/I2C_Key.obj: ../function/I2C_Key.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"D:/soft/TI-CCS_IDE/ccsv6/tools/compiler/ti-cgt-msp430_15.12.7.LTS/bin/cl430" -vmsp --use_hw_mpy=none --include_path="D:/soft/TI-CCS_IDE/ccsv6/ccs_base/msp430/include" --include_path="E:/MSP430 Project/Electronic_Clock/function" --include_path="E:/MSP430 Project/Electronic_Clock/src" --include_path="D:/soft/TI-CCS_IDE/ccsv6/tools/compiler/ti-cgt-msp430_15.12.7.LTS/include" -g --define=__MSP430G2553__ --display_error_number --diag_wrap=off --diag_warning=225 --printf_support=minimal --preproc_with_compile --preproc_dependency="function/I2C_Key.d" --obj_directory="function" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

function/LCD_Display.obj: ../function/LCD_Display.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"D:/soft/TI-CCS_IDE/ccsv6/tools/compiler/ti-cgt-msp430_15.12.7.LTS/bin/cl430" -vmsp --use_hw_mpy=none --include_path="D:/soft/TI-CCS_IDE/ccsv6/ccs_base/msp430/include" --include_path="E:/MSP430 Project/Electronic_Clock/function" --include_path="E:/MSP430 Project/Electronic_Clock/src" --include_path="D:/soft/TI-CCS_IDE/ccsv6/tools/compiler/ti-cgt-msp430_15.12.7.LTS/include" -g --define=__MSP430G2553__ --display_error_number --diag_wrap=off --diag_warning=225 --printf_support=minimal --preproc_with_compile --preproc_dependency="function/LCD_Display.d" --obj_directory="function" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

function/Sec_Clock.obj: ../function/Sec_Clock.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"D:/soft/TI-CCS_IDE/ccsv6/tools/compiler/ti-cgt-msp430_15.12.7.LTS/bin/cl430" -vmsp --use_hw_mpy=none --include_path="D:/soft/TI-CCS_IDE/ccsv6/ccs_base/msp430/include" --include_path="E:/MSP430 Project/Electronic_Clock/function" --include_path="E:/MSP430 Project/Electronic_Clock/src" --include_path="D:/soft/TI-CCS_IDE/ccsv6/tools/compiler/ti-cgt-msp430_15.12.7.LTS/include" -g --define=__MSP430G2553__ --display_error_number --diag_wrap=off --diag_warning=225 --printf_support=minimal --preproc_with_compile --preproc_dependency="function/Sec_Clock.d" --obj_directory="function" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


