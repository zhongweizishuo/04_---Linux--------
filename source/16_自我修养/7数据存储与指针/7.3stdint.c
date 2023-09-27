#include <stdio.h>
#include <stdint.h>
// 1. stdint.h头文件主要用来定义可移植的数据类型，我们在编程中可以直接使用

int main(){
    __int16_t s;
    int16_t s1;
    printf("size s= %ld\n",sizeof(s));
    printf("size s1= %ld\n",sizeof(s1));
    return 0;
}