#include <stdio.h>
#include <stdlib.h>  
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/i2c-dev.h> 
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#define BUFSIZE 1024
//#include <i2c-core.c/i2c.h>
pthread_t ntid;
void write_screen_to_i2c(int file);
void *read_from_i2c_print(void *arg);

int main (void)  
{  
    int file;
    int adapter_nr = 1; /* probably dynamically determined */
    char filename[20]; 
    
    snprintf(filename, 19, "/dev/i2c-%d", adapter_nr);
    file = open(filename, O_RDWR);
    if (file < 0) {
        printf("open error\n");
        /* ERROR HANDLING; you can check errno to see what went wrong */
        exit(1);
    }   
    int addr = 0x08; /* The I2C address */
    if (ioctl(file, I2C_SLAVE, addr) < 0) {
        /* ERROR HANDLING; you can check errno to see what went wrong */
        exit(1);
    }
#if 0
    __u8 reg = 0x10; /* Device register to access */
    __s32 res;
#endif
    int err = pthread_create(&ntid, NULL, read_from_i2c_print, (void *)&file);
    if (err <0 )
    {
        perror("thread create");
        exit(1);
    }
    write_screen_to_i2c(file);
}
void write_screen_to_i2c(int file)
{
    char wbuf[BUFSIZE] = {0};
    /* Using I2C Write, equivalent of 
     i2c_smbus_write_word_data(file, reg, 0x6543) */
    bzero(wbuf,10);
    int i;
    while(1)
    {
        bzero(wbuf,10);
        printf("input what do you want to transfer:\n");
        scanf("%s", wbuf);
        if (write(file, wbuf, strlen(wbuf)) !=  strlen(wbuf)) {
            printf("write error\n");
            perror("write");
            exit(1);
        /* ERROR HANDLING: i2c transaction failed */
        } 
        i++;
        printf("sends %d\n", i);
        /* have to delay or it will be err or*/
        usleep(1);
    }
    //FIXME: did not close file
}
void *read_from_i2c_print(void *arg)
{
    int file = *((int*)(arg));
    char rbuf[1]={0};
	long long howMany = 0;
    while(1)
    {
        /* Using I2C Read, equivalent of i2c_smbus_read_byte(file) */
        /*  read will not be hungup */
        bzero(rbuf, 1);
        if (read(file, rbuf, 1) != 1) {
        /* ERROR HANDLING: i2c transaction failed */
            printf("read error\n");
            sleep(1);
            exit(0);
        }
        if(*rbuf == 0)
        {
            continue;
        }
        howMany ++;
        
        printf("%lld: %c\n", howMany, *rbuf);        
    }
}  
