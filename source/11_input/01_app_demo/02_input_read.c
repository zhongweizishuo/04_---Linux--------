
#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>


/* ./01_get_input_info /dev/input/event0 noblock */
int main(int argc, char **argv)
{
	int fd;
	int err;
	int len;
	int i;
	unsigned char byte;
	int bit;
	struct input_id id;
	unsigned int evbit[2];
	struct input_event event;
	
	char *ev_names[] = {
		"EV_SYN ",
		"EV_KEY ",
		"EV_REL ",
		"EV_ABS ",
		"EV_MSC ",
		"EV_SW	",
		"NULL ",
		"NULL ",
		"NULL ",
		"NULL ",
		"NULL ",
		"NULL ",
		"NULL ",
		"NULL ",
		"NULL ",
		"NULL ",
		"NULL ",
		"EV_LED ",
		"EV_SND ",
		"NULL ",
		"EV_REP ",
		"EV_FF	",
		"EV_PWR ",
		};
	
	if (argc < 2)
	{
		// 提示用户正确的用法：<必须的参数> [可省略的参数]
		printf("Usage: %s <dev> [noblock]\n", argv[0]);
		return -1;
	}

	if (argc == 3 && !strcmp(argv[2], "noblock")) //参数3为非阻塞方式，也就是查询方式来读写IO
	{
		fd = open(argv[1], O_RDWR | O_NONBLOCK);
	}
	else
	{
		fd = open(argv[1], O_RDWR);//默认打开方式为阻塞打开方式（休眠唤醒机制）
	}

	if (fd < 0)
	{
		printf("open %s err\n", argv[1]);
		return -1;
	}

	// 读取设备ID相关的信息
	err = ioctl(fd, EVIOCGID, &id);
	if (err == 0)
	{
		printf("bustype = 0x%x\n", id.bustype );
		printf("vendor	= 0x%x\n", id.vendor  );
		printf("product = 0x%x\n", id.product );
		printf("version = 0x%x\n", id.version );
	}

	// 读取位图相关的信息
	len = ioctl(fd, EVIOCGBIT(0, sizeof(evbit)), &evbit);
	if (len > 0 && len <= sizeof(evbit))
	{
		printf("support ev type: ");
		for (i = 0; i < len; i++)
		{
			byte = ((unsigned char *)evbit)[i];
			for (bit = 0; bit < 8; bit++)
			{
				if (byte & (1<<bit)) {
					printf("%s ", ev_names[i*8 + bit]);
				}
			}
		}
		printf("\n");
	}
	
	// 死循环读取event
		// 如果非阻塞，一直会返回read err -1; 当外设产生数据的时候，就会返回get event 数据
		// 如果阻塞，不会返回read err -1; 当外设产生数据的时候，就会返回get event 数据
	while (1)
	{
		len = read(fd, &event, sizeof(event));
		if (len == sizeof(event))
		{
			printf("get event: type = 0x%x, code = 0x%x, value = 0x%x\n", event.type, event.code, event.value);
		}
		else
		{
			printf("read err %d\n", len);
		}
	}

	return 0;
}

