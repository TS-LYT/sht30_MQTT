/*********************************************************************************
 *      Copyright:  (C) 2020 longyongtu<longyongtu13@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  sht_temp_hmti.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(06/07/20)
 *         Author:  longyongtu <longyongtu13@qq.com>
 *      ChangeLog:  1, Release initial version on "06/07/20 14:31:09"
 *                 
 ********************************************************************************/
#include "sht_temp_hmti.h"


#define SHT31D_FIX_INITIAL_FAIL 0

/*delay ms*/
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

    rv=ioctl(fd, I2C_SLAVE, sht_addr);
    if(rv < 0)
    {
        printf("function ioctl failure\n");
        close(fd);
        return -2;
    }
#ifdef SHT31D_FIX_INITIAL_FAIL
    uint8_t buf = 0x00;
    if (write(fd, &buf, 1) != 1) {}
    if ( read(fd, &buf, 1) != 1) {}
#endif

    return fd;
}

int sht_read_write(int fd, uint16_t read_model, uint8_t *buf, int readsize)
{
    int        rv = -1;
    int        sendsize = 2;
    uint8_t    send[sendsize];

    send[0]=(read_model>>8) & 0xff;
    send[1]=read_model & 0xff;

    rv=write(fd, send, sendsize);
    if(rv < 0)
    {
        printf("ERROR sending command  :- %s\n", strerror (errno));
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

int get_temp_hmti(int sht_fd, float *temp, float *hmti)
{
    int             rv = -1;
    uint8_t         buf[10];
    uint16_t        tmp_t, tmp_h;

    memset(buf, 0, sizeof(buf));
    rv=sht_read_write(sht_fd, SHT_MEAS_LOWREP, buf, 6);
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

    return 1;
}


int main()
{
    float temp, hmti;
    int  sht_fd;

    sht_fd=sht_open(I2C_ADDR, SHT_ADDR);
    //printf("%d\n", sht_fd);
    get_temp_hmti(sht_fd, &temp, &hmti);
    printf("Temperature %.2fc\n", temp);
    printf("Humidity %.2f%%\n", hmti);

    return 0;
}













