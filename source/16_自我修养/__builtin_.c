#include <stdio.h>
#include <string.h>

struct data{
	char a;
	short b;
	int c;
}__attribute__((packed));

int main(){
	
	struct data s;
	printf("size:%ld\n",sizeof(s));
	
	// 1. puts()函数的 samples
	char a[100];
	memcpy(a, "hello ", 20);
	puts(a);
	
	// 2. __builtin_ 内建函数的例子
	// __builtin_memcpy(a, "__builtin_hello ", 50);
	// __builtin_puts(a);
	
	// 3. type transfrom 隐式类型转换
	signed char j=-10; //transfrom into unsigned char  j = 118
	unsigned char b = 2;
	unsigned char c; 
	
	// c = b+j;//c = 118+2 = 120
	c = (unsigned char)(b + (unsigned char)j);
	// printf("Type of c: %u\n", typeof(c)); // Use typeof(c) to print the type
	printf("c = %c\n",c); //print char
	printf("c = %hhu\n",c); //print unsigned char
	printf("c = %d\n",c);
	
	return 0;
}
