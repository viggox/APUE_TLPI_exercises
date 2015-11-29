#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

int writeFile(int fd, char *str){
    size_t nbytes;

    nbytes = strlen(str);
    return write(fd, str, nbytes);
}

int lseekRead(int fd, off_t offset, size_t nbytes, char *buf){

    if(lseek(fd, offset, SEEK_SET) == -1){
        return -2;
    }
    return read(fd, buf, nbytes);

}

int lseekUpdate(int fd, off_t offset, size_t nbytes, char *nbuf){

    if(lseek(fd, offset, SEEK_SET) == -1){
        return -2;
    }
    return write(fd, nbuf, nbytes);
}

int main(void){
    off_t offset;
    int fd, n;
    mode_t mode;
    char str[32] = "SouthParkIsSoBeautiful!";
    char nbuf[10] = "Funny!!!";
    char buf[10];
    mode = S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IWGRP|S_IXGRP|S_IROTH|S_IWOTH|S_IXOTH;

    /*open and write a new file*/
    if((fd = open("exercise3_6.txt", O_RDWR|O_CREAT|O_TRUNC, mode)) == -1){
        return -1;
    }
    if(writeFile(fd, str) == -1){
        return -1;
    }
    close(fd);

    offset = 13;              //set place after "so"

    /*check lseek before Read*/
    /*open the existed file with O_APPEND to test lseekRead*/
    if((fd = open("exercise3_6.txt", O_RDWR|O_CREAT|O_APPEND, mode)) == -1){
        return -1;
    }
    if((n = lseekRead(fd, offset, 6, buf)) == -2){
        printf("lseek cannot do anything after open with O_APPEND!\n");
    }
    else if(n == -1){
        printf("inner read error!\n");
    }
    else{
        printf("check the read string: %s\n", buf);
    }
    close(fd);

    /*open the existed file with O_APPEND to test lseekUpdate*/
    if((fd = open("exercise3_6.txt", O_RDWR|O_CREAT|O_APPEND, mode)) == -1){
        return -1;
    }
    /*check can lseek can update or not*/
    if((n = lseekUpdate(fd, offset, strlen(nbuf), nbuf)) == -2){
        printf("lseek cannot be used to update file at anywhere!\n");
    }
    else if(n == -1){
        printf("inner write error!\n");
    }
    else{
        printf("check the updated string: %s\n", nbuf);
    }
    close(fd);

    return 0;
}
