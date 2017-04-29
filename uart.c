#include "uart.h"
#include "pwm.h"


	void UartConfig(void){



	/*	//Set up UART0 for debug
	    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	    GPIOPinConfigure(GPIO_PA0_U0RX);
	    GPIOPinConfigure(GPIO_PA1_U0TX);
	    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);




	     // set up UART3 for the xbee
	     SysCtlPeripheralEnable(SYSCTL_PERIPH_UART3);
	     SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	     GPIOPinConfigure(GPIO_PC6_U3RX);                // Connected to DOUT XBEE --> PC6
	     GPIOPinConfigure(GPIO_PC7_U3TX);                // Connected to DIN XBEE  --> PC7
	     GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_6 | GPIO_PIN_7);


	   /*  UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
	    UARTConfigSetExpClk(UART1_BASE, SysCtlClockGet(), 115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));*/
	    UARTConfigSetExpClk(UART3_BASE, SysCtlClockGet(), 115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));



	    // enable Interrupts
	    IntMasterEnable();

		IntEnable(INT_UART3);
		UARTIntEnable(UART3_BASE, UART_INT_RX | UART_INT_RT);




/*
	    UARTCharPut(UART3_BASE, 'c');
	    UARTCharPut(UART3_BASE, 'o');
	    UARTCharPut(UART3_BASE, 'n');
	    UARTCharPut(UART3_BASE, 'n');
	    UARTCharPut(UART3_BASE, 'e');
	    UARTCharPut(UART3_BASE, 'c');
	    UARTCharPut(UART3_BASE, 't');
	    UARTCharPut(UART3_BASE, 'e');
	    UARTCharPut(UART3_BASE, 'd');
	    UARTCharPut(UART3_BASE, ' ');

*/




	}
