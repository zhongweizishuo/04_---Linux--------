
#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdio.h>


/*使用命令： ./01_get_input_info /dev/input/event0 */
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
	/*
`unsigned int evbit[2];` 是一个用于存储事件类型位图的数据结构。
在Linux中，通常使用数组或类似的数据结构来表示事件类型位图，以便在输入设备管理中记录和配置支持的事件类型。
在这里，`evbit` 是一个无符号整数数组，它可能用于存储输入设备支持的事件类型的位图信息。
通常，这样的位图数据结构用于在 `ioctl` 函数中进行 `EVIOCGBIT` 和 `EVIOCSBIT` 操作，以获取和配置输入设备的事件类型信息。
数组的大小通常取决于需要表示的事件类型数量，而 `unsigned int` 类型被选择是因为它提供足够的位数来表示多个事件类型的位图信息。
每个 `unsigned int` 变量可以存储32位的信息，所以两个 `unsigned int` 变量可以存储64位的位图信息。
操作这个位图时，通常需要使用位运算来启用或禁用特定的事件类型标志位。
	*/
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
	
	if (argc != 2)
	{
		printf("Usage: %s <dev>\n", argv[0]);
		return -1;
	}

	// 打开文件argv[1]
	fd = open(argv[1], O_RDWR);
	if (fd < 0)
	{
		printf("open %s err\n", argv[1]);//打印错误信息
		return -1;
	}

// ioctl()函数的使用，`ioctl`函数来获取输入设备信息,将需要的信息存入id
	err = ioctl(fd, EVIOCGID, &id);//`EVIOCGID`用来获取输入设备ID信息
	if (err == 0)//如果`err`等于0，表示`ioctl`函数调用成功
	{
		printf("bustype 总线类型= 0x%x\n", id.bustype );
		printf("vendor	制造商= 0x%x\n", id.vendor  );
		printf("product = 0x%x\n", id.product );
		printf("version = 0x%x\n", id.version );
	}

	// 读到数据存入evbit数组
	// 解释参考 04/04_WDS_Linux_base/source/11_input/01_app_demo/位运算.md
	len = ioctl(fd, EVIOCGBIT(0, sizeof(evbit)), &evbit);
	if (len > 0 && len <= sizeof(evbit))//读取到了数据
	{
		printf("打印支持的ev事件类型 support ev type: ");
		for (i = 0; i < len; i++)
		{
			byte = ((unsigned char *)evbit)[i];//byte等于evbit第[i]个字节
			for (bit = 0; bit < 8; bit++)
			{
				if (byte & (1<<bit)) { // 解释参考 04/04_WDS_Linux_base/source/11_input/01_app_demo/位运算.md
					printf("%s ", ev_names[i*8 + bit]);
				}
			}
		}
		printf("\n");
	}

	return 0;
}

