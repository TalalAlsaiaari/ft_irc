This project is about creating an IRC server.
An actual IRC client will be used to connect to the server and test it.

Internet Relay Chat or IRC is a text-based communication protocol on the Internet.
It offers real-time messaging that can be either public or private. Users can exchange
direct messages and join group channels.
IRC clients connect to IRC servers in order to join channels. IRC servers are connected
together to form a network.

 docker run -itd --name=irssi irssi
 docker exec -it irssi irssi
 /set nick [ name ]
 /connect liberachat
 /join #TALALMR

https://www.bogotobogo.com/cplusplus/sockets_server_client.php
https://www.geeksforgeeks.org/socket-programming-cc/
http://www.cs.cmu.edu/~srini/15-441/F06/project1/chapter6.html

Your executable will be run as follows:
./ircserv <port> <password>

Everything in C++ 98.

# Man Pages

### [ socket ]

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

### [ close ]

	#include <unistd.h>

	int close(int fd);

https://man7.org/linux/man-pages/man2/close.2.html

closes a file descriptor, so that it no longer refers to
any file and may be reused.  Any record locks (see fcntl(2)) held
on the file it was associated with, and owned by the process, are
removed (regardless of the file descriptor that was used to
obtain the lock).

### [ setsockopt ]

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

### [ getsockname ]

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

### [ getprotobyname ]

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

### ~~[ gethostbyname ]~~

	#include <netdb.h>

**The gethostbyname(), gethostbyaddr(), herror(), and hstrerror()**
**functions are obsolete.  Applications should use getaddrinfo(3),**
**getnameinfo(3), and gai_strerror(3) instead.**

### [ getaddrinfo ]

	#include <sys/types.h>

	#include <sys/socket.h>

	#include <netdb.h>

	int getaddrinfo(const char *restrict node,
const char *restrict service,
const struct addrinfo *restrict hints,
struct addrinfo **restrict res);

Given node and service, which identify an Internet host and a
service, getaddrinfo() returns one or more addrinfo structures,
each of which contains an Internet address that can be specified
in a call to bind(2) or connect(2).  The getaddrinfo() function
combines the functionality provided by the gethostbyname(3) and
getservbyname(3) functions into a single interface, but unlike
the latter functions, getaddrinfo() is reentrant and allows
programs to eliminate IPv4-versus-IPv6 dependencies.

**reentrant:**
	In computing, a computer program or subroutine is called reentrant if multiple
	invocations can safely run concurrently on multiple processors, or on a single-processor system,
	where a reentrant procedure can be interrupted in the middle of its execution and then safely be called again
	("re-entered") before its previous invocations complete execution.

The addrinfo structure used by getaddrinfo() contains the
following fields:

    struct addrinfo {
        int              ai_flags;
        int              ai_family;
        int              ai_socktype;
        int              ai_protocol;
        socklen_t        ai_addrlen;
        struct sockaddr *ai_addr;
        char            *ai_canonname;
        struct addrinfo *ai_next;
    };

### [ freeaddrinfo ]

	#include <sys/types.h>

	#include <sys/socket.h>

	#include <netdb.h>

	void freeaddrinfo(struct addrinfo *res);

The freeaddrinfo() function shall free one or more addrinfo
structures returned by getaddrinfo(), along with any additional
storage associated with those structures. If the ai_next field of
the structure is not null, the entire list of structures shall be
freed. The freeaddrinfo() function shall support the freeing of
arbitrary sublists of an addrinfo list originally returned by
getaddrinfo().

### [ bind ]

	#include <sys/socket.h>

	int bind(int sockfd, const struct sockaddr *addr,
socklen_t addrlen);

When a socket is created with socket(2), it exists in a name
space (address family) but has no address assigned to it.  bind()
assigns the address specified by addr to the socket referred to
by the file descriptor sockfd.  addrlen specifies the size, in
bytes, of the address structure pointed to by addr.
Traditionally, this operation is called “assigning a name to a
socket”.

It is normally necessary to assign a local address using bind()
before a SOCK_STREAM socket may receive connections (see
accept(2)).

The actual structure passed for the addr argument will depend on
the address family.  The sockaddr structure is defined as
something like:

    struct sockaddr {
        sa_family_t sa_family;
        char        sa_data[ 14 ];
    }

The only purpose of this structure is to cast the structure
pointer passed in addr in order to avoid compiler warnings.

### [ connect ]

	#include <sys/socket.h>

	int connect(int sockfd, const struct sockaddr *addr,
socklen_t addrlen);

The connect() system call connects the socket referred to by the
file descriptor sockfd to the address specified by addr.  The
addrlen argument specifies the size of addr.  The format of the
address in addr is determined by the address space of the socket
sockfd; see socket(2) for further details.

If the socket sockfd is of type SOCK_DGRAM, then addr is the
address to which datagrams are sent by default, and the only
address from which datagrams are received.  If the socket is of
type SOCK_STREAM or SOCK_SEQPACKET, this call attempts to make a
connection to the socket that is bound to the address specified
by addr.

Some protocol sockets (e.g., UNIX domain stream sockets) may
successfully connect() only once.

Some protocol sockets (e.g., datagram sockets in the UNIX and
Internet domains) may use connect() multiple times to change
their association.

Some protocol sockets (e.g., TCP sockets as well as datagram
sockets in the UNIX and Internet domains) may dissolve the
association by connecting to an address with the sa_family member
of sockaddr set to AF_UNSPEC; thereafter, the socket can be
connected to another address.  (AF_UNSPEC is supported since
Linux 2.2.)


### [ listen ]
### [ accept ]
### [ htons ]
### [ htonl ]
### [ ntohs ]
### [ ntohl ]
### [ inet_addr ]
### [ inet_ntoa ]
### [ send ]
### [ recv ]
### [ signal ]
### [ sigaction ]
### [ lseek ]
### [ fstat ]
### [ fcntl ]
### [ poll ]

