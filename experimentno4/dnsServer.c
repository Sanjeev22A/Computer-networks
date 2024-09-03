#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>

int main(int argc, char *argv[]) {
    struct hostent *host_info;
    struct in_addr addr;
    int flag = 1;
    int key;
    char input[256]; // Increased buffer size

    do {
        printf("Enter 1 to convert domain name to IP address\n");
        printf("Enter 2 to convert IP address to domain name\n");
        printf("Enter input: ");
        if (flag != 1) {
            getchar();
        }
        scanf("%d", &key);

        switch (key) {
            case 1:
                printf("Enter the domain name: ");
                getchar();
                scanf("%255s", input); // Limiting input size

                printf("Will do a DNS query on: %s\n", input);

                host_info = gethostbyname(input);
                if (host_info == NULL) {
                    herror("Error in gethostbyname");
                    break;
                }
                addr = *(struct in_addr *)(host_info->h_addr);
                printf("%s has address %s\n", input, inet_ntoa(addr));
                break;

            case 2:
                printf("Enter the IP address: ");
                getchar();
                scanf("%255s", input); // Limiting input size

                if (inet_aton(input, &addr) == 0) {
                    fprintf(stderr, "Invalid IP address format\n");
                    break;
                }

                printf("Will do a DNS query on: %s\n", input);

                host_info = gethostbyaddr((const void *)&addr, sizeof(addr), AF_INET);
                if (host_info == NULL) {
                    herror("Error in gethostbyaddr");
                    break;
                }

                printf("The IP address %s corresponds to the domain name %s\n", input, host_info->h_name);
                break;

            default:
                printf("Invalid input\n");
        }

        printf("Enter 1 to continue, 0 to exit: ");
        getchar();
        scanf("%d", &flag);

    } while (flag);

    return 0;
}
