#include <stdlib.h>
#include <string.h>
#include <mcheck.h>

/*  内存泄漏与内存泄漏的追踪
    mtrace(); //开启追踪
    // 待检查的代码
    muntrace(); //关闭追踪
*/

int main(){
    mtrace(); //开启追踪
    char *p, *q;
    p =(char *)malloc(8);
    q =(char *)malloc(8);
    strcpy(p, "memory");
    strcpy(q, "leak");
    free(p);
    muntrace(); //关闭追踪
    return 0;
}