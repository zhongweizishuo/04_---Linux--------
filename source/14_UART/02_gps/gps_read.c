// 串口GPS模块实验
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>

/* set_opt(fd,115200,8,'N',1) */
int set_opt(int fd,int nSpeed, int nBits, char nEvent, int nStop)
{
	struct termios newtio,oldtio;
	
	if ( tcgetattr( fd,&oldtio) != 0) { 
		perror("SetupSerial 1");
		return -1;
	}
	
	bzero( &newtio, sizeof( newtio ) );
	newtio.c_cflag |= CLOCAL | CREAD; 
	newtio.c_cflag &= ~CSIZE; 

	newtio.c_lflag  &= ~(ICANON | ECHO | ECHOE | ISIG);  /*Input*/
	newtio.c_oflag  &= ~OPOST;   /*Output*/

	switch( nBits )
	{
	case 7:
		newtio.c_cflag |= CS7;
	break;
	case 8:
		newtio.c_cflag |= CS8;
	break;
	}

	switch( nEvent )
	{
	case 'O':
		newtio.c_cflag |= PARENB;
		newtio.c_cflag |= PARODD;
		newtio.c_iflag |= (INPCK | ISTRIP);
	break;
	case 'E': 
		newtio.c_iflag |= (INPCK | ISTRIP);
		newtio.c_cflag |= PARENB;
		newtio.c_cflag &= ~PARODD;
	break;
	case 'N': 
		newtio.c_cflag &= ~PARENB;
	break;
	}

	switch( nSpeed )
	{
	case 2400:
		cfsetispeed(&newtio, B2400);
		cfsetospeed(&newtio, B2400);
	break;
	case 4800:
		cfsetispeed(&newtio, B4800);
		cfsetospeed(&newtio, B4800);
	break;
	case 9600:
		cfsetispeed(&newtio, B9600);
		cfsetospeed(&newtio, B9600);
	break;
	case 115200:
		cfsetispeed(&newtio, B115200);
		cfsetospeed(&newtio, B115200);
	break;
	default:
		cfsetispeed(&newtio, B9600);
		cfsetospeed(&newtio, B9600);
	break;
	}
	
	if( nStop == 1 )
		newtio.c_cflag &= ~CSTOPB;
	else if ( nStop == 2 )
		newtio.c_cflag |= CSTOPB;
	
	newtio.c_cc[VMIN]  = 1;  /* 读数据时的最小字节数: 没读到这些数据我就不返回! */
	newtio.c_cc[VTIME] = 0; /* 等待第1个数据的时间: 
	                         * 比如VMIN设为10表示至少读到10个数据才返回,
	                         * 但是没有数据总不能一直等吧? 可以设置VTIME(单位是10秒)
	                         * 假设VTIME=1，表示: 
	                         *    10秒内一个数据都没有的话就返回
	                         *    如果10秒内至少读到了1个字节，那就继续等待，完全读到VMIN个数据再返回
	                         */

	tcflush(fd,TCIFLUSH);
	
	if((tcsetattr(fd,TCSANOW,&newtio))!=0)
	{
		perror("com set error");
		return -1;
	}
	//printf("set done!\n");
	return 0;
}

int open_port(char *com)
{
	int fd;
	//fd = open(com, O_RDWR|O_NOCTTY|O_NDELAY);
	fd = open(com, O_RDWR|O_NOCTTY);
    if (-1 == fd){
		return(-1);
    }
	
	  if(fcntl(fd, F_SETFL, 0)<0) /* 设置串口为阻塞状态*/
	  {
			printf("fcntl failed!\n");
			return -1;
	  }
  
	  return fd;
}


int read_gps_raw_data(int fd, char *buf)
{
	// 读取原始数据，存入一个buf
	int i = 0;
	int iRet;
	char c;
	int start = 0;
	
	while (1)// 数据持续读取
	{
		iRet = read(fd, &c, 1);//读取一个数据保存进入字符c
		if (iRet == 1)
		{
			if (c == '$')//开始标志
				start = 1;
			if (start)
			{
				buf[i++] = c; //c持续写入buf
			}
			if (c == '\n' || c == '\r') //见到回车 换行，数据读取结束
				return 0;
		}
		else
		{
			return -1;
		}
	}
}

