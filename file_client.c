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

        strcpy(send,"");
        int fd = open(pipeSelected, O_WRONLY);
        if (fd == -1)
        {
            return 1;
        }
        printf("Enter input: ");
        scanf("%[^\n]%*c", send);
        write(fd, send, strlen(send) + 1);
        printf("Sended: %s\n", send);
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