#include <stdio.h>
#include <assert.h>
#include <unistd.h>

#define NMAX  1024

int mydup2(const int fd, const int fd2){

    int tmpfd[NMAX], i;
    /*check fd2 is an illegal value*/
    if((fd2 < 3)||(fd2 > sysconf(_SC_OPEN_MAX))){
        return -1;
    }
    /*check fd is opened*/
    if((tmpfd[0] = dup(fd)) == -1){
        return -1;
    }
    /* special case which fd equals fd2 */
    if(fd == fd2){
        return fd2;
    }
    /*case which fd > fd2*/
    else{

        if(tmpfd[0] == fd2){
            return fd2;
        }
        else if(tmpfd[0] < fd2){
            i = 1;
            while((tmpfd[i++] = dup(fd)) != -1){

                if(tmpfd[i-1] >= fd2){

                    if(tmpfd[i-1] == fd2){
                        /*close the opened fds*/
                        for(i-=2; i>=0; i--){
                            close(tmpfd[i]);
                        }
                        break;
                    }
                    else{
                        close(fd2);
                        tmpfd[i] = dup(fd);
                        assert(fd2 == tmpfd[i]);
                        /*close the opened fds*/
                        close(tmpfd[i-1]);
                        for(i-=2; i>=0; i--){
                            close(tmpfd[i]);
                        }
                        break;
                    }
                }
            }
            return fd2;
        }
        else{
            close(fd2);
            tmpfd[1] = dup(fd);
            assert(fd2 == tmpfd[1]);
            /*close the opened fds*/
            close(tmpfd[0]);
            return fd2;
        }
    }
}

int main(){
    int fd;

    fd = dup2(1, 9);
    printf("The result by dup2 is %d\n", fd);
    fd = mydup2(2, 5);
    printf("The result by mydup2 is %d\n", fd);

    return 0;

}
