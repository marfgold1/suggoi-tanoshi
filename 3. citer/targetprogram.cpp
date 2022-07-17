#include <iostream>
#include <stdio.h>
#include <cstring>

int main() {
    int buf = 0xDEADBEEF;
    char* anjay = (char*)malloc(15);
    strcpy(anjay, "amar fadil");
    printf("Target value : 0x%x\nTarget address : 0x%p\n", buf, &buf);
    printf("Target string val : %s\nTarget string address : 0x%p\nInput any number to continue\n", anjay, anjay);

    int prompt_buf;
    std::cin >> prompt_buf;

    printf("Target value : 0x%x\n", buf);
    printf("Target string val : %s\n", anjay);
    return 0;
}
