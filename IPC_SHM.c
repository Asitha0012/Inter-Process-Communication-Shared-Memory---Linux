#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#define SHM_SIZE 4096

void write_to_file(const char *procName, pid_t pid) {
    FILE *fp = fopen("File2.text", "a");
    if (fp == NULL) {
        perror("File open error");
        exit(1);
    }
    fprintf(fp, "All children of %s with PID %d responded.\n", procName, pid);
    fclose(fp);
}

int main() {
    key_t keys[6];
    int shmids[6];
    char *shmptrs[6];

    // Create keys and shared memory for each level
    for (int i = 0; i < 6; i++) {
        char fname[20];
        sprintf(fname, "file%d.txt", i + 1);
        keys[i] = ftok(fname, 65 + i);
        shmids[i] = shmget(keys[i], SHM_SIZE, IPC_CREAT | 0666);
        if (shmids[i] < 0) {
            perror("Shared memory creation failed");
            exit(1);
        }
        shmptrs[i] = (char *)shmat(shmids[i], NULL, 0);
    }

    pid_t PID1;
    switch (PID1 = fork()) {
        case -1:
            perror("Fork error C1");
            exit(1);
        case 0: { // This is C1
            pid_t PID2;
            strcpy(shmptrs[0], "Hello my child");
            printf("C1 (PID: %d) received from P1 (PID: %d): %s\n", getpid(), getppid(), shmptrs[0]);
            fflush(stdout);
            switch (PID2 = fork()) {
                case -1:
                    perror("Fork error C2");
                    exit(1);
                case 0: { // This is C2
                    pid_t PID3;
                    strcpy(shmptrs[1], "Hello my child");
                    printf("C2 (PID: %d) received from C1 (PID: %d): %s\n", getpid(), getppid(), shmptrs[1]);
                    fflush(stdout);
                    switch (PID3 = fork()) {
                        case -1:
                            perror("Fork error C3");
                            exit(1);
                        case 0: { // This is C3
                            pid_t PID4;
                            strcpy(shmptrs[2], "Hello my child");
                            printf("C3 (PID: %d) received from C2 (PID: %d): %s\n", getpid(), getppid(), shmptrs[2]);
                            fflush(stdout);
                            switch (PID4 = fork()) {
                                case -1:
                                    perror("Fork error C4");
                                    exit(1);
                                case 0: { // This is C4
                                    pid_t PID5;
                                    strcpy(shmptrs[3], "Hello my child");
                                    printf("C4 (PID: %d) received from C3 (PID: %d): %s\n", getpid(), getppid(), shmptrs[3]);
                                    fflush(stdout);
                                    switch (PID5 = fork()) {
                                        case -1:
                                            perror("Fork error C5");
                                            exit(1);
                                        case 0: { // This is C5
                                            pid_t PID6;
                                            strcpy(shmptrs[4], "Hello my child");
                                            printf("C5 (PID: %d) received from C4 (PID: %d): %s\n", getpid(), getppid(), shmptrs[4]);
                                            fflush(stdout);
                                            switch (PID6 = fork()) {
                                                case -1:
                                                    perror("Fork error C6");
                                                    exit(1);
                                                case 0: { // This is C6
                                                    printf("C6 (PID: %d) received from C5 (PID: %d): Hello my child\n", getpid(), getppid());
                                                    fflush(stdout);
                                                    strcpy(shmptrs[5], "Hello mum");
                                                    exit(0);
                                                }
                                                default: // C5 (parent of C6)
                                                    wait(NULL);
                                                    printf("C5 (PID: %d) received from C6 (PID: %d): %s\n", getpid(), PID6, shmptrs[5]);
                                                    fflush(stdout);
                                                    strcpy(shmptrs[4], "Hello mum");
                                                    write_to_file("C5", getpid());
                                                    exit(0);
                                            }
                                        }
                                        default: // This is C4
                                            wait(NULL);
                                            printf("C4 (PID: %d) received from C5 (PID: %d): %s\n", getpid(), PID5, shmptrs[4]);
                                            fflush(stdout);
                                            strcpy(shmptrs[3], "Hello mum");
                                            write_to_file("C4", getpid());
                                            exit(0);
                                    }
                                }
                                default: // This is C3
                                    wait(NULL);
                                    printf("C3 (PID: %d) received from C4 (PID: %d): %s\n", getpid(), PID4, shmptrs[3]);
                                    fflush(stdout);
                                    strcpy(shmptrs[2], "Hello mum");
                                    write_to_file("C3", getpid());
                                    exit(0);
                            }
                        }
                        default: // This is C2
                            wait(NULL);
                            printf("C2 (PID: %d) received from C3 (PID: %d): %s\n", getpid(), PID3, shmptrs[2]);
                            fflush(stdout);
                            strcpy(shmptrs[1], "Hello mum");
                            write_to_file("C2", getpid());
                            exit(0);
                    }
                }
                default: // This is C1
                    wait(NULL);
                    printf("C1 (PID: %d) received from C2 (PID: %d): %s\n", getpid(), PID2, shmptrs[1]);
                    fflush(stdout);
                    strcpy(shmptrs[0], "Hello mum");
                    write_to_file("C1", getpid());
                    exit(0);
            }
        }
        default: // This is parent P1
            wait(NULL);
            printf("P1 (PID: %d) received from C1 (PID: %d): %s\n", getpid(), PID1, shmptrs[0]);
            fflush(stdout);
            write_to_file("P1", getpid());
            break;
    }

    // Cleanup shared memory
    for (int i = 0; i < 6; i++) {
        shmctl(shmids[i], IPC_RMID, NULL);
    }

    return 0;
}

