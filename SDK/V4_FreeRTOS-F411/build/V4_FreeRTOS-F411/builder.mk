EIDE_UNIFY_BUILDER := 1
CFLAGS := -c --apcs=interwork --cpu Cortex-M4.fp --c99 -D__MICROLIB -O0 --split_sections --diag_suppress=1 --diag_suppress=1295 -g -I.\Core\Inc -I.\Drivers\STM32F4xx_HAL_Driver\Inc -I.\Drivers\STM32F4xx_HAL_Driver\Inc\Legacy -I.\Middlewares\Third_Party\FreeRTOS\Source\include -I.\Middlewares\Third_Party\FreeRTOS\Source\CMSIS_RTOS_V2 -I.\Middlewares\Third_Party\FreeRTOS\Source\portable\RVDS\ARM_CM4F -I.\Drivers\CMSIS\Device\ST\STM32F4xx\Include -I.\Drivers\CMSIS\Include -I.\Drivers\Hardware\inc -I.\APP\inc -I.\.cmsis\include -I.\MDK-ARM\RTE\_V4_FreeRTOS-F411 -I.\.eide\deps -DUSE_HAL_DRIVER -DSTM32F411xE
CXXFLAGS := -c --cpp --apcs=interwork --cpu Cortex-M4.fp -D__MICROLIB -O0 --split_sections --diag_suppress=1 --diag_suppress=1295 -g -I.\Core\Inc -I.\Drivers\STM32F4xx_HAL_Driver\Inc -I.\Drivers\STM32F4xx_HAL_Driver\Inc\Legacy -I.\Middlewares\Third_Party\FreeRTOS\Source\include -I.\Middlewares\Third_Party\FreeRTOS\Source\CMSIS_RTOS_V2 -I.\Middlewares\Third_Party\FreeRTOS\Source\portable\RVDS\ARM_CM4F -I.\Drivers\CMSIS\Device\ST\STM32F4xx\Include -I.\Drivers\CMSIS\Include -I.\Drivers\Hardware\inc -I.\APP\inc -I.\.cmsis\include -I.\MDK-ARM\RTE\_V4_FreeRTOS-F411 -I.\.eide\deps -DUSE_HAL_DRIVER -DSTM32F411xE
ASMFLAGS := --apcs=interwork --cpu Cortex-M4.fp --pd "__MICROLIB SETA 1" -g -I.\Core\Inc -I.\Drivers\STM32F4xx_HAL_Driver\Inc -I.\Drivers\STM32F4xx_HAL_Driver\Inc\Legacy -I.\Middlewares\Third_Party\FreeRTOS\Source\include -I.\Middlewares\Third_Party\FreeRTOS\Source\CMSIS_RTOS_V2 -I.\Middlewares\Third_Party\FreeRTOS\Source\portable\RVDS\ARM_CM4F -I.\Drivers\CMSIS\Device\ST\STM32F4xx\Include -I.\Drivers\CMSIS\Include -I.\Drivers\Hardware\inc -I.\APP\inc -I.\.cmsis\include -I.\MDK-ARM\RTE\_V4_FreeRTOS-F411 -I.\.eide\deps
LDFLAGS := --cpu Cortex-M4.fp --library_type=microlib --scatter "c:/Users/Jason/Desktop/Engineering/WuhuTakeOff/SDK/V4_FreeRTOS-F411/build/V4_FreeRTOS-F411/V4_FreeRTOS-F411.sct" --strict --summary_stderr --info summarysizes --map --xref --callgraph --symbols --info sizes --info totals --info unused --info veneers --list .\build\V4_FreeRTOS-F411\V4_FreeRTOS-F411.map
LDLIBS := 
