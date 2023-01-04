#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>

pthread_mutex_t mutexLock;
char *fileList[10];
int threadsayi = 5;
pthread_t threadList[5];
void *status;
int count = 0;
char *message;
char recieve[256];
char *strArr[5];
int fileIndex = 0;
char *fileName = "0";
int nullIndex = 0;
char fileMessage[256];

void createFonk()
{
    pthread_mutex_lock(&mutexLock);
    puts("file create");
    fileList[nullIndex] = fileName;
    FILE *f = fopen(fileName, "w");
    fclose(f);
    puts(fileName);
    pthread_mutex_unlock(&mutexLock);
}
void deleteFonk()
{
    pthread_mutex_lock(&mutexLock);
    fileList[fileIndex] = NULL;
    if (remove(fileName) == 0)
    {
        puts("Deleted successfully");
    }
    else
    {
        puts("Unable to delete the file");
    }
    puts(fileName);
    pthread_mutex_unlock(&mutexLock);
}

void readFonk()
{
    pthread_mutex_lock(&mutexLock);
    puts("file read");
    FILE *f = fopen(fileName, "r");
    while (fgets(fileMessage,256,f)!=NULL)
    {
        puts(fileMessage);
    }
    fclose(f);
    pthread_mutex_unlock(&mutexLock);
}
void writeFonk()
{
    pthread_mutex_lock(&mutexLock);
    puts("file write");
    FILE *f = fopen(fileName, "w");
    fputs(strArr[2],f);
    fclose(f);
    pthread_mutex_unlock(&mutexLock);
}

int fileControl(char *fileName)
{
    nullIndex = 0;
    for (int i = 0; i < 10; i++)
    {
        puts("for donuyor");
        if (fileList[i] != NULL)
        {
            puts(fileList[i]);
            puts(fileName);
            puts("null değil");
            if (strcmp(fileList[i], fileName) == 0)
            {
                puts("filelist dönüyor");
                fileIndex = i;
                printf("index: %d",fileIndex );
                return i;
            }
            nullIndex++;
        }
    }
    fileIndex = 10;
    return 10;
}

int main()
{
    char *command = "mali";
    for (int i = 0; i < threadsayi; ++i)
    {
        pthread_join(threadList[i], &status);
    }
    for (int i = 0; i < 5; i++)
    {
        if (pthread_mutex_init(&mutexLock, NULL) != 0)
        {
            printf("\n mutex init hatasi\n");
            return 1;
        }
    }
    message = "calisti";
    while (strcmp(command, "exit"))
    {
        puts("while");
        int fd = open("pipe", O_RDONLY);
        if (fd == -1)
        {
            return 1;
        }
        read(fd, recieve, 256);
        puts(recieve);
        char *str = strtok(recieve, " ");
        int i = 0;
        strArr[i] = str;
        i++;
        while (str != NULL) // inputu parçalara ayırma
        {
            str = strtok(NULL, " ");
            strArr[i] = str;
            i++;
        }
        i--;
        for (int j = i; j < i; j++)
        {
            puts(strArr[i]);
        }
        command = strArr[0];
        if (strcmp(command, "create") == 0)
        {
            fileIndex = fileControl(strArr[1]);
            fileName = strArr[1];
            if (fileIndex == 10 && nullIndex != 10)
            {
                pthread_create(threadList + count, NULL, createFonk, count + 1);
                puts(command);
                message = "create yapildi";
                count++;
            }
            else
            {
                message = "file exists";
            }
        }
        else if (strcmp(command, "delete") == 0)
        {
            fileIndex = fileControl(strArr[1]);
            fileName = strArr[1];
            if (fileIndex != 10 )
            {
                pthread_create(threadList + count, NULL, deleteFonk, count + 1);
                puts(command);
                message = "delete yapildi";
                count++;
            }
            else
            {
                message = "file not exists";
            }
        }
        else if (strcmp(command, "read") == 0)
        {
            fileIndex = fileControl(strArr[1]);
            fileName = strArr[1];
            if (fileIndex != 10)
            {
                pthread_create(threadList + count, NULL, readFonk, count + 1);
                puts(command);
                strcpy(message,fileMessage);
                count++;
            }
            else
            {
                message = "file not exists";
            }
        }
        else if (strcmp(command, "write") == 0)
        {
            fileIndex = fileControl(strArr[1]);
            fileName = strArr[1];
            puts(strArr[0]);
            puts(strArr[1]);
            puts(strArr[2]);
            if (fileIndex != 10)
            {
                pthread_create(threadList + count, NULL, writeFonk, count + 1);
                puts(command);
                message = "write yapildi";
                count++;
            }
            else
            {
                message = "file not exists";
            }
        }
        else if (strcmp(command, "exit") == 0)
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
        pthread_mutex_destroy(&mutexLock);
    }
    return 0;
}