#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main() {
    int pid;
    int n;
    char buf[101];
    int pfd_A[2];
    int pfd_B[2];

    if (pipe(pfd_A) == -1) {
        perror("pipe failed");
        exit(-1);
    }

    if (pipe(pfd_B) == -1) {
        perror("pipe failed");
        exit(-1);
    }

    if ((pid = fork()) < 0) {
        perror("fork failed");
        exit(-2);
    }

    if (pid == 0) {
        
        dup2(pfd_A[0], 0);

        
        close(pfd_A[0]);
        close(pfd_A[1]);

        
        dup2(pfd_B[1], 1);

        
        close(pfd_B[0]);
        close(pfd_B[1]);

        execlp("./A4_CHILD", "./A4_CHILD", (char *) 0);
        perror("execlp");
        exit(-3);
    }

    

    close(pfd_A[0]);
    close(pfd_B[1]);

    printf("\t** Welcome to this number-guessing game! ***\n\n");

    while (1) {
        char NUM[100];
        int r = 0;

        printf("Enter a Number: ");
        fflush(stdout);
        scanf("%s", NUM);

        
        write(pfd_A[1], NUM, strlen(NUM));

        r = read(pfd_B[0], buf, 100);

        if (r > 0) {
            buf[r] = '\0';
            printf("%s\n", buf);
            fflush(stdout);
        } else {
            printf("[PARENT] Reading from child: read() returned %d\n", r);
            break;
        }
    }

    return 0;
}

