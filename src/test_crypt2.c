#define _GNU_SOURCE
#include <unistd.h>
#include <crypt.h>
#include <stdio.h>
int main() {
    printf("crypt test: %s\n", crypt("test", "salt"));
    return 0;
}
