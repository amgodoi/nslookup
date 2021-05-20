/**
 * nslookup.c
 */
#include <stdio.h>

#include <string.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("Usage:\n");
        printf("%s [url]\n", argv[0]);
        exit(1);
    }
    
    struct addrinfo hints;
    memset(&hints, 0, sizeof hints);  // make sure all fields are empty
    hints.ai_family = AF_UNSPEC;      // IPv4 or IPv6 (AF_INET or AF_INET6)
    hints.ai_socktype = SOCK_STREAM;  // TCP stream sockets
        
    char *node = argv[1];
    printf("node: %s\n", node);

    struct addrinfo *servinfo;        // point to the results
    int status;

    if ((status = getaddrinfo(node, NULL, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        exit(1);
    }

    for (struct addrinfo *p = servinfo; p != NULL; p = p->ai_next) {
        void *addr;
        char *ipver;

        if (p->ai_family == AF_INET) { 
            // IPv4
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
            addr = &(ipv4->sin_addr);
            ipver = "IPv4";
        } else { 
            // IPv6
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
            addr = &(ipv6->sin6_addr);
            ipver = "IPv6";
        }

        // convert the IP to a string and print it:
        char ipstr[INET6_ADDRSTRLEN];
        inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
        printf("%s: %s \n", ipver, ipstr);
    }
    
    freeaddrinfo(servinfo);

    return 0;
}

