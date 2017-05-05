#include "mpu9250.h"
#include "console.h"
#include "alarm.h"
#include <math.h>
#include <stdio.h>
#include "pwm.h"
#include "uart.h"

#define M_PI    3.14159265359

int mi = 0;
char xbee_data[3];
char number[3];
uint32_t val= 650 ;

uint8_t dataReceived[100];
char dataArray[100];



//uint32_t duty = 250;



int main(void) {
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
    //InitConsole();
    //Alarms_Init();
    I2C_Init();
    //UartConfig();

    // DEFINE PWM VARIABLES
     volatile uint32_t ui32Load; // Load count for PWM counter
     uint32_t PA7_Z = 365;
     uint32_t PA6_X = 135;
     ui32Load = 5000;


      PwmConfig( ui32Load, PA6_X, PA7_Z);

    // enable Interrupts




       // INITIALIZE PWM
         // PwmConfig(ui32Load, PA6_X, PA7_Z);


   // Initialize UART for XBee communications
    UART_XbeeInit(115200);

    uint8_t id;

    /*Checking connection here*/
    id = MPU_WhoAmI();
    if(id == 0x71){
        /*We are connected set armed alarm*/
       // LED_ArmedAlarm();
    }else{
        /*connection failed somehow, set failed alarm. Check connections*/
        UARTprintf("Connection failed id found: %x", id);
        LED_FailedAlarm();
        while(1);
    }

    /*MPU9250 Simple Initialization*/
    MPU_WritePowerManagement1(CLKSEL_1);
    MPU_WriteConfiguration(DLPF_CFG_3);
    MPU_WriteSampleRateDivider(4);
    MPU_WriteGyroConfiguration(GYRO_FS_SELECT_250);
    MPU_WriteAcceConfiguration(ACCE_FS_SELECT_2G);
    MPU_WriteAcceConfiguration2(A_DLPF_CFG_3);

    float acceleration_bias[3];
    float degree_bias[3];

    /*Calibrate MPU9250 and return bias values*/
    MPU9250_calibrate(acceleration_bias, degree_bias);

    float Acceleration[3];
    float Degrees[3];
    float Temperature;

    float Pitch = 0;
    float Roll = 0;
    float Yaw = 0;
    float Acceleration_Pitch = 0;
    float Acceleration_Roll = 0;
    //float Acceleration_Yaw = 0; /*it will not work magnetometer necessary!*/

    char Pitch_String[16];
    char Roll_String[16];



    /* delay 10 ms */
    uint32_t delay_data = SysCtlClockGet()/100;

    /*Sample rate = System frequency / desired sample frequency*/
    uint32_t Sample_Rate = SysCtlClockGet()/100;

    while(1){
        /*Get data from Gyroscope and Accelerometer*/
        MPU9250_Motion(Acceleration, Degrees);
        MPU9250_Temperature(&Temperature);

        /*Readings minus bias*/
        Acceleration[0] -= acceleration_bias[0];
        Acceleration[1] -= acceleration_bias[1];
        //Acceleration[2] -= acceleration_bias[2];

        Degrees[0] -= degree_bias[0];
        Degrees[1] -= degree_bias[1];
        Degrees[2] -= degree_bias[2];

        Pitch += Degrees[0] * .01;
        Roll += Degrees[1] * .01;
        Yaw += Degrees[2] * .01;

        Acceleration_Pitch = atan2f(Acceleration[1], sqrtf(Acceleration[0] * Acceleration[0] + Acceleration[2] * Acceleration[2]) ) * 180 / M_PI;
        Acceleration_Roll = atan2f(Acceleration[0], sqrt(Acceleration[1] * Acceleration[1] + Acceleration[2] * Acceleration[2]) ) * 180 / M_PI;
        //Acceleration_Yaw = atan2f(Acceleration[2], sqrt(Acceleration[0] * Acceleration[0] + Acceleration[1] * Acceleration[1]) ) * 180 / M_PI;

        Pitch = 0.94 * Pitch + 0.06 * Acceleration_Pitch;
        Roll = 0.94 * Roll + 0.06 * Acceleration_Roll;
        //Yaw = 0.98 * Yaw + 0.02 * Acceleration_Yaw;

       sprintf(Pitch_String, "%f", Pitch);
       sprintf(Roll_String, "%f", Roll);



       // Send MPU data through XBee
          UART_XBeeWriteString(Pitch_String);
          UART_XBeeWriteString(" ");
          UART_XBeeWriteString(Roll_String);
          UART_XBeeWriteString("\n");


       // Receive data from XBee
          UART_XBeeRead(dataReceived, sizeof(dataReceived));
          memcpy(dataArray,dataReceived, sizeof(dataReceived));









        /*delay 10 ms*/
        SysCtlDelay(Sample_Rate);
    }
}






