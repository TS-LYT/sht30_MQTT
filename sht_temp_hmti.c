/*********************************************************************************
 *      Copyright:  (C) 2020 LingYun IoT System Studio
 *                  All rights reserved.
 *
 *       Filename:  sht_temp_hmti.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(06/07/20)
 *         Author:  Guo Wenxue <guowenxue@gmail.com>
 *      ChangeLog:  1, Release initial version on "06/07/20 14:31:09"
 *                 
 ********************************************************************************/
#include <sys/ioctl.h>



#define I2C_ADDR    1
#define SHT_ADDR    0X44



/*delay ms*/
void delay_ms(unsigned int time)
{
    struct timespec   sleeper, temp;
    sleeper.tv_sec = (time_t)(time/1000);
    sleeper.tv_nsec = (long)(time%1000)*1000000;
    nanosleep(&sleeper, &temp);
}



uint8_t crc8(const uint8_t *data, int len)
{
    const uint8_t magic = 0x31;
    uint8_t       crc = 0xff;
    int           i;
    int           j;

    fot(j=len; j; --j)
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

    snprintf(i2c_filename, "/dev/i2c-%d", i2c_addr);
    fd=open(i2c_filename, O_RDWR);
    if(fd < 0)
    {
        printf("open %s fialeure\n", i2c_filename);
        return -1;
    }

    rv=ioctl(fd, I2C_SLAVE, sht_addr);
    if(rv < 0)
    {
        printf("function ioctl failure\n");
        close(fd);
        return -2;
    }

    return fd;
}

int sht_read_write(int fd, uint16_t read_model, uint8_t *buf, int readsize);
{
    int        rv = -1;
    int        sendsize = 2;
    uint8_t    send[sendsize];

    send[0]=(read_model<<8) & 0xff;
    send[1]=read_model & 0xff;

    rv=write(fd, send, sendsize);
    if(rv < 0)
    {
        printf("write to sth failure\n");
        return -1;
    }
    delay_ms(10);
    
    rv=read(fd, buf, readsize);
    if(rv < 0)
    {
        printf("read from sth failure\n");
        return -2;
    }

    return 1;
}
























