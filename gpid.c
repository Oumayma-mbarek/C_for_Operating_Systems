#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <stdio.h>

int main (int argc, char *argv){
    pid_t mypid;
    mypid= syscall(SYS_getpid);
    printf("L'appel %ld donne le PID: %ld\n", SYS_getpid, mypid);
    return 0;
}