#include <stdio.h>

void func(int *ptr) {
	printf("%p \n",ptr);
    *ptr = 10;
}

int main() {
    int x = 5;
    func(&x);
    printf("%p \n",&x);
    printf("%d\n", x);
    return 0;
}

/*
 * 0x7ffca26d3394
   0x7ffca26d3394
   10
 *
 */
