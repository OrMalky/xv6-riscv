#include "kernel/types.h"
#include "user/user.h"

int main(){
    printf("%d\n", memsize());
    int* c = malloc(20000);
    printf("%d\n", memsize());
    free(c);
    printf("%d\n", memsize());
    return 0;
}