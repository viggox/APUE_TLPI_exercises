#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>

#define BUFSIZE  1024

int copy(const char *file1, const char *file2){
    /*copy contents of file1 to file2, ignore the hole*/
    int fd1, fd2, num, i;
    int base, probe;
    char buf[BUFSIZE];
    /*open the source and target files*/
    if((fd1 = open(file1, O_RDWR)) == -1){
        printf("Error opening %s for copying\n", file1);
        return -1;
    }
    if((fd2 = open(file2, O_RDWR|O_APPEND|O_CREAT|O_TRUNC)) == -1){
        printf("Error creating %s to copy\n", file2);
        return -1;
    }

    do{
        memset(buf, '\0', BUFSIZE);

        if((num = read(fd1, buf, BUFSIZE)) == -1){
            printf("Error reading %s for copying\n", file1);
            return -1;
        }
        /*clear all "holes"('\0') in the buf array*/
        base =0;
        for(i=0; i<num; i++){
            if(buf[i] != '\0'){
                base++;
            }
            else{
                probe = i+1;
                while(probe < num){
                    if(buf[probe] != '\0'){
                        buf[base] = buf[probe];
                        i = probe;
                        base++;
                        break;
                    }
                    probe++;
                }
                if(probe == num){
                     break;
                }
            }
        }
        /*write the buf into file2*/
        if((num = write(fd2, buf, base)) == -1){
            printf("Error writing %s to copy\n", file2);
            return -1;
        }
    }
    while(num >0);

    close(fd1);
    close(fd2);

    return 0;
}

int main(int argc, char *argv[]){
    char *file1, *file2;

    if(argc != 3){
        printf("parameter error!\n");
        return -1;
    }

    file1 = argv[1];
    file2 = argv[2];
    return copy(file1, file2);
}
