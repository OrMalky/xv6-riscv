#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

#define SIZE 65536 // 2^16
#define NPROCS 4  // Requested number of child processes

int main() {
    static int arr[SIZE];
    int pids[NPROCS];
    int n;
    int statuses[NPROC]; // Allocate for maximum possible children

    int sum = 0;


    // Initialize the array
    for (int i = 0; i < SIZE; i++){
        arr[i] = i;
    }
    int i = forkn(NPROCS, pids);

    if (i == -1){
        exit(-1,"forkn failed");
     
    } else if (i == 0){
        // Parent process
        waitall(&n, statuses);
        printf("\n");

        if (n != NPROCS) {
            exit(-1, "waitall error");
        }

        for (int j = 0; j < NPROCS; j++) {
            sum += statuses[j];
            printf("pid: %d, sum: %d\n", pids[j], statuses[j]);
        }

        printf("Total sum: %d\n", sum);

        exit(0,"completed");

    } else {
        // Child process
        int chunk = SIZE / NPROCS;
        int start = (i - 1) * chunk;
        int end = (i == NPROCS) ? SIZE : i * chunk;

        for (int j = start; j < end; j++) {
            sum += arr[j];
        }

        printf("sum=%d\n", sum);
        exit(sum, "");
    }

    return 0;
}