#include <stdio.h>
#define SIZE 10000
#define NPROCS 4

int main(){
    int* arr[SIZE];
    int pids[NPROCS];
    int sum = 0;
    int* n;
    int statuses[NPROCS];

    for (int i = 0; i < SIZE; i++){
        arr[i] = i;
    }

    i = forkn(NPROCS, pids);
    if (i == 0){
        for (int j = 0; j < NPROCS; j++){
            printf("child %d: %d\n", j, pids[j]);
        }
        waitall(n, statuses);
    } else if (i==-1){
        printf("fork operation failed.");
        return 1;
    } else {
        // Child process
        int start = i * (SIZE / NPROCS);
        int end = (i + 1) * (SIZE / NPROCS);
        for (int j = start; j < end; j++){
            sum += arr[j];
        }
        printf("Child %d: sum = %d\n", i, sum);
        return 0;
    }
    
    return 0;
}