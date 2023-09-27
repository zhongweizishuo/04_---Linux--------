#include <stdio.h>
// 1. 数据对齐
// 结构体数据对齐
struct person
{
    char age;
    double num;
};

struct student {
    char a ;
    int c ;
    short b ;
    struct person per;
};

void test01(){
    struct student stu;
    // 打印数据地址
    printf("&a = %p\n",&stu.a);
    printf("&b = %p\n",&stu.b);
    printf("&c = %p\n",&stu.c);
    printf("&per = %p\n",&stu.per.age);

    printf("size of struct stu = %ld\n",sizeof(stu)); //size of = 12与结构体的参数顺序有关
    printf("size of struct per = %ld\n",sizeof(stu.per)); //8+1 = 对齐为16

    struct person per1;
    printf("size of struct per1 = %ld\n",sizeof(per1)); //8+1 = 对齐为16
}

// 2. union:存储数据的开始地址一样，但是最终只存一个
union u
{
    char sex;
    double num;
    int age;
    char a[11];
};

void test_02(){
    union u uu;
    // union的成员的初始地址相同
    printf("&a = %p\n",&uu.sex);
    printf("&b = %p\n",&uu.num);
    printf("&c = %p\n",&uu.age);
    printf("&c = %p\n",&uu.a);

    //16  取4的倍数，而且覆盖最大的成员长度，char a[11]; max(1,8,4,11)
    printf("size of union uu = %ld\n",sizeof(uu)); 


}

int main(){
    // test01();
    test_02();
    return 0;
}