#include "uart.h"
#include "mpu9250.h"
#include "console.h"
#include "alarm.h"
#include <math.h>
#include <stdio.h>
#include "pwm.h"

#define M_PI    3.14159265359
float float_test = 2.11;
uint8_t bytes_Pitch[4];
uint8_t bytes_Roll[4];

void float2Bytes(float val,uint8_t* bytes_array);
void UART_XBeeWriteBytes(uint8_t *data, uint32_t size);

int main(void) {

    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    UART_XbeeInit(115200);

   // UART_XBeeWriteString("Hello from XBee\n");
   // UART_XBeeWriteUint(12674);
   // UART_XBeeWriteFloat(3.1415);


       // InitConsole();
        //Alarms_Init();
        I2C_Init();
        PwmConfig();





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

        /* UART_data */
        char identifier;
        uint8_t dataReceived[4];
        float duty;
       // UART_XBeeWriteString("C"); // handshake
        char handshake;


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




           UART_XBeeRead(dataReceived, sizeof(dataReceived));
           memcpy(&identifier,&dataReceived, sizeof(dataReceived));
           memset(&dataReceived[0], 0, sizeof(dataReceived));
           UART_XBeeRead(dataReceived, sizeof(dataReceived));
           memcpy(&duty,&dataReceived, sizeof(dataReceived));











            float2Bytes(Pitch, bytes_Pitch);

            UART_XBeeWriteString("P");

            UART_XBeeWriteBytes(bytes_Pitch, sizeof(bytes_Pitch));



            //UART_XBeeWriteString("R");
            //UART_XBeeWriteBytes(bytes_Roll, sizeof(bytes_Roll));









            /*delay 10 ms*/
            SysCtlDelay(Sample_Rate);
        }








}


void float2Bytes(float val,uint8_t* bytes_array){
  // Create union of shared memory space
  union {
    float float_variable;
    uint8_t temp_array[4];
  } u;
  // Overite bytes of union with float variable
  u.float_variable = val;
  // Assign bytes to input array
  memcpy(bytes_array, u.temp_array, 4);
}


