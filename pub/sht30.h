/********************************************************************************
 *      Copyright:  (C) 2020 longyongtu<longyongtu13@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  sht_temp_hmti.h
 *    Description:  This head file 
 *
 *        Version:  1.0.0(06/07/20)
 *         Author:  longyongtu <longyongtu13@qq.com>
 *      ChangeLog:  1, Release initial version on "06/07/20 14:31:18"
 *                 
 ********************************************************************************/
#ifndef    _SHT30_H
#define    _SHT30_H

#include <stdio.h>
#include <string.h>
#include <linux/i2c-dev.h>
#include <time.h>
#include <sys/ioctl.h>
#include <time.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c.h>


#define I2C_ADDR            1
#define SHT_ADDR            0X44
#define SHT_MEAS_MEDREP     0x240b
#define SHT_MEAS_LOWREP     0x2416


void delay_ms(unsigned int time);
uint8_t crc8(const uint8_t *data, int len);
int sht_open(int i2c_addr, uint8_t sht_addr);
int sht_read_write(int fd, int sht_addr, uint16_t read_model, uint8_t *buf, int readsize);
int get_temp_hmti(int i2c_addr, uint8_t sht_addr,  float *temp, float *hmti);


#endif
