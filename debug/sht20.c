/*********************************************************************************
 *      Copyright:  (C) 2020 longyongtu (longyongtu13@qq.com)
 *                  All rights reserved.
 *
 *       Filename:  sht20.c
 *    Description:  This file is temperature and relative humidity sensor SHT20 code
 *                 
 *        Version:  1.0.0(2018/7/05)
 *         Author:  long yongtu <longyongtu13@gmail.com>
 *      ChangeLog:  1, Release initial version on "2020/7/05 12:13:26"
 *                 
 ********************************************************************************/

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <sys/stat.h>
#include <linux/i2c-dev.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <errno.h>
#include <string.h>


#define SOFTRESET                        0xFE
#define TRIGGER_TEMPERATURE_NO_HOLD      0xF3
#define TRIGGER_HUMIDITY_NO_HOLD         0xF5

//#define I2C_API_IOCTL  /* Use I2C userspace driver ioctl API */
#define I2C_API_RDWR /* Use I2C userspace driver read/write API */

static inline void msleep(unsigned long ms);
static inline void dump_buf(const char *prompt, uint8_t *buf, int size);
int sht2x_init(void);
int sht2x_get_serialnumber(int fd, uint8_t *serialnumber, int size);
int sht2x_get_temp_humidity(int fd, float *temp, float *rh);
 
int main(int argc, char **argv)
{
       	int          fd;
	float        temp;
	float        rh;
	uint8_t      serialnumber[8];

	fd = sht2x_init();
	if(fd < 0)
	{
		printf("SHT2x initialize failure\n");
		return 1;
	}

	if( sht2x_get_serialnumber(fd, serialnumber, 8) < 0)
	{
		printf("SHT2x get serial number failure\n");
		return 3;
	}

	if( sht2x_get_temp_humidity(fd, &temp, &rh) < 0 )
	{
		printf("SHT2x get get temperature and relative humidity failure\n");
		return 3;
	}

	printf("Temperature=%lf ℃ relative humidity=%lf%\n", temp, rh);

	close(fd);
}


static inline void msleep(unsigned long ms) 
{
       	struct timespec cSleep;
       	unsigned long ulTmp;
       
	cSleep.tv_sec = ms / 1000;
       	if (cSleep.tv_sec == 0)
       	{   
		ulTmp = ms * 10000;
	       	cSleep.tv_nsec = ulTmp * 100;
       	}   
	else
       	{   
		cSleep.tv_nsec = 0;
       	}   
	
	nanosleep(&cSleep, 0); 
}

static inline void dump_buf(const char *prompt, uint8_t *buf, int size)
{
	int          i;

	if( !buf )
	{
		return ;
	}

	if( prompt )
	{
		printf("%s ", prompt);
	}

	for(i=0; i<size; i++)
	{
		printf("%02x ", buf[i]);
	}
	printf("\n");

	return ;
}



#ifdef I2C_API_RDWR  /* Use I2C userspace driver read/write API */

int sht2x_softreset(int fd)
{
	uint8_t           buf[4];

	if( fd<0 )
	{
		printf("%s line [%d] %s() get invalid input arguments\n", __FILE__, __LINE__, __func__ );
		return -1;
	}

	/* software reset SHT2x */
	memset(buf, 0, sizeof(buf));

	buf[0] = SOFTRESET;
	write(fd, buf, 1);

	msleep(50);

	return 0;
}

int sht2x_init(void)
{
	int     fd;
	
	if( (fd=open("/dev/i2c-1", O_RDWR)) < 0)
	{
		printf("i2c device open failed: %s\n", strerror(errno));
	       	return -1;
	}
	
	/* set I2C mode and SHT2x slave address */
	ioctl(fd, I2C_TENBIT, 0);    /* Not 10-bit but 7-bit mode */
	ioctl(fd, I2C_SLAVE, 0x40); /* set SHT2x slava address 0x40*/

	if( sht2x_softreset(fd) < 0 )
	{
		printf("SHT2x softreset failure\n");
		return -2;
	}

	return fd;
}

int sht2x_get_temp_humidity(int fd, float *temp, float *rh)
{
	uint8_t           buf[4];

	if( fd<0 || !temp || !rh )
	{
		printf("%s line [%d] %s() get invalid input arguments\n", __FILE__, __LINE__, __func__ );
		return -1;
	}

	/* send trigger temperature measure command and read the data */
	memset(buf, 0, sizeof(buf));
	buf[0]=TRIGGER_TEMPERATURE_NO_HOLD;
	write(fd, buf, 1);

	msleep(85); /* datasheet: typ=66, max=85 */

	memset(buf, 0, sizeof(buf));
	read(fd, buf, 3);
	dump_buf("Temperature sample data: ", buf, 3);
	*temp = 175.72 * (((((int) buf[0]) << 8) + buf[1]) / 65536.0) - 46.85;
	
	/* send trigger humidity measure command and read the data */
	memset(buf, 0, sizeof(buf));
	buf[0] = TRIGGER_HUMIDITY_NO_HOLD;	
	write(fd, buf, 1);

	msleep(29); /* datasheet: typ=22, max=29 */
	memset(buf, 0, sizeof(buf));

	read(fd, buf, 3);
	dump_buf("Relative humidity sample data: ", buf, 3);
	*rh = 125 * (((((int) buf[0]) << 8) + buf[1]) / 65536.0) - 6;

	return 0;
}

