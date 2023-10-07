#include <stdio.h>
// 1. 位运算一般用来操作寄存器


int main(){

    int i = 0xFF;
    printf("source = %X\n",i);
    printf("%X\n",0xFF & 0x0);
    printf("%X\n",0xFF | 0x0);
    printf("%X\n",~0xFF);
    printf("%X\n",0x0001 << 3);
    printf("%X\n",0x1000 >> 4);

    /*解释 
    0x1000 >> 4
    0x1000 先转为二进制的    0001 0000 0000 0000 ；
    右移四位，得到    0001 0000 0000，得到16进制0x100
    不明白的时候可以借助win的计算器
    */


    // 异或^：如果a、b两个值不相同，则异或结果为1。如果a、b两个值相同，异或结果为0。
    printf("%X\n",0^0); // 0
    printf("%X\n",0^1); // 1
    printf("%X\n",1^0); // 1
    printf("%X\n",1^1); // 0
    return 0;
}