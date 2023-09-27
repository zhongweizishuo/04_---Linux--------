#include <stdio.h>
#include <stdlib.h>
// 段错误

int main(){
    
    char *p = malloc(64);
    printf("p = %p\n",p);
    *p = 1;
    printf("*p = %d\n",*p); 

    #if 0
    
    char *p ;
    printf("p = %p\n",p);
    *p = 1;//Segmentation fault (core dumped)
    printf("*p = %d\n",*p); 
    #endif
    
    free(p);
    free(p); //double free detected in tcache 2
    return 0;
}
