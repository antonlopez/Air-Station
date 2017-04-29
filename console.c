#include "console.h"

void InitConsole(void){

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);

    GPIOPinConfigure(GPIO_PC6_U3RX);                // Connected to DOUT XBEE --> PC6
    GPIOPinConfigure(GPIO_PC7_U3TX);                // Connected to DIN XBEE  --> PC7

    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART3);

    UARTClockSourceSet(UART3_BASE, UART_CLOCK_PIOSC);

    GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_6 | GPIO_PIN_7);

    UARTStdioConfig(2, 115200, 16000000);
}






