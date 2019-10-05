#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include "VL53L1X_api.h"

int main(void)
{
	
  uint16_t	dev;
  int status=0;
  uint8_t byteData, sensorState=0;
  uint16_t wordData;
  uint16_t Distance;
  uint16_t SignalRate;
  uint16_t AmbientRate;
  uint16_t SpadNum; 
  uint8_t RangeStatus;
  uint8_t dataReady;

  dev = VL53L1_i2c_init("/dev/i2c-1", 0x29);
  if(dev < 0){
    printf("i2c device open error\n");
    exit -1;
  }

  status = VL53L1_RdByte(dev, 0x010F, &byteData);
  if(status != 0) printf("%d\n", status);
  printf("VL53L1X Model_ID: %X\n", byteData);
  status = VL53L1_RdByte(dev, 0x0110, &byteData);
  if(status != 0) printf("%d\n", status);
  printf("VL53L1X Module_Type: %X\n", byteData);
  status = VL53L1_RdWord(dev, 0x010F, &wordData);
  if(status != 0) printf("%d\n", status);
  printf("VL53L1X: %X\n", wordData);
  while(sensorState==0){
		status = VL53L1X_BootState(dev, &sensorState);
        if(status != 0) 
    usleep(500 * 1000);
  }
  printf("Chip booted\n");

  status = VL53L1X_SensorInit(dev);
  status = VL53L1X_SetDistanceMode(dev, 2); /* 1=short, 2=long */
  status = VL53L1X_SetTimingBudgetInMs(dev, 100); /* in ms possible values [20, 50, 100, 200, 500] */
  status = VL53L1X_SetInterMeasurementInMs(dev, 100); /* in ms, IM must be > = TB */
  printf("VL53L1X Ultra Lite Driver Example running ...\n");
  status = VL53L1X_StartRanging(dev);   /* This function has to be called to enable the ranging */
  while(1){ /* read and display data */
	  while (dataReady == 0){
		  status = VL53L1X_CheckForDataReady(dev, &dataReady);
          usleep(500 * 1000);
	  }
	  dataReady = 0;
	  status = VL53L1X_GetRangeStatus(dev, &RangeStatus);
	  status = VL53L1X_GetDistance(dev, &Distance);
	  status = VL53L1X_GetSignalRate(dev, &SignalRate);
	  status = VL53L1X_GetAmbientRate(dev, &AmbientRate);
	  status = VL53L1X_GetSpadNb(dev, &SpadNum);
	  status = VL53L1X_ClearInterrupt(dev); /* clear interrupt has to be called to enable next interrupt*/
	  printf("%u, %u, %u, %u, %u\n", RangeStatus, Distance, SignalRate, AmbientRate, SpadNum);
  }
}
