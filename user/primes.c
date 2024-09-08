#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include <stdbool.h>


    //   close(1);
    //         dup(cmup[1]);
    //         close(cmup[1]);
    //  if(pid != 0){
        
    //         close(cmup[0]);
    //     }
    //     else{
    //         close(0);
    //         close(1);
    //         dup(cmup[0]);
    //         close(cmup[0]);
    //         close(cmup[1]);
    //     }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Winfinite-recursion"
void process(int *cmup){
    // close(cmup[0]);
    // close(0);
    int n = -5;
    close(cmup[1]);
    int size = read(cmup[0],&n,4);
    // fprintf(1,"%s:%d,%d\n","recieve",n,size);

    if(size == 0) exit(0);
    int init = n;
    fprintf(1,"%s %d\n","prime",n);
    int fds[2];
    pipe(fds);
    if (fork() == 0){
        close(cmup[0]);
        // close(fds[1]);
        process(fds);
    }
    // real process (fork =0 can not got to here)
    while(true){
        // int size = read(fds[0],&n,4);
        int size = read(cmup[0],&n,4);
        if(size == 0) break;
        else{
            if( n % init != 0 ){
                // write(cmup[1],n,sizeof(n));
                // fprintf(1,"%s:%d\n","recieve",n);
                write(fds[1],(void *)&n,4);
            }
        } 
    }
    close(cmup[0]);
    close(fds[1]);
    wait(0);
    exit(0);
   
}
#pragma GCC diagnostic pop

int main(){
    int fds[2];

    pipe(fds);
    if (fork() == 0){
        close(fds[1]);
        process(fds);
        // process part
    }
    else{
        close(fds[0]);
        for (int i = 2;i<36;i++){
            write(fds[1],(void *)&i,4);
        }
        close(fds[1]);
        wait(0);
        exit(0);
    }
    
}