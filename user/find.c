#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include <stdbool.h>

char buf[512], *p;

int judge(char *path,char *tar,int len){

  char *pq;

  // Find first character after last slash.
  for(pq=path+len; pq >= path; pq--){
     if(*pq == '/'){
        if(strcmp(pq+1,tar)==0){
            return true;
        }
     }
  }
  return false;
    
}

void find(char *path,char *tar,int len){

  int fd;
  struct dirent de;
  struct stat st;
  // printf("%s:%s,%d\n","path",path,len);

  if((fd = open(path, 0)) < 0){
    fprintf(2, "ls: cannot open %s,%d,%d\n", path,strlen(path),len);
    return;
  }
  if(fstat(fd, &st) < 0){
    fprintf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
  case T_FILE:
    // printf("%s %d %d %l\n", fmtname(path), st.type, st.ino, st.size);
    // printf("%s:%s\n","check",path);
    if(judge(path,tar,len)) printf("%s\n",path);
    break;

  case T_DIR:
    // if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
    //   printf("ls: path too long\n");
    //   break;
    // }
    // strcpy(buf, path);
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      // 每次执行 p的地址是始终不变的
      int size = strlen(de.name);
      if(de.name[size-1]=='.') continue;
      p = path+len;
      *p= '/';
      ++p;
      memmove(p, de.name, size);
      p[size] = '\0';
      find(path,tar,len+size+1);
    //   if(judge(path,tar))
    // //   printf("%s %d %d %d\n", fmtname(buf), st.type, st.ino, st.size);
    //     printf("%s %d %d %d\n", (buf), st.type, st.ino, st.size);

    }

    break;
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  if(argc != 3){
    fprintf(2,"%s","error\n");
    exit(0);
  }
  char *path = "./sh";
  int fd = -1;
  if((fd = open(path, 0)) < 0){
    fprintf(2, "ls: cannot open %s\n", path);
  }
  char *dir = argv[1];
  strcpy(buf,dir);
  char *tar = argv[2];
  // printf("%s:%s\n",buf,tar);
  int len = strlen(buf);
  find(buf,tar,len);

  exit(0);
}
