#include <stdio.h>
#include <unistd.h>

int main() {
    int i = 0;
    while (1) {
        printf("loop i=%d\n", i);
        sleep(1);
        i++;
    }
    return 0;
}
