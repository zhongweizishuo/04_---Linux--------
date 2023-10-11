#include <stdio.h>


/* 执行命令: ./hello weidongshan 
 * argc = 2
 * argv[0] = ./hello
 * argv[1] = weidongshan
 */

// 指针数组和二级指针作为函数参数时，二者是等价的
// int main(int argc , char *argv[])与下面一行的意义是一样的

int main(int argc, char **argv)
{
	if (argc >= 2)
		printf("Hello, %s!\n", argv[1]);
	else
		printf("Hello, world!\n");
	return 0;
}


