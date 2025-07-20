#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

int *portscan(char *ip, int range, int *count);
int main (int argc, char *argv[]) 
{
   if(argc == 3) 
   {
        int count = 0;
        int range = atoi(argv[2]);
        printf("The address is %s\nthe range of ports is %s\n", argv[1], argv[2]);
        int *result = portscan(argv[1], range, &count);
        if (result)
        {
            if(count == 0) printf("No open ports\n");
            else
            {
                printf("Open ports: \n");
                for (int i = 0; i < count; i++) 
                {
                    printf("%d\n", result[i]);
                }
            }
            free(result);
        }
        return 0;
   }
   else if(argc > 3) 
   {
        printf("Too many arguments supplied.\n");
        return 1;
   }
   else 
   {
        printf("Ip address and range are command line arguments.\n");
        return 2;
   }
}

int *portscan(char *ip, int range, int *count) 
{
    int *ports = malloc(range * sizeof(int));
    if (!ports) 
    {
        perror("malloc failed");
        *count = 0;
        return NULL;
    }
    int add = 0;

    struct sockaddr_in target_addr;
    memset(&target_addr, 0, sizeof(target_addr));
    target_addr.sin_family = AF_INET;

    if (inet_pton(AF_INET, ip, &target_addr.sin_addr) <= 0) 
    {
        fprintf(stderr, "IP not found\n");
        free(ports);
        return NULL;
    }


    for (int port = 1; port <= range; port++) {
        int sockfd = socket(AF_INET, SOCK_STREAM, 0);

        if (sockfd < 0) 
        {
            perror("Socket creation failed");
            continue;
        }

        target_addr.sin_port = htons(port); 

        if (connect(sockfd, (struct sockaddr *)&target_addr, sizeof(target_addr)) == 0) 
        {   
            ports[add++] = port;
            printf("Port %d is open\n", port);
        }
        close(sockfd); 
    }
    *count = add;
    return ports;
}