int sht2x_get_serialnumber(int fd, uint8_t *serialnumber, int size)
{
	uint8_t           buf[4];

	if( fd<0 || !serialnumber || size!=8 )
	{
		printf("%s line [%d] %s() get invalid input arguments\n", __FILE__, __LINE__, __func__ );
		return -1;
	}

	/* Read SerialNumber from Location 1 */
	memset(buf, 0, sizeof(buf));
	buf[0] = 0xfa;  /* command for readout on-chip memory */
	buf[1] = 0x0f;  /* on-chip memory address */
	write(fd, buf, 2);

	memset(buf, 0, sizeof(buf));
	read(fd, buf, 4);

	serialnumber[5]=buf[0]; /* Read SNB_3 */
	serialnumber[4]=buf[1]; /* Read SNB_2 */
	serialnumber[3]=buf[2]; /* Read SNB_1 */
	serialnumber[2]=buf[3]; /* Read SNB_0 */
	
	/* Read SerialNumber from Location 2 */
	memset(buf, 0, sizeof(buf) );
	buf[0]=0xfc;  /* command for readout on-chip memory */
	buf[1]=0xc9;  /* on-chip memory address */
	write(fd, buf, 2);

	memset(buf, 0, sizeof(buf) );
	read(fd, buf, 4);

	serialnumber[1]=buf[0]; /* Read SNC_1 */
	serialnumber[0]=buf[1]; /* Read SNC_0 */
	serialnumber[7]=buf[2]; /* Read SNA_1 */
	serialnumber[6]=buf[3]; /* Read SNA_0 */

	dump_buf("SHT2x Serial number: ", serialnumber, 8);

	return 0;
}

#elif (defined I2C_API_IOCTL)  /* Use I2C userspace driver read/write API */

int sht2x_softreset(int fd)
{
       	struct i2c_msg               msg;
	struct i2c_rdwr_ioctl_data   sht2x_data;
	uint8_t                      buf[2];


	if( fd<0 )
	{
		printf("%s line [%d] %s() get invalid input arguments\n", __FILE__, __LINE__, __func__ );
		return -1;
	}

	msg.addr= 0x40; 
	msg.flags=0; //write
	msg.len= 1; 
	msg.buf= buf; 
	msg.buf[0]=SOFTRESET;

	sht2x_data.nmsgs= 1; 
	sht2x_data.msgs= &msg;

	if( ioctl(fd, I2C_RDWR, &sht2x_data) < 0 )
	{
		printf("%s() ioctl failure: %s\n", __func__, strerror(errno));
		return -2;
	}

	msleep(50);

	return 0;
}


int sht2x_init(void)
{
	int     fd;
	
	if( (fd=open("/dev/i2c-1", O_RDWR)) < 0)
	{
		printf("i2c device open failed: %s\n", strerror(errno));
	       	return -1;
	}

	if( sht2x_softreset(fd) < 0 )
	{
		printf("SHT2x softreset failure\n");
		return -2;
	}

	return fd;
}


