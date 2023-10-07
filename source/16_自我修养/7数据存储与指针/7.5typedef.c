#include <stdio.h>
#include <stdint.h>

// 0. typedef 与define定义的对比
typedef char* PCHAR1;
#define PCHAR2 char *


// 1. typedef在结构体的运用
typedef struct student
{
    char name[20];
    int age;
    float score;
}student_t, *student_ptr;

int main(){
    student_t stu = {"linghuchong",20,99.5};
    printf("name %s\n",stu.name);
    printf("age %d\n",stu.age);
    printf("age %f\n",stu.score);

    // 两种地址的表示方法，得到的地址是一样的
    student_t *p1 = &stu;
    student_ptr p2 = &stu;
    printf("student_t * = %p\n",p1 );
    printf("student_ptr = %p\n",p2 );

    // size of char * = 8
    /*
    `int*` 的 `sizeof` 取决于所在系统的架构和编译器的实现。在大多数现代系统中：
    - 在 32 位系统上，指针的大小通常是 4 个字节。
    - 在 64 位系统上，指针的大小通常是 8 个字节。
    */
    int * ptr;
    printf("size of int * =  %ld\n",sizeof(ptr));

    PCHAR1 pch1, pch2;
    PCHAR2 pch3, pch4;
    printf("size of char* =  %ld\n",sizeof(pch1));
    printf("size of char* =  %ld\n",sizeof(pch2));
    printf("size of char* =  %ld\n",sizeof(pch3));
    printf("size of char  =  %ld\n",sizeof(pch4));
    return 0;
}