
/* 
* This file is part of VL53L1 Platform 
* 
* Copyright (c) 2016, STMicroelectronics - All Rights Reserved 
* 
* License terms: BSD 3-clause "New" or "Revised" License. 
* 
* Redistribution and use in source and binary forms, with or without 
* modification, are permitted provided that the following conditions are met: 
* 
* 1. Redistributions of source code must retain the above copyright notice, this 
* list of conditions and the following disclaimer. 
* 
* 2. Redistributions in binary form must reproduce the above copyright notice, 
* this list of conditions and the following disclaimer in the documentation 
* and/or other materials provided with the distribution. 
* 
* 3. Neither the name of the copyright holder nor the names of its contributors 
* may be used to endorse or promote products derived from this software 
* without specific prior written permission. 
* 
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE 
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
* 
*/

#include "vl53l1_platform.h"
#include <string.h>
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <unistd.h>
#include <errno.h>

static int i2c_write(int fd, uint16_t index, uint8_t * data, uint8_t len){

    uint8_t buf[BUFSIZ];
    if (len > BUFSIZ - 2){
        printf("i2c_write: data size over.\n");
        return 1;
    }
    buf[0] = index >> 8;
    buf[1] = index & 0xff;
    memcpy(buf + 2, data, len);
    if (write(fd, buf, len + 2) != len + 2) {
        printf("i2c_write: failed to write.\n");
        return 1;
    }
    return 0;
}

static int i2c_read(int fd, uint16_t index, uint8_t * data, uint8_t len){

    uint8_t buf[BUFSIZ];
    buf[0] = index >> 8;
    buf[1] = index & 0xff;

    if (write(fd, buf, 2) != 2) {
        printf("i2c_read: failed to write.\n");
        return 1;
    }

    if (read(fd, buf, len) != len) {
        printf("i2c_read: failed to read.\n");
        return 1;
    }
    memcpy(data, buf, len);

    return 0;
}


int8_t VL53L1_WriteMulti( uint16_t dev, uint16_t index, uint8_t *pdata, uint32_t count) {
    return i2c_write(dev, index, pdata, count);
}

int8_t VL53L1_ReadMulti(uint16_t dev, uint16_t index, uint8_t *pdata, uint32_t count){
    return i2c_write(dev, index, pdata, count);
}

int8_t VL53L1_WrByte(uint16_t dev, uint16_t index, uint8_t data) {
	return i2c_write(dev, index, &data, 1);
}

int8_t VL53L1_WrWord(uint16_t dev, uint16_t index, uint16_t data) {
    uint8_t buf[4];
    buf[1] = data>>0&0xFF;
    buf[0] = data>>8&0xFF;
    return i2c_write(dev, index, buf, 2);
}

int8_t VL53L1_WrDWord(uint16_t dev, uint16_t index, uint32_t data) {
    uint8_t buf[4];
    buf[3] = data>>0&0xFF;
    buf[2] = data>>8&0xFF;
    buf[1] = data>>16&0xFF;
    buf[0] = data>>24&0xFF;
    return i2c_write(dev, index, buf, 4);
}

int8_t VL53L1_RdByte(uint16_t dev, uint16_t index, uint8_t *data) {
    uint8_t tmp = 0;
    int ret = i2c_read(dev, index, &tmp, 1);
    *data = tmp;
    //printf("%u\n", tmp);
    return ret;
}

int8_t VL53L1_RdWord(uint16_t dev, uint16_t index, uint16_t *data) {
    uint8_t buf[2];
    int ret = i2c_read(dev, index, buf, 2);
    uint16_t tmp = 0;
    tmp |= buf[1]<<0;
    tmp |= buf[0]<<8;
    // printf("%u\n", tmp);
    *data = tmp;
    return ret;
}

int8_t VL53L1_RdDWord(uint16_t dev, uint16_t index, uint32_t *data) {
    uint8_t buf[4];
    int ret = i2c_read(dev, index, buf, 4);
    uint32_t tmp = 0;
    tmp |= buf[3]<<0;
    tmp |= buf[2]<<8;
    tmp |= buf[1]<<16;
    tmp |= buf[0]<<24;
    *data = tmp;
    // printf("%zu\n", tmp);
    return ret;
}

int8_t VL53L1_WaitMs(uint16_t dev, int32_t wait_ms){
    struct timespec wait;
    int ret = EINTR;
  
    wait.tv_sec  = wait_ms / 1000;
    wait.tv_nsec = (wait_ms % 1000) * 1000 * 1000; 
    while(ret == EINTR) ret = clock_nanosleep(CLOCK_MONOTONIC, 0, &wait, &wait); 
    if(ret != 0){
        printf("%s\n", __FUNCTION__);
        return 1;
    }
	return 0;
}

int VL53L1_i2c_init(char * devPath, int devAddr)
{
    int file;
    if ((file = open(devPath, O_RDWR)) < 0) {
        /* ERROR HANDLING: you can check errno to see what went wrong */
        perror("Failed to open the i2c bus");
        return -1;
    }
    if (ioctl(file, I2C_SLAVE, devAddr) < 0) {
        printf("Failed to acquire bus access and/or talk to slave.\n");
        /* ERROR HANDLING; you can check errno to see what went wrong */
        return -1;
    }
    return file;
}

int8_t VL53L0X_i2c_close(void)
{
    printf("%s\n", __FUNCTION__);
    return 1;
}