int sht2x_get_serialnumber(int fd, uint8_t *serialnumber, int size)
{
       	struct i2c_msg               msgs[2];
	struct i2c_rdwr_ioctl_data   sht2x_data;
	uint8_t                      sbuf[2];
	uint8_t                      rbuf[4];

	if( fd<0 )
	{
		printf("%s line [%d] %s() get invalid input arguments\n", __FILE__, __LINE__, __func__ );
		return -1;
	}

	
	/*+------------------------------------------+
	 *|     Read SerialNumber from Location 1    |
	 *+------------------------------------------+*/

	msgs[0].addr= 0x40; 
	msgs[0].flags=0; //write
	msgs[0].len= 2; 
	msgs[0].buf= sbuf; 
	msgs[0].buf[0]=0xfa;  /* command for readout on-chip memory */
	msgs[0].buf[1]=0x0f;  /* on-chip memory address */
	
	msgs[1].addr=0x40; 
	msgs[1].flags=I2C_M_RD; //write
	msgs[1].len= 4; 
	msgs[1].buf= rbuf; 

	sht2x_data.nmsgs= 2; 
	sht2x_data.msgs= msgs;

	if( ioctl(fd, I2C_RDWR, &sht2x_data) < 0 )
	{
		printf("%s() ioctl failure: %s\n", __func__, strerror(errno));
		return -2;
	}
	
	serialnumber[5]=rbuf[0]; /* Read SNB_3 */
       	serialnumber[4]=rbuf[1]; /* Read SNB_2 */
       	serialnumber[3]=rbuf[2]; /* Read SNB_1 */
       	serialnumber[2]=rbuf[3]; /* Read SNB_0 */


	/*+------------------------------------------+
	 *|     Read SerialNumber from Location 2    |
	 *+------------------------------------------+*/
	
	msgs[0].addr= 0x40; 
	msgs[0].flags=0; //write
	msgs[0].len= 2; 
	msgs[0].buf= sbuf; 
	msgs[0].buf[0]=0xfc;  /* command for readout on-chip memory */
	msgs[0].buf[1]=0xc9;  /* on-chip memory address */
	
	msgs[1].addr=0x40; 
	msgs[1].flags=I2C_M_RD; //write
	msgs[1].len= 4; 
	msgs[1].buf= rbuf; 

	sht2x_data.nmsgs= 2; 
	sht2x_data.msgs= msgs;

	if( ioctl(fd, I2C_RDWR, &sht2x_data) < 0 )
	{
		printf("%s() ioctl failure: %s\n", __func__, strerror(errno));
		return -2;
	}
	
	serialnumber[1]=rbuf[0]; /* Read SNC_1 */
	serialnumber[0]=rbuf[1]; /* Read SNC_0 */
	serialnumber[7]=rbuf[2]; /* Read SNA_1 */
	serialnumber[6]=rbuf[3]; /* Read SNA_0 */

	dump_buf("SHT2x Serial number: ", serialnumber, 8);

	return 0;
}


int sht2x_get_temp_humidity(int fd, float *temp, float *rh)
{
       	struct i2c_msg               msg;
	struct i2c_rdwr_ioctl_data   sht2x_data;
	uint8_t                      buf[4];

	if( fd<0 )
	{
		printf("%s line [%d] %s() get invalid input arguments\n", __FILE__, __LINE__, __func__ );
		return -1;
	}

	
	/*+------------------------------------------+
	 *|       measure and get temperature        |
	 *+------------------------------------------+*/

	msg.addr= 0x40; 
	msg.flags=0; //write
	msg.len= 1; 
	msg.buf= buf; 
	msg.buf[0]=TRIGGER_TEMPERATURE_NO_HOLD;  /* trigger temperature without hold I2C bus */

	sht2x_data.nmsgs= 1; 
	sht2x_data.msgs= &msg;

	if( ioctl(fd, I2C_RDWR, &sht2x_data) < 0 )
	{
		printf("%s() ioctl failure: %s\n", __func__, strerror(errno));
		return -2;
	}

	msleep(85);
	
	memset(buf, 0, sizeof(buf));
	msg.addr=0x40; 
	msg.flags=I2C_M_RD; //write
	msg.len= 3; 
	msg.buf= buf; 

	sht2x_data.nmsgs= 1; 
	sht2x_data.msgs= &msg;

	if( ioctl(fd, I2C_RDWR, &sht2x_data) < 0 )
	{
		printf("%s() ioctl failure: %s\n", __func__, strerror(errno));
		return -2;
	}
	
	//dump_buf("Temperature sample data: ", buf, 3);
	*temp = 175.72 * (((((int) buf[0]) << 8) + buf[1]) / 65536.0) - 46.85;

	
	/*+------------------------------------------+
	 *|    measure and get relative humidity     |
	 *+------------------------------------------+*/

	msg.addr= 0x40; 
	msg.flags=0; //write
	msg.len= 1; 
	msg.buf= buf; 
	msg.buf[0]=TRIGGER_HUMIDITY_NO_HOLD;  /* trigger humidity without hold I2C bus */

	sht2x_data.nmsgs= 1; 
	sht2x_data.msgs= &msg;

	if( ioctl(fd, I2C_RDWR, &sht2x_data) < 0 )
	{
		printf("%s() ioctl failure: %s\n", __func__, strerror(errno));
		return -2;
	}

	msleep(29);
	
	memset(buf, 0, sizeof(buf));
	msg.addr=0x40; 
	msg.flags=I2C_M_RD; //write
	msg.len= 3; 
	msg.buf= buf; 

	sht2x_data.nmsgs= 1; 
	sht2x_data.msgs= &msg;

	if( ioctl(fd, I2C_RDWR, &sht2x_data) < 0 )
	{
		printf("%s() ioctl failure: %s\n", __func__, strerror(errno));
		return -2;
	}

	//dump_buf("Relative humidity sample data: ", buf, 3);
       	*rh = 125 * (((((int) buf[0]) << 8) + buf[1]) / 65536.0) - 6;
	
	return 0;
}

#endif

