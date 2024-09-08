#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(){
    int fds[2];
    int ads[2];
    pipe(ads);
    pipe(fds);
    char buf[100];
    if (fork() ==0){
        // close(0);
        // close(1);
        // dup(fds[0]);
        // close(fds[0]);
        // close(fds[1]);
        // dup(ads[1]);
        // close(ads[1]);
        // close(ads[0]);

        close(ads[0]);
        int len = read(fds[0], buf, 5);
        // fprintf(2, "%d: %s\n", getpid(),"ping");
        fprintf(1,"%d: %s\n",getpid(),"received ping");
        close(fds[1]);
        write(ads[1], buf, len);
        close(ads[1]);
        exit(0);
    }
    else{
        // close(0);
        // close(1);
        // dup(ads[0]);
        // dup(fds[1]);
        // close(fds[0]);
        // close(fds[1]);
        // close(ads[0]);
        // close(ads[1]);
        close(fds[0]);
        close(ads[1]);
        write(fds[1], "hello", 5);
        close(fds[1]);
        read(ads[0], buf, 5);
        fprintf(1, "%d: %s\n", getpid(),"received pong");
        exit(0);
    }
}