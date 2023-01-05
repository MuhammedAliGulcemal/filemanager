#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>

pthread_mutex_t mutexLock; // mutexlock
char *fileList[10];        // dosyalari tutmak icin filelist
int threadsayi = 100;
pthread_t threadList[100]; // threadler icin array
int count = 0;
char *message;                   // dondurulecek mesaj
char recieve[256];               // alinan input
char *strArr[5];                 // inputu boldugumuz parcalar icin
int fileIndex = 0;               // index tutma
int nullIndex = 0;               // null olan indexi tutma
char fileMessage[256];           // dosyadan okunan yazi icin degisken
char pipeSelected[256] = "pipe"; // pipe
char *command;                   // komutu tutmak icin degisken

void *createFonk(void *arg) // komut create ise
{
    pthread_mutex_lock(&mutexLock);   // lockla
    fileList[fileIndex] = strArr[1];  // bos yerde o dosyayi olustur
    FILE *f = fopen(strArr[1], "w");  // filei olustur
    fclose(f);                        // kapat
    pthread_mutex_unlock(&mutexLock); // locku ac
    return NULL;
}
void *deleteFonk(void *arg) // komut delete ise
{
    pthread_mutex_lock(&mutexLock);   // lockla
    fileList[fileIndex] = NULL;       // ilgili indexi null yap
    remove(strArr[1]);                // dosyayi sil
    pthread_mutex_unlock(&mutexLock); // locku ac
    return NULL;
}

void *readFonk(void *arg) // komut read ise
{
    pthread_mutex_lock(&mutexLock);            // lockla
    FILE *f = fopen("mali", "r");              // filei ac
    while (fgets(fileMessage, 256, f) != NULL) // icindeki mesaji al
    {
        puts(fileMessage);
    }
    fclose(f);                        // dosyayi kapat
    pthread_mutex_unlock(&mutexLock); // locku ac
    return NULL;
}
void *writeFonk(void *arg) // komut write ise
{
    pthread_mutex_lock(&mutexLock);   // lockla
    FILE *f = fopen(strArr[1], "w");  // yazma modunda ac
    fputs(strArr[2], f);              // mesaji yaz
    fclose(f);                        // dosyayi kapat
    pthread_mutex_unlock(&mutexLock); // locku ac
    return NULL;
}

int fileControl() // dosya var mi kontrol icin
{
    for (int i = 0; i < 10; i++)
    {
        if (fileList[i] != NULL)
        {
            if (strcmp(fileList[i], strArr[1]) == 0) // varsa
            {
                fileIndex = i; // indexi al
                return 0;
            }
        }
        else
        {
            fileIndex = i;
            return 0;
        }
    }
    fileIndex = 10; // yoksa 10 dondur
    return 0;
}
int writeControl() // write yaptiginda dosya var mi kontrol icin
{
    int control = 1;
    for (int i = 0; i < 10; i++)
    {
        if (fileList[i] != NULL)
        {
            for (int j = 0; j < strlen(fileList[i]); j++)
            {

                if (*(fileList[i] + j) != *(strArr[1] + j + 1)) // char kontrolu
                {
                    control = 0;
                }
            }
            if (control)
            {
                fileIndex = i; // indexi al
                return 0;
            }
        }
    }
    fileIndex = 10; // yoksa 10 dondur
    return 0;
}

int readControl() // read yaptiginda dosya var mı kontrol icin
{
    int control = 1;
    for (int i = 0; i < 10; i++)
    {
        if (fileList[i] != NULL)
        {
            for (int j = 0; j < strlen(fileList[i]); j++)
            {
                if (*(fileList[i] + j) != *(strArr[1] + j + 2)) // char kontrolu
                {
                    control = 0;
                }
            }
            if (control)
            {
                fileIndex = i; // indexi al
                return 0;
            }
        }
    }
    fileIndex = 10; // yoksa 10 dondur
    return 0;
}

