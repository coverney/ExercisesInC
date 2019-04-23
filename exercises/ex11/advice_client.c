/*
Connects to advice server and prints a reply

Written by Cassandra Overney
Released under the Bad Advice Inc.

gcc advice_client.c -o advice_client

./advice_client
telnet 127.0.0.1 30000

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

/* Print error
*/
void error(char *msg)
{
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(1);
}

/* Open socket
*/
int open_socket(char *host, char *port)
{
    struct addrinfo *res;
    struct addrinfo hints;
    int d_sock, c;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = PF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    if (getaddrinfo(host, port, &hints, &res) == -1)
        error("Can't resolve the address");
    if (( d_sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1)
        error("Can't open socket");

    c = connect(d_sock, res->ai_addr, res->ai_addrlen);
    freeaddrinfo(res);
    if (c == -1)
        error("Can't connect to the socket");

    return d_sock;
}

/* Send message
*/
int say(int socket, char *s)
{
    int result = send(socket, s, strlen(s), 0);
    if (result == -1)
        error("Can't talk to the server");

    return result;
}

/* Open socket, receive advice and then print reply
*/
int main(int argc, char *argv[])
{
    int d_sock, bytes_received;
    char buf[255], rec[256];

    /* connect to server */
    d_sock = open_socket("127.0.0.1", "30000");

    // QUESTION: I don't need the GET command and hostname right? No
    // /* request the resource */
    // sprintf(buf, "GET /wiki/%s http/1.1\r\n", argv[1]);
    // say(d_sock, buf);
    //
    // /* send host name and required blank line */
    // say(d_sock, "Host: en.wikipedia.org\r\n\r\n");

    /* display advice in 255 byte chunks */
    bytes_received = recv(d_sock, rec, 255, 0);
    while (bytes_received) {
        if (bytes_received == EOF)
            error("can't read from server");

        rec[bytes_received] = '\0';
        printf("%s", rec);
        bytes_received = recv(d_sock, rec, 255, 0);
    }

    // print reply
    printf("%s\n", "Thanks for the bad advice!");

    close(d_sock);

    return 0;
}
