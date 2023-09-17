#include <stdio.h>
#include <string.h>
#include <wchar.h>
// 源代码可以是GB2312或者UTF-8都可以，wchar_t得到的都是字符的unicode值

int main( int argc, char** argv)
{
	wchar_t *chinese_str = L"涓璯if"; //  GB2312
	unsigned int *p = (wchar_t *)chinese_str;
	int i;

	printf("sizeof(wchar_t) = %d, str's Uniocde: \n", (int)sizeof(wchar_t));
	for (i = 0; i < wcslen(chinese_str); i++)
	{
		printf("0x%x ", p[i]);
	}
	printf("\n");

    // sizeof(wchar_t) = 4, str's Uniocde: 
    // 0x4e2d 0x67 0x69 0x66    

	return 0;
}

