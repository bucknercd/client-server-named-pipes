#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

typedef struct message {
    char prog[256];
    char rfifo[256];
    char data[512];
} Message;

int main(int arc, char **argv) {
    char rdata[4096];
    Message *msg = malloc(sizeof(Message));
    strcpy(msg->prog, argv[1]);
    strcpy(msg->rfifo, "/tmp/rfifo");
    strcpy(msg->data, "I am the data!");
    int fifo = open("/tmp/disp", O_WRONLY);
    write(fifo, msg, sizeof(Message));
    int rfifo = open("/tmp/rfifo", O_RDONLY);
    read(rfifo, rdata, 4096);
    printf("DATA:\n%s\n", rdata);
    free(msg);
    exit(0);
}
