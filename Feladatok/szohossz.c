#include<stdio.h>

int main(void)
{
    int h = 0;
    int n = 0x01;
    do{ ++h;}
    while (n<<=1);
    printf("Szóhossz -> "); printf("%d",h); printf(" bit" ); printf("\n");
    return 0;

}