//*****************************************************************************
// File:Main.c
// Implements the requirements for prob3, HW5
// @authorRavi Prakash Dubey
// Date:6th April, 2018
// blinky.c - Example has been leveraged for this problem
//
//*****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"

#include "FreeRTOS.h" //should be the first to be included from amongst all free rtos files
#include "task.h"
#include "timers.h"

#define STACK_DEPTH 1024 //1024 words
#define GPIO_PIN_ON           1
#define GPIO_PIN_OFF          0
#define TICKS_PER_SECOND      400

//Global Variables
TaskHandle_t Task1Handle, Task2Handle;

void vTask1(void* pvParameters)
{
    // uint32_t ulFrequency = *((uint32_t*) pvParameters);

    // Enable the GPIO port that is used for the on-board LED.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    // Check if the peripheral access is enabled.
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF))
    {
    }

    // Enable the GPIO pin for the LED (PF0).  Set the direction as output, and
    // enable the GPIO pin for digital function.
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_0);

//    const TickType_t xDelayTicks = pdMS_TO_TICKS(MS_PER_SEC/(ulFrequency * 5));
//    TickType_t xLastWakeTime = xTaskGetTickCount();
    uint32_t led_state = GPIO_PIN_OFF;

    for (;;)
    {

        //   vTaskDelayUntil(&xLastWakeTime, xDelayTicks);
        vTaskSuspend(Task1Handle);    //resumed in the callback function
        led_state = !led_state;
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, led_state); // Turn on the LED.

    }

    vTaskDelete(NULL);
}

void vTask2(void* pvParameters)
{
    //uint32_t ulFrequency = *((uint32_t*) pvParameters);

    // Enable the GPIO port that is used for the on-board LED.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);

    // Check if the peripheral access is enabled.
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPION))
    {
    }

    // Enable the GPIO pin for the LED (PN0).  Set the direction as output, and
    // enable the GPIO pin for digital function.
    GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0);

//    const TickType_t xDelayTicks = pdMS_TO_TICKS(MS_PER_SEC/(ulFrequency * 5));
//    TickType_t xLastWakeTime = xTaskGetTickCount();
    uint32_t led_state = GPIO_PIN_OFF;

    for (;;)
    {

        //   vTaskDelayUntil(&xLastWakeTime, xDelayTicks);
        vTaskSuspend(Task2Handle);    //resumed in the callback function
        led_state = !led_state;
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, led_state); // Turn on the LED.

    }

    vTaskDelete(NULL);
}

static void vTimer2hzCallbackFunction(TimerHandle_t xTimer)
{
    vTaskResume(Task1Handle);
}

static void vTimer4hzCallbackFunction(TimerHandle_t xTimer)
{
    vTaskResume(Task2Handle);
}

int main(void)
{

    static const uint32_t ulFrequency2hz = 2;
    static const uint32_t ulFrequency4hz = 4;
    static const TickType_t Ticks2hz = pdMS_TO_TICKS(TICKS_PER_SECOND/ulFrequency2hz) ;
    static const TickType_t Ticks4hz = pdMS_TO_TICKS(TICKS_PER_SECOND/ulFrequency4hz) ;

    BaseType_t ret;
    TimerHandle_t Timer2hz, Timer4hz;

    Timer2hz = xTimerCreate("Timer 2 HZ",
                            Ticks2hz,
                            pdTRUE,
                            0, vTimer2hzCallbackFunction);

    configASSERT(Timer2hz != NULL);
    Timer4hz = xTimerCreate("Timer 4 HZ",
                            Ticks4hz,
                            pdTRUE,
                            0, vTimer4hzCallbackFunction);

    configASSERT(Timer4hz != NULL);

        xTimerStart(Timer2hz, 0);
        xTimerStart(Timer4hz, 0);

    //create Task

    ret = xTaskCreate(vTask1, "Task 2 hz", STACK_DEPTH, (void*) &ulFrequency2hz,
                      1, &Task1Handle);
    configASSERT(ret == pdPASS);

    ret = xTaskCreate(vTask2, "Task 4 hz", STACK_DEPTH, (void*) &ulFrequency4hz,
                      1, &Task2Handle);
    configASSERT(ret == pdPASS);

    //start the scheduler
    vTaskStartScheduler();

    for (;;)
        ;

    return 0;
}

