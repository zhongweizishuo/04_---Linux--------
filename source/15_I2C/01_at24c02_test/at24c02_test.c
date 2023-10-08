// 使用I2C（双向串行总线）直接读写EEPROM
// EEPROM：电可擦写ROM (read only memory)
#include <sys/ioctl.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <i2c/smbus.h>
#include "i2cbusses.h"
#include <time.h>


/* 执行命令： 
 * ./at24c02 <i2c_bus_number> w "100ask.taobao.com"
 * ./at24c02 <i2c_bus_number> r
 */

int main(int argc, char **argv)
{
	unsigned char dev_addr = 0x50;
	unsigned char mem_addr = 0;
	unsigned char buf[32];

	int file;
	char filename[20];
	unsigned char *str;

	int ret;

	struct timespec req;
	
	// 用法提示
	if (argc != 3 && argc != 4)
	{
		printf("Usage:\n");
		printf("write eeprom: %s <i2c_bus_number> w string\n", argv[0]);
		printf("read  eeprom: %s <i2c_bus_number> r\n", argv[0]);
		return -1;
	}

	// 打开I2C的设备驱动
	file = open_i2c_dev(argv[1][0]-'0', filename, sizeof(filename), 0);
	if (file < 0)
	{
		printf("can't open %s\n", filename);
		return -1;
	}

	// 在驱动存在的条件下，使用I2C的普遍方式，强制读写（设置）数据
	if (set_slave_addr(file, dev_addr, 1))
	{
		printf("can't set_slave_addr\n");
		return -1;
	}

	if (argv[2][0] == 'w')
	{
		// write str: argv[3] 写操作
		str = argv[3];

		req.tv_sec  = 0;
		req.tv_nsec = 20000000; /* 20ms */
		
		while (*str)
		{
			// mem_addr, *str
			// mem_addr++, str++
			ret = i2c_smbus_write_byte_data(file, mem_addr, *str);
			if (ret)
			{
				printf("i2c_smbus_write_byte_data err\n");
				return -1;
			}
			// wait tWR(10ms) 硬件属性决定的，写完一个字节必须等待10mx
			nanosleep(&req, NULL);
			
			mem_addr++;
			str++;
		}
		ret = i2c_smbus_write_byte_data(file, mem_addr, 0); // string end char
		if (ret)
		{
			printf("i2c_smbus_write_byte_data err\n");
			return -1;
		}
	}
	else
	{
		// read 读操作
		ret = i2c_smbus_read_i2c_block_data(file, mem_addr, sizeof(buf), buf);
		if (ret < 0)
		{
			printf("i2c_smbus_read_i2c_block_data err\n");
			return -1;
		}
		
		buf[31] = '\0';
		printf("get data: %s\n", buf);
	}
	
	return 0;
	
}