int main()//main
{
    if (pthread_mutex_init(&mutexLock, NULL) != 0)//lock init
    {
        printf("mutex init hatasi\n");
        return 1;
    }
    command = "mali";//komut farkli olsun
    while (strcmp(command, "exit"))//exit olmadikca
    {
        puts("File list:");
        for (int i = 0; i < 10; i++)//file listi yazdir
        {   
            if(fileList[i] != NULL){
                puts(fileList[i]);
            }else{
                puts("NULL");
            }
        }
        
        int fd = open(pipeSelected, O_RDONLY);//pipe ac
        if (fd == -1)
        {
            return 1;
        }
        read(fd, recieve, 256);//pipedan oku
        char *str = strtok(recieve, " ");//okudugu veriyi parcalara ayir
        int i = 0;
        strArr[i] = str;
        i++;
        while (str != NULL)
        {
            str = strtok(NULL, " ");
            strArr[i] = str;//parclari arrayde tut
            i++;
        }
        i--;
        command = strArr[0];//komutu al
        close(fd);
        if (strcmp(command, "create") == 0)//komut create ise
        {
            fileControl();//dosya var mı kontrol et
            if (fileIndex != 10)
            {
                if (fileList[fileIndex] == NULL)
                {
                    pthread_create(threadList + count, NULL, createFonk, NULL);//thread yarat
                    pthread_join(threadList[++count], NULL);//joinle
                    message = "File created";//mesaj dondur
                }
                else
                {
                    message = "File already exists!";//dosya var
                }
            }
            else
            {
                message = "File list is full!";//liste dolu
            }
        }
        else if (strcmp(command, "delete") == 0)//komut delete ise
        {
            fileControl();//dosya var mı kontrol et
            if (fileIndex != 10)
            {
                if (fileList[fileIndex] == NULL)
                {
                    message = "File is not exists!";
                }
                else
                {
                    if (strcmp(fileList[fileIndex], strArr[1]) == 0)
                    {

                        pthread_create(threadList + count, NULL, deleteFonk, NULL);//thread yarat
                        pthread_join(threadList[++count], NULL);//joinle
                        message = "File deleted";//mesaj dondur
                    }
                }
            }
            else
            {
                message = "File is not exists!";//dosya yok
            }
        }
        else if (strcmp(command, "read") == 0)//komut read ise
        {
            readControl();//dosya var mı kontrol
            if (fileIndex != 10)
            {

                if (fileList[fileIndex] == NULL)
                {
                    message = "File is not exists!";
                }
                else
                {
                    if (fileIndex != 10)
                    {
                        pthread_create(threadList + count, NULL, readFonk, NULL);//thread yarat
                        pthread_join(threadList[++count], NULL);//joinle
                        message = fileMessage;//dosyanin icerigini mesaj yap
                    }
                }
            }
            else
            {
                message = "File is not exists!";//dosya yok
            }
        }
        else if (strcmp(command, "write") == 0)//komut write ise
        {
            writeControl();//dosya var mi kontrol et
            if (fileIndex != 10)
            {

                if (fileList[fileIndex] == NULL)
                {
                    message = "File is not exists!";
                }
                else
                {
                    if (fileIndex != 10)
                    {

                        pthread_create(threadList + count, NULL, writeFonk, NULL);//thread yarat
                        pthread_join(threadList[++count], NULL);//joinle
                        message = "File writed";//basarili
                    }
                }
            }
            else
            {
                message = "File is not exists!";//dosya yok
            }
        }
        else
        {
            message = "Wrong Input!";//yanlis input
        }

        fd = open(pipeSelected, O_WRONLY);//pipe ac
        if (fd == -1)
        {
            return 1;
        }
        write(fd, message, strlen(message) + 1);//pipea mesaji yaz
        printf("Message: %s\n", message);
        close(fd);//kapat
        pthread_mutex_destroy(&mutexLock);//locku yok et
    }

    return 0;
}