#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

char send[256];
char pipeSelected[256] = "pipe";
int counter = 0;
int main()
{
    
    while (strcmp(send, "exit"))
    {

        puts("while girdi");
        puts(pipeSelected);
        int fd = open(pipeSelected, O_WRONLY);
        if (fd == -1)
        {
            return 1;
        }
        printf("input gir: ");
        scanf("%[^\n]%*c", send);
        write(fd, send, strlen(send) + 1);
        printf("sended: %s\n", send);
        close(fd);
        fd = open(pipeSelected, O_RDONLY);
        if (fd == -1)
        {
            return 1;
        }
        char recieve[256];
        read(fd, recieve, 256);
        puts(recieve);
        close(fd);
    }
    return 0;
}