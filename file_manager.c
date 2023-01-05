#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>

pthread_mutex_t mutexLock;
char *fileList[10];
int threadsayi = 100;
pthread_t threadList[100];
int count = 0;
char *message;
char recieve[256];
char *strArr[5];
int fileIndex = 0;
int nullIndex = 0;
char fileMessage[256];
char pipeSelected[256] = "pipe";
char *command;

void *createFonk(void *arg)
{
    pthread_mutex_lock(&mutexLock);
    fileList[fileIndex] = strArr[1];
    puts("FILE CREATE:");
    FILE *f = fopen(strArr[1], "w");
    fclose(f);
    pthread_mutex_unlock(&mutexLock);
    return NULL;
}
void *deleteFonk(void *arg)
{
    pthread_mutex_lock(&mutexLock);
    fileList[fileIndex] = NULL;
    puts("FILE DELETE:");
    remove(strArr[1]);
    pthread_mutex_unlock(&mutexLock);
    return NULL;
}

void *readFonk(void *arg)
{
    pthread_mutex_lock(&mutexLock);
    puts("FILE READ");
    FILE *f = fopen("mali", "r");
    while (fgets(fileMessage, 256, f) != NULL)
    {
        puts(fileMessage);
    }
    fclose(f);
    pthread_mutex_unlock(&mutexLock);
    return NULL;
}
void *writeFonk(void *arg)
{
    pthread_mutex_lock(&mutexLock);
    fileList[fileIndex] = strArr[1];
    puts("FILE WRITE:");
    FILE *f = fopen(strArr[1], "w");
    fputs(strArr[2], f);
    fclose(f);
    pthread_mutex_unlock(&mutexLock);
    return NULL;
}

int fileControl()
{
    puts("FILE CONTROL:");
    for (int i = 0; i < 10; i++)
    {
        if (fileList[i] != NULL)
        {
            printf("Filelist %d: ", i);
            puts(fileList[i]);
            printf("Filename: ");
            puts(strArr[1]);
            if (strcmp(fileList[i], strArr[1]) == 0)
            {
                puts("esitlik var");
                fileIndex = i;
                return 0;
            }
        }
        else
        {
            fileIndex = i;
            return 0;
        }
    }
    fileIndex = 10;
    return 0;
}
int writeControl()
{
    int control = 1;
    puts("WRITE CONTROL");
    for (int i = 0; i < 10; i++)
    {
        if (fileList[i] != NULL)
        {
            printf("Filelist %d: ", i);
            puts(fileList[i]);
            printf("Filename: ");
            puts(strArr[1]);
            for (int j = 0; j < strlen(fileList[i]); j++)
            {
                puts("FOR GIRDI");
                if (*(fileList[i] + j) == *(strArr[i] + j + 1))
                {
                    puts("IF GIRDI");
                    control = 0;
                }
            }
            if (control)
            {
                puts("ESITLIK VAR");
                fileIndex = i;
                return 0;
            }
        }
    }
    fileIndex = 10;
    return 0;
}

int readControl()
{
    int control = 1;
    puts("READ CONTROL");
    for (int i = 0; i < 10; i++)
    {
        if (fileList[i] != NULL)
        {
            printf("Filelist %d: ", i);
            puts(fileList[i]);
            printf("Filename: ");
            puts(strArr[1]);
            for (int j = 0; j < strlen(fileList[i]); j++)
            {
                puts("FOR GIRDI");
                if (*(fileList[i] + j) == *(strArr[i] + j + 1))
                {
                    puts("IF GIRDI");
                    control = 0;
                }
            }
            if (control)
            {
                puts("ESITLIK VAR");
                fileIndex = i;
                return 0;
            }
        }
    }
    fileIndex = 10;
    return 0;
}

int main()
{
    if (pthread_mutex_init(&mutexLock, NULL) != 0)
    {
        printf("mutex init hatasi\n");
        return 1;
    }
    command = "mali";
    while (strcmp(command, "exit"))
    {
        for (int i = 0; i < 10; i++)
        {
            if (fileList[i] != NULL)
            {
                puts(fileList[i]);
            }
            else
            {
                puts("NULL");
            }
        }

        int fd = open(pipeSelected, O_RDONLY);
        if (fd == -1)
        {
            return 1;
        }
        read(fd, recieve, 256);
        char *str = strtok(recieve, " ");
        int i = 0;
        strArr[i] = str;
        i++;
        while (str != NULL)
        {
            str = strtok(NULL, " ");
            strArr[i] = str;
            i++;
        }
        i--;
        for (int j = 0; j < i; j++)
        {
            printf("Input %d: ", j);
            puts(strArr[j]);
        }
        command = strArr[0];
        close(fd);
        if (strcmp(command, "create") == 0)
        {
            fileControl();
            if (fileIndex != 10)
            {
                printf("Fileindex1: %d\n", fileIndex);
                if (fileList[fileIndex] == NULL)
                {
                    printf("Fileindex2: %d\n", fileIndex);
                    pthread_create(threadList + count, NULL, createFonk, NULL);
                    pthread_join(threadList[count], NULL);
                    count++;
                    message = "File created";
                }
                else
                {
                    message = "File already exists!";
                }
            }
            else
            {
                message = "File list is full!";
            }
        }
        else if (strcmp(command, "delete") == 0)
        {
            fileControl();
            if (fileIndex != 10)
            {
                printf("Fileindex1: %d\n", fileIndex);
                if (fileList[fileIndex] == NULL)
                {
                    message = "File is not exists!";
                }
                else
                {
                    if (strcmp(fileList[fileIndex], strArr[1]) == 0)
                    {
                        printf("Fileindex2: %d\n", fileIndex);
                        pthread_create(threadList + count, NULL, deleteFonk, NULL);
                        pthread_join(threadList[count], NULL);
                        count++;
                        message = "File deleted";
                    }
                }
            }
            else
            {
                message = "File is not exists!";
            }
        }
        else if (strcmp(command, "read") == 0)
        {
            readControl();
            if (fileIndex != 10)
            {
                printf("Fileindex1: %d\n", fileIndex);
                if (fileList[fileIndex] == NULL)
                {
                    message = "File is not exists!";
                }
                else
                {
                    if (fileIndex != 10)
                    {
                        printf("Fileindex2: %d\n", fileIndex);
                        pthread_create(threadList + count, NULL, readFonk, NULL);
                        pthread_join(threadList[count], NULL);
                        count++;
                        message = fileMessage;
                    }
                }
            }
            else
            {
                message = "File is not exists!";
            }
        }
        else if (strcmp(command, "write") == 0)
        {
            writeControl();
            if (fileIndex != 10)
            {
                printf("Fileindex1: %d\n", fileIndex);
                if (fileList[fileIndex] == NULL)
                {
                    message = "File is not exists!";
                }
                else
                {
                    if (fileIndex != 10)
                    {
                        printf("Fileindex2: %d\n", fileIndex);
                        pthread_create(threadList + count, NULL, writeFonk, NULL);
                        pthread_join(threadList[count], NULL);
                        count++;
                        message = "File writed";
                    }
                }
            }
            else
            {
                message = "File is not exists!";
            }
        }
        else
        {
            message = "Wrong Input!";
        }

        fd = open(pipeSelected, O_WRONLY);
        if (fd == -1)
        {
            return 1;
        }
        write(fd, message, strlen(message) + 1);
        printf("message: %s\n", message);
        close(fd);
        pthread_mutex_destroy(&mutexLock);
    }

    return 0;
}