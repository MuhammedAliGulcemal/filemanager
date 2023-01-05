#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
 
char send[256];//gonderilecek input
char pipeSelected[256] = "pipe";//pipe
int counter = 0;
int main()
{
    
    while (strcmp(send, "exit"))//exit degilken
    {

        int fd = open(pipeSelected, O_WRONLY);//pipe ac
        if (fd == -1)
        {
            return 1;
        }
        printf("Enter input: ");//input iste
        scanf("%[^\n]%*c", send);//inputu al
        write(fd, send, strlen(send) + 1);//pipea inputu yaz
        printf("Sended: %s\n", send);
        close(fd);//pipe kapat
        fd = open(pipeSelected, O_RDONLY);//geri ac
        if (fd == -1)
        {
            return 1;
        }
        char recieve[256];//mesajı almak icin degisken
        read(fd, recieve, 256);//pipetan mesaji oku
        puts(recieve);//yaz
        close(fd);//pipe kapat
    }
    return 0;
}