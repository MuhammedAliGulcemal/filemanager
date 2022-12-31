#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{

    char send[256];
    while (strcmp(send, "exit"))
    {

        int fd = open("pipe", O_WRONLY);
        if (fd == -1)
        {
            return 1;
        }

        printf("input gir: ");
        scanf("%[^\n]%*c", send);
        write(fd, send, strlen(send) + 1);
        printf("sended: %s\n",send);
        close(fd);
        fd = open("pipe", O_RDONLY);
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