/* eg. $GPGGA,082559.00,4005.22599,N,11632.58234,E,1,04,3.08,14.6,M,-5.6,M,,*76"<CR><LF> */
int parse_gps_raw_data(char *buf, char *time, char *lat, char *ns, char *lng, char *ew)
{
	char tmp[10];
	
	if (buf[0] != '$')
		return -1;
	else if (strncmp(buf+3, "GGA", 3) != 0) // 第三个字符开始必须是GGA
		return -1;
	else if (strstr(buf, ",,,,,"))
	{
		// strstr()找出",,,,,"第一次在buf里面出现的位置
		printf("Place the GPS to open area\n");
		return -1;
	}
	else {
		//printf("raw data: %s\n", buf);
		// 从buf中读取格式化输入，数据存入形参buf,这些buf在main()中提供
		
		#if 0
		sscanf(buf, "%s, %s, %s, %s, %s, %s", tmp, time, lat, ns, lng, ew);
		// 数据读取错误，sscanf()需要剔除原始数据中的,也就是写成 %[^,]
		#endif

		sscanf(buf, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,]", tmp, time, lat, ns, lng, ew);
		return 0;
	}
}


/*
 * ./serial_send_recv <dev>
 */
int main(int argc, char **argv)
{
	int fd;
	int iRet;
	char c;
	char buf[1000];
	
	// raw_data解析的buf
	char time[100];
	char Lat[100]; 
	char ns[100]; 
	char Lng[100]; 
	char ew[100];

	float fLat, fLng;

	// main() 的实现步骤:
	
	/* 1. open */

	/* 2. setup 
	 * 115200,8N1
	 * RAW mode
	 * return data immediately
	 */

	/* 3. write and read */
	
	if (argc != 2)
	{
		printf("Usage: \n");
		printf("%s </dev/ttySAC1 or other>\n", argv[0]);
		return -1;
	}

	fd = open_port(argv[1]);
	if (fd < 0)
	{
		printf("open %s err!\n", argv[1]);
		return -1;
	}

	iRet = set_opt(fd, 9600, 8, 'N', 1);
	if (iRet)
	{
		printf("set port err!\n");
		return -1;
	}

	// 读写数据与数据处理
	while (1)
	{
		/* eg. $GPGGA,082559.00,4005.22599,N,11632.58234,E,1,04,3.08,14.6,M,-5.6,M,,*76"<CR><LF>
		<CR><LF>结尾，表示回车换行
		*/
		/* read line */
		iRet = read_gps_raw_data(fd, buf);
		
		/* parse line  数据解析*/
		if (iRet == 0)
		{
			iRet = parse_gps_raw_data(buf, time, Lat, ns, Lng, ew);
		}
		
		/* printf */
		if (iRet == 0)
		{
			printf("Time : %s\n", time);
			printf("ns   : %s\n", ns);
			printf("ew   : %s\n", ew);
			printf("Lat  : %s\n", Lat);
			printf("Lng  : %s\n", Lng);

			/* 纬度格式: ddmm.mmmm ；将分（mm.mmmm）转换为dd  */
			sscanf(Lat+2, "%f", &fLat);
			fLat = fLat / 60;
			fLat += (Lat[0] - '0')*10 + (Lat[1] - '0');

			/* 经度格式: dddmm.mmmm ；将分（mm.mmmm）转换为dd  */
			sscanf(Lng+3, "%f", &fLng);
			fLng = fLng / 60;
			fLng += (Lng[0] - '0')*100 + (Lng[1] - '0')*10 + (Lng[2] - '0');
			// (Lng[0] - '0')为了得到数值
			
			printf("Lng, Lat: %.06f,%.06f\n", fLng, fLat);
		}
	}

	return 0;
}

