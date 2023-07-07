This project is about creating an IRC server.
An actual IRC client will be used to connect to the server and test it.

Internet Relay Chat or IRC is a text-based communication protocol on the Internet.
It offers real-time messaging that can be either public or private. Users can exchange
direct messages and join group channels.
IRC clients connect to IRC servers in order to join channels. IRC servers are connected
together to form a network.

 docker run -itd --name=irssi irssi
 docker exec -it irssi irssi
 /set nick [name]
 /connect liberachat
 /join #TALALMR

https://www.bogotobogo.com/cplusplus/sockets_server_client.php
https://www.geeksforgeeks.org/socket-programming-cc/
http://www.cs.cmu.edu/~srini/15-441/F06/project1/chapter6.html

Your executable will be run as follows:
./ircserv <port> <password>

Everything in C++ 98.

### [socket]

#include <sys/socket.h>

int socket(int domain, int type, int protocol);

tcp_socket = socket(AF_INET6, SOCK_STREAM, 0);

https://man7.org/linux/man-pages/man7/tcp.7.html

creates an endpoint for communication and returns a file
descriptor that refers to that endpoint.  The file descriptor
returned by a successful call will be the lowest-numbered file
descriptor not currently open for the process.

AF_INET6     IPv6 Internet protocols

AF_INET      IPv4 Internet protocols

SOCK_NONBLOCK

	Set the O_NONBLOCK file status flag on the open file
	description (see open(2)) referred to by the new file
	descriptor.  Using this flag saves extra calls to fcntl(2)
	to achieve the same result.

SOCK_NONBLOCK and SOCK_CLOEXEC are Linux-specific.

### [close]

#include <unistd.h>

int close(int fd);

https://man7.org/linux/man-pages/man2/close.2.html

closes a file descriptor, so that it no longer refers to
any file and may be reused.  Any record locks (see fcntl(2)) held
on the file it was associated with, and owned by the process, are
removed (regardless of the file descriptor that was used to
obtain the lock).

### [setsockopt]
#include <sys/socket.h>

int setsockopt(int socket, int level, int option_name,
	const void *option_value, socklen_t option_len);

function shall set the option specified by the
option_name argument, at the protocol level specified by the
level argument, to the value pointed to by the option_value
argument for the socket associated with the file descriptor
specified by the socket argument.

Upon successful completion, setsockopt() shall return 0.
Otherwise, -1 shall be returned and errno set to indicate the
error.

### [getsockname]

#include <sys/socket.h>

int getsockname(int sockfd, struct sockaddr *restrict addr,
	socklen_t *restrict addrlen);

returns the current address to which the socket
sockfd is bound, in the buffer pointed to by addr.  The addrlen
argument should be initialized to indicate the amount of space
(in bytes) pointed to by addr.  On return it contains the actual
size of the socket address.

The returned address is truncated if the buffer provided is too
small; in this case, addrlen will return a value greater than was
supplied to the call.

### [getprotobyname]

#include <netdb.h>

struct protoent *getprotobyname(const char *name);

The getprotobyname() function shall search the database from the
beginning and find the first entry for which the protocol name
specified by name matches the p_name member, opening and closing
a connection to the database as necessary.

Upon successful completion, getprotobyname(), getprotobynumber(),
and getprotoent() return a pointer to a protoent structure if the
requested entry was found, and a null pointer if the end of the
database was reached or the requested entry was not found.
Otherwise, a null pointer is returned.

The protoent structure is defined in <netdb.h> as follows:

    struct protoent {
        char  *p_name;       /* official protocol name */
        char **p_aliases;    /* alias list */
        int    p_proto;      /* protocol number */
    }

The members of the protoent structure are:

p_name The official name of the protocol.

p_aliases
       A NULL-terminated list of alternative names for the
       protocol.

p_proto
       The protocol number.

### [gethostbyname]

#include <netdb.h>

**The gethostbyname(), gethostbyaddr(), herror(), and hstrerror()**
**functions are obsolete.  Applications should use getaddrinfo(3),**
**getnameinfo(3), and gai_strerror(3) instead.**

### [getaddrinfo]
### [freeaddrinfo]
### [bind]
### [connect]
### [listen]
### [accept]
### [htons]
### [htonl]
### [ntohs]
### [ntohl]
### [inet_addr]
### [inet_ntoa]
### [send]
### [recv]
### [signal]
### [sigaction]
### [lseek]
### [fstat]
### [fcntl]
### [poll]

