/*********************************************************************************
 *      Copyright:  (C) 2020 longyongtu<longyongtu13@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  d_sht_temp_hmti.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(12/07/20)
 *         Author:  longyongtu <longyongtu13@qq.com>
 *      ChangeLog:  1, Release initial version on "12/07/20 14:45:20"
 *                 
 ********************************************************************************/

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


/* delay ms*/
void delay_ms(unsigned int time)
{
    struct timespec   sleeper, temp;
    sleeper.tv_sec = (time_t)(time/1000);
    sleeper.tv_nsec = (long)(time%1000)*1000000;
    nanosleep(&sleeper, &temp);
    return ;
}



uint8_t crc8(const uint8_t *data, int len)
{
    const uint8_t magic = 0x31;
    uint8_t       crc = 0xff;
    int           i;
    int           j;

    for(j=len; j; --j)
    {
        crc ^= *data++;
        for(i=8; i; --i)
        {
            crc=(crc & 0x80) ? (crc << 1) ^ magic : (crc << 1);
        }
    }
    return crc;
}


int sht_open(int i2c_addr, uint8_t sht_addr)
{
    char    i2c_filename[10];
    int     fd = -1;
    int     rv = -1;

    snprintf(i2c_filename, 19, "/dev/i2c-%d", i2c_addr);
    fd=open(i2c_filename, O_RDWR);
    if(fd < 0)
    {
        printf("open %s fialeure\n", i2c_filename);
        return -1;
    }

    return fd;
}

int sht_read_write(int fd, int sht_addr, uint16_t read_model, uint8_t *buf, int readsize)
{
    int                        rv = -1;
    int                        sendsize = 2;
    uint8_t                    send[2];
    struct i2c_msg             msgs;
    struct i2c_rdwr_ioctl_data data;


    send[0]=(read_model>>8) & 0xff;
    send[1]=read_model & 0xff;

    data.nmsgs = 1;//消息的数目
    msgs.len = sendsize;//写入目标的字节数
    msgs.addr = sht_addr;//i2c设备地址  
    msgs.flags = 0;//flags为0:表示写;为1:表示读 
    msgs.buf = send;//发送的数据
    data.msgs = &msgs;
    rv=ioctl(fd, I2C_RDWR, &data);
    if(rv < 0)
    {
        printf("write to sht_30 failure\n");
        return -1;
    }

    delay_ms(10);

    data.nmsgs =1;
    msgs.len = readsize;
    msgs.addr = sht_addr;
    msgs.flags = 1;
    msgs.buf = buf;
    data.msgs = &msgs;
    rv=ioctl(fd, I2C_RDWR, &data);
    if(rv < 0)
    {
        printf("read to sht_30 failure\n");
        return -1;
    }
    return 1;
}

int get_temp_hmti(int i2c_addr, uint8_t sht_addr,  float *temp, float *hmti)
{
    int             rv = -1;
    int             sht_fd = -1;
    uint8_t         buf[10];
    uint16_t        tmp_t, tmp_h;

    memset(buf, 0, sizeof(buf));

    sht_fd=sht_open(I2C_ADDR, SHT_ADDR);
    if(sht_fd < 0)
    {
        return -1;
    }

    rv=sht_read_write(sht_fd, sht_addr, SHT_MEAS_LOWREP, buf, 6);
    if(rv < 0)
    {
        return -1;
    }

    if(buf[2] != crc8(buf, 2) || buf[5] != crc8(buf+3, 2))
    {
        printf("crc check errno\n");
        return -1;
    }

    tmp_t = buf[0];
    tmp_t <<= 8;
    tmp_t |= buf[1];

    tmp_h = buf[3];
    tmp_h <<= 8;
    tmp_h |= buf[4];


    *temp = -45.0 + (175.0 * ((float) tmp_t / (float) 0xFFFF));
    *hmti = 100.0 * ((float) tmp_h / (float) 0xFFFF);

    close(sht_fd);
    return 1;
}






int main(int argc, char *argv[])
{
    float temp, hmti;
    get_temp_hmti(I2C_ADDR, SHT_ADDR, &temp, &hmti);
    printf("Temperature %.2fc\n", temp);
    printf("Humidity %.2f%%\n", hmti);
    return 0;
}
