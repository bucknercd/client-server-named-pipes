#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

typedef struct message {
    char prog[256];
    char rfifo[256];
    char data[512];
} Message;

void execute(Message *msg) {
    int i, parent, id = 0;
    for(i=1; i < 3; i++) {
        switch(parent = fork()) {
            case -1:
                perror("fork error");
                exit(1);
            case 0:
                id = i;
            default:;
        }
        if (parent)
            break;
    }
    if (id == 0)
        wait(NULL);
    if (id == 1)
        exit(0);
    if (id == 2) {
        int fdp[2];
        pipe(fdp);
        write(fdp[1], msg->data, 512);
        close(fdp[1]);
        dup2(fdp[0], STDIN_FILENO);
        close(fdp[0]);
        int rfifo = open(msg->rfifo, O_WRONLY);
        dup2(rfifo, STDOUT_FILENO);
        close(rfifo);
        execlp(msg->prog, msg->prog, NULL);
        perror("exec error");
        exit(1);
    }
 }


int main(void) {
    int fifo = open("/tmp/disp", O_RDWR);
    while(1) {
        Message *msg = malloc(sizeof(Message));
        read(fifo, msg, sizeof(Message));
        execute(msg);
        free(msg);
    }
}