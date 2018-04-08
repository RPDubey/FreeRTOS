//*****************************************************************************
// File:Main.c
// Implements the requirements for prob2, HW5
// @authorRavi Prakash Dubey
// Date:6th April, 2018
// uart_echo.c - Example has been leveraged for this problem
//
//*****************************************************************************

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"

//****************************************************************************
//
// System clock rate in Hz.
//
//****************************************************************************
uint32_t g_ui32SysClock;

//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}
#endif

//convert integer to ascii
#define ITOA            digit_count = 0;\
                        quo = count;\
                        do{\
                        rem = quo%10;\
                        quo = (quo - rem)/10;\
                        buffer[6-digit_count] = rem+48;\
                        digit_count++;\
                        }while(quo);\
                        buffer[6-digit_count] =' ';\


#define GPIO_PIN_ON     1
#define GPIO_PIN_OFF    0



// removed the  the UART Interrupt handler

//*****************************************************************************
//
// Send a string to the UART.
//
//*****************************************************************************
void UARTSend(const uint8_t *pui8Buffer, uint32_t ui32Count)
{
    //
    // Loop while there are more characters to send.
    //
    while (ui32Count--)
    {
        //
        // Write the next character to the UART.
        //
        ROM_UARTCharPutNonBlocking(UART0_BASE, *pui8Buffer++);
    }
}

int main(void)
{
    //
    // Set the clocking to run directly from the crystal at 120MHz.
    //
    g_ui32SysClock = MAP_SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
    SYSCTL_OSC_MAIN |
    SYSCTL_USE_PLL |
    SYSCTL_CFG_VCO_480),
                                            120000000);
    //
    // Enable the GPIO port that is used for the on-board LED.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);

    //
    // Enable the GPIO pins for the LED (PN0).
    //
    GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0);

    //
    // Enable the peripherals used by this example.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    //
    // Enable processor interrupts.
    //
    IntMasterEnable();

    //
    // Set GPIO A0 and A1 as UART pins.
    //
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //
    // Configure the UART for 115,200, 8-N-1 operation.
    //
    UARTConfigSetExpClk(UART0_BASE, g_ui32SysClock, 115200,
                            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                            UART_CONFIG_PAR_NONE));

    //
    // Enable the UART interrupt.
    //
    IntEnable(INT_UART0);
    UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);


    UARTSend((uint8_t *) "\nProject for:", 13);
    SysCtlDelay(g_ui32SysClock / (1000 * 3));

    UARTSend((uint8_t *) " Ravi Dubey,", 12);
    SysCtlDelay(g_ui32SysClock / (1000 * 3));
    UARTSend((uint8_t *) " 4th April. ", 11);

    uint16_t quo, count = 0;
    uint8_t rem, digit_count;
    char buffer[6];

    while (1)
    {
        //Turn on the LED
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, GPIO_PIN_ON);

        // Delay for 500 millisecond for 2 HZ blink frequency.
        SysCtlDelay(g_ui32SysClock / (3 * 2));

        count++;
        ITOA
        ;
        UARTSend((uint8_t *) &buffer[6 - digit_count], digit_count + 1);

        // Turn off the LED
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, GPIO_PIN_OFF);

        // Delay for 500 millisecond for 2 HZ blink frequency.
        SysCtlDelay(g_ui32SysClock / (3 * 2));

        count++;
        ITOA
        ;
        UARTSend((uint8_t *) &buffer[6 - digit_count], digit_count + 1);

    }
}
