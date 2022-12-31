#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>

pthread_mutex_t lockList[5];
FILE *fileList[10];
int threadsayi = 5;
pthread_t threadList[5];
void *status;
int count = 0;
char *message;


void createFonk()
{
    puts("file create");
}
void deleteFonk()
{
    puts("file delete");
}

void readFonk()
{
    puts("file read");
}
void writeFonk()
{
    puts("file write");
}

void lockMutex(int val){
    for (int i = 0; i < 5; i++)
    {
        if(i == val){
            pthread_mutex_unlock(&lockList[i]);
        }else{
            pthread_mutex_lock(&lockList[i]);
        }
    }
    
}

int main()
{
    char recieve[256];
    while (strcmp(recieve, "exit"))
    {
        int fd = open("pipe", O_RDONLY);
        if (fd == -1)
        {
            return 1;
        }
        for (int i = 0; i < threadsayi; ++i)
        {
            pthread_join(threadList[i], &status);
        }
        for (int i = 0; i < 5; i++)
        {
            if (pthread_mutex_init(&lockList[i], NULL) != 0)
            {
                printf("\n mutex init hatasi\n");
                return 1;
            }
        }

        read(fd, recieve, 256);
        if (strcmp(recieve, "create") == 0)
        {
            pthread_create(threadList + count, NULL, createFonk, count + 1);
            puts(recieve);
            lockMutex(count);
            message = "create yapildi";
            count++;
        }
        else if (strcmp(recieve, "delete") == 0)
        {
            pthread_create(threadList + count, NULL, deleteFonk, count + 1);
            puts(recieve);
            lockMutex(count);
            message = "delete yapildi";
            count++;
        }
        else if (strcmp(recieve, "read") == 0)
        {
            pthread_create(threadList + count, NULL, readFonk, count + 1);
            puts(recieve);
            lockMutex(count);
            message = "read yapildi";
            count++;
        }
        else if (strcmp(recieve, "write") == 0)
        {
            pthread_create(threadList + count, NULL, writeFonk, count + 1);
            puts(recieve);
            lockMutex(count);
            message = "write yapildi";
            count++;
        }
        else if (strcmp(recieve, "exit") == 0)
        {
            
            message = "exit yapildi";
        }
        else
        {
            message = "yanlis input";
        }
        for (int i = 0; i < threadsayi; ++i)
        {
            pthread_join(threadList[i], &status);
        }
        close(fd);
        fd = open("pipe", O_WRONLY);
        if (fd == -1)
        {
            return 1;
        }
        write(fd, message, strlen(message) + 1);
        close(fd);
    }
    return 0;
}