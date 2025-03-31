#include "kernel/types.h"
#include "user/user.h"

#define SIZE 65536 // 2^16
#define NPROCS 4 // Number of child processes

int main(){
    int* arr = malloc(SIZE * sizeof(int));
    int* pids = malloc(NPROCS * sizeof(int));
    int* n = malloc(sizeof(int));
    int* statuses = malloc(NPROCS * sizeof(int));
    int sum = 0;
    int pipefd[NPROCS][2]; // Array of pipes for each child process


    // Initialize the array
    for (int i = 0; i < SIZE; i++){
        arr[i] = i;
    }

    // Create pipes
    for (int i = 0; i < NPROCS; i++) {
        if (pipe(pipefd[i]) < 0) {
            printf("Pipe %d creation failed.\n", i);
            exit(1, "error");
        }
    }

    int i = forkn(NPROCS, pids);
    if (i == 0){
        // Parent process
        for (int j = 0; j < NPROCS; j++) {
            close(pipefd[j][1]); // Close write end of the pipe
        }
        waitall(n, statuses);
        printf("\n");
        int child_sum;
        for (int j = 0; j < NPROCS; j++) {
            read(pipefd[j][0], &child_sum, sizeof(int)); // Read sum from child
            sum += child_sum;
            printf("pid: %d\n", pids[j]);
        }
    } else if (i==-1){
        printf("fork operation failed.\n");
        exit(1,"failed");
        return 1;
    } else {
        // Child process
        int chunk = SIZE / NPROCS;
        int start = (i - 1) * chunk;
        int end = (i == NPROCS) ? SIZE : i * chunk;
        int local_sum = 0;
    
        for (int j = start; j < end; j++) {
            local_sum += arr[j];
        } 

        printf("sum=%d\n", local_sum);
        write(pipefd[i - 1][1], &local_sum, sizeof(int)); // Write sum to pipe
        close(pipefd[i - 1][1]); // Close write end of the pipe
        exit(0, "");
    }
    if(*n!=NPROCS){
        printf("fork operation failed. %d\n", *n);
        exit(1,"failed");
        return 1;
    }
    
    printf("Total sum: %d\n", sum);
    exit(0,"completed");
    return 0;
}