#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h"
#include <stdbool.h>
void debug(char *p[]){
    int i =0;
    printf("debug:\n");
    while(p[i]!=0){
        printf("%s\n",p[i++]);
    }
    printf("end debug\n");
}
int main(int argc, char* argv[]){
    if(argc < 2)  fprintf(2,"%s error\n",argv[0]);
    char buf[512];
    int loc = 0;
    char *p[MAXARG];
    int idx = 0;
    for(;idx<argc-1;idx++){
        p[idx] = argv[idx+1];
    }
    int i = 0; int end = 0;
    // p[idx++] = buf;
    int start = loc;
    while(true){
        int n = (read(0,buf+loc,sizeof(buf)-loc));
        if(n < 0 ) {
            fprintf(2,"read error\n");
            exit(1);
        }
        buf[n+loc] = 0;
        // printf("read num from loc %d:%d,buf:%s\n",loc,n,buf);
        // printf("%c ",buf[i]);
        // printf("\n");
        if( n == 0 ){
            if(loc == 0)
                exit(0);
            p[idx++] =buf+start;
            buf[loc] = '\0';
            if(fork()==0){
                p[idx++]=0;
                exec(argv[0],p);
            }
            else{
                wait(0);
                exit(0);

            }
        }
        i = loc; end = loc+n;

        for(;i<end;i++){
            if (buf[i]==' '){
                if( i!=start){
                    p[idx++] = buf+start;
                    start = i;
                }
                buf[i]='\0';
                // while(i<end && buf[i]==' ') i++;
                start++;
                // --i;
            }
            if ( buf[i]=='\n'){
                if(i==start){
                    start++;
                    continue;
                }
                p[idx++] = buf+start;
                buf[i]='\0';
                // printf("param:%s\n",buf+start);
                // printf("%d:%d\n",i,start);
                if(fork()==0){
                    p[idx++] = 0;
                    // debug(p);
                    // printf("exec:%s\n",argv[1]);
                    exec(argv[1],p);
                }
                else{
                    wait(0);
                    idx = argc - 1;
                    start = i+1;                 
                }
            }
        }
        loc = end;
    }
  
}