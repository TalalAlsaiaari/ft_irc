# ABOUT

This project is about creating an IRC server.
An actual IRC client will be used to connect to the server and test it.

Internet Relay Chat or IRC is a text-based communication protocol on the Internet.
It offers real-time messaging that can be either public or private. Users can exchange
direct messages and join group channels.
IRC clients connect to IRC servers in order to join channels. IRC servers are connected
together to form a network.

The only network configuration allowed for IRC servers is that of a spanning tree where each server acts as a central node for the rest of the net it sees. <sup>[1](https://www.cs.cmu.edu/~srini/15-441/F06/project1/chapter1.html)</sup>

>                               [ Server 15 ]  [ Server 13 ] [ Server 14]
>                                     /                \         /
>                                    /                  \       /
>            [ Server 11 ] ------ [ Server 1 ]       [ Server 12]
>                                  /        \          /
>                                 /          \        /
>                      [ Server 2 ]          [ Server 3 ]
>                        /       \                      \
>                       /         \                      \
>               [ Server 4 ]    [ Server 5 ]         [ Server 6 ]
>                /    |    \                           /
>               /     |     \                         /
>              /      |      \____                   /
>             /       |           \                 /
>     [ Server 7 ] [ Server 8 ] [ Server 9 ]   [ Server 10 ]
>                                      :
>                                   [ etc. ]
>                                      :

> A client is anything connecting to a server that is not another server. Each client is distinguished from other clients by a unique nickname having a maximum length of nine (9) characters.
> In addition to the nickname, all servers must have the following information about all clients: the real name of the host that the client is running on, the username of the client on that host, and the server to which the client is connected. <sup>[1](https://www.cs.cmu.edu/~srini/15-441/F06/project1/chapter1.html)</sup>

# TO DO

- [x] Make awesome README
- [ ] Create Server
	- [ ] figure it out
- [ ] Parse Client Input

## Run irssi in Docker

	docker run -itd --name=irssi irssi
	docker exec -it irssi irssi
	/set nick <name>
	/connect liberachat
	/join #<channel_name>

## Run Executable

	./ircserv <port> <password>

### Get unique includes from README.md

	cat README.md | grep "#include" | sort -u > headerfile.hpp

Everything in C++ 98.

# Man Pages

<details>
	<summary><strong>[ socket ]</strong></summary>

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

</details>


<details>
	<summary><strong>[ close ]</strong></summary>

	#include <unistd.h>

	int close(int fd);

https://man7.org/linux/man-pages/man2/close.2.html

closes a file descriptor, so that it no longer refers to
any file and may be reused.  Any record locks (see fcntl(2)) held
on the file it was associated with, and owned by the process, are
removed (regardless of the file descriptor that was used to
obtain the lock).
</details>


<details>
	<summary><strong>[ setsockopt ]</strong></summary>

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
</details>

<details>
	<summary><strong>[ getsockname ]</strong></summary>

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
</details>

<details>
	<summary><strong>[ getprotobyname ]</strong></summary>

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
</details>

<details>
	<summary><strong><del>[ gethostbyname ]</del></strong></summary>

	#include <netdb.h>

**The gethostbyname(), gethostbyaddr(), herror(), and hstrerror()**
**functions are obsolete.  Applications should use getaddrinfo(3),**
**getnameinfo(3), and gai_strerror(3) instead.**
</details>

<details>
	<summary><strong>[ getaddrinfo ]</strong></summary>

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
</details>

<details>
	<summary><strong>[ freeaddrinfo ]</strong></summary>

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
</details>

<details>
	<summary><strong>[ bind ]</strong></summary>

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
</details>

<details>
	<summary><strong>[ connect ]</strong></summary>

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

</details>

<details>
	<summary><strong>[ listen ]</strong></summary>

	#include <sys/socket.h>

	int listen(int sockfd, int backlog);

listen() marks the socket referred to by sockfd as a passive
socket, that is, as a socket that will be used to accept incoming
connection requests using accept(2).

The sockfd argument is a file descriptor that refers to a socket
of type SOCK_STREAM or SOCK_SEQPACKET.

The backlog argument defines the maximum length to which the
queue of pending connections for sockfd may grow.  If a
connection request arrives when the queue is full, the client may
receive an error with an indication of ECONNREFUSED or, if the
underlying protocol supports retransmission, the request may be
ignored so that a later reattempt at connection succeeds.
</details>

<details>
	<summary><strong>[ accept ]</strong></summary>

	#include <sys/socket.h>

	int accept(int sockfd, struct sockaddr *_Nullable restrict addr,
socklen_t *_Nullable restrict addrlen);

The accept() system call is used with connection-based socket
types (SOCK_STREAM, SOCK_SEQPACKET).  It extracts the first
connection request on the queue of pending connections for the
listening socket, sockfd, creates a new connected socket, and
returns a new file descriptor referring to that socket.  The
newly created socket is not in the listening state.  The original
socket sockfd is unaffected by this call.

**In order to be notified of incoming connections on a socket, you
can use select(2), poll(2), or epoll(7).  A readable event will
be delivered when a new connection is attempted and you may then
call accept() to get a socket for that connection.
Alternatively, you can set the socket to deliver SIGIO when
activity occurs on a socket; see socket(7) for details.**
</details>

<details>
	<summary><strong>[ htons ]</strong></summary>

	#include <arpa/inet.h>

	uint16_t htons(uint16_t hostshort);

converts the unsigned short integer
hostshort from host byte order to network byte order.
</details>

<details>
	<summary><strong>[ htonl ]</strong></summary>

	#include <arpa/inet.h>

	uint32_t htonl(uint32_t hostlong);

converts the unsigned integer hostlong from
host byte order to network byte order.
</details>

<details>
	<summary><strong>[ ntohs ]</strong></summary>

	#include <arpa/inet.h>

	uint16_t ntohs(uint16_t netshort);

converts the unsigned short integer netshort
from network byte order to host byte order.
</details>

<details>
	<summary><strong>[ ntohl ]</strong></summary>

	#include <arpa/inet.h>

	uint32_t ntohl(uint32_t netlong);

converts the unsigned integer netlong from
network byte order to host byte order.
</details>

<details>
	<summary><strong>[ inet_addr ]</strong></summary>

	#include <arpa/inet.h>

	in_addr_t inet_addr(const char *cp);

convert the string pointed to by
cp, in the standard IPv4 dotted decimal notation, to an integer
value suitable for use as an Internet address.
</details>

<details>
	<summary><strong>[ inet_ntoa ]</strong></summary>

	#include <arpa/inet.h>

	char *inet_ntoa(struct in_addr in);

convert the Internet host address
specified by in to a string in the Internet standard dot
notation.

The return value of inet_ntoa() may point to static data that may
be overwritten by subsequent calls to inet_ntoa().
</details>

<details>
	<summary><strong>[ send ]</strong></summary>

	#include <sys/socket.h>

	ssize_t send(int sockfd, const void buf[.len], size_t len, int flags);

The send() call may be used only when the socket is in a
connected state (so that the intended recipient is known).  The
only difference between send() and write(2) is the presence of
flags.  With a zero flags argument, send() is equivalent to
write(2).  Also, the following call

	send(sockfd, buf, len, flags);

	is equivalent to

	sendto(sockfd, buf, len, flags, NULL, 0);

The argument sockfd is the file descriptor of the sending socket.

</details>

<details>
	<summary><strong>[ recv ]</strong></summary>

	#include <sys/socket.h>

	ssize_t recv(int sockfd, void buf[.len], size_t len,
int flags);

The only difference between recv() and read(2) is the presence of
flags.  With a zero flags argument, recv() is generally
equivalent to read(2) (but see NOTES).  Also, the following call

	recv(sockfd, buf, len, flags);

	is equivalent to

	recvfrom(sockfd, buf, len, flags, NULL, NULL);

The recv() call is normally used only on a connected socket (see
connect(2)).  It is equivalent to the call:

	recvfrom(fd, buf, len, flags, NULL, 0);

</details>

<details>
	<summary><strong>[ signal ]</strong></summary>

	#include <signal.h>

	typedef void (*sighandler_t)(int);

	sighandler_t signal(int signum, sighandler_t handler);

WARNING: the behavior of signal() varies across UNIX versions,
and has also varied historically across different versions of
Linux.  Avoid its use: use sigaction(2) instead.  See Portability
below.

	Portability

	The only portable use of signal() is to set a signal's
	disposition to SIG_DFL or SIG_IGN.  The semantics when using
	signal() to establish a signal handler vary across systems (and
	POSIX.1 explicitly permits this variation); do not use it for
	this purpose.

**The signals SIGKILL and SIGSTOP cannot be caught or ignored.**

</details>

<details>
	<summary><strong>[ sigaction ]</strong></summary>

	#include <signal.h>

	int sigaction(int signum,
const struct sigaction *_Nullable restrict act,
struct sigaction *_Nullable restrict oldact);

The sigaction structure is defined as something like:

	struct sigaction {
	    void     (*sa_handler)(int);
	    void     (*sa_sigaction)(int, siginfo_t *, void *);
	    sigset_t   sa_mask;
	    int        sa_flags;
	    void     (*sa_restorer)(void);
	};

The siginfo_t data type is a structure with the following fields:

	siginfo_t {
	    int      si_signo;     /* Signal number */
	    int      si_errno;     /* An errno value */
	    int      si_code;      /* Signal code */
	    int      si_trapno;    /* Trap number that caused
	                              hardware-generated signal
	                              (unused on most architectures) */
	    pid_t    si_pid;       /* Sending process ID */
	    uid_t    si_uid;       /* Real user ID of sending process */
	    int      si_status;    /* Exit value or signal */
	    clock_t  si_utime;     /* User time consumed */
	    clock_t  si_stime;     /* System time consumed */
	    union sigval si_value; /* Signal value */
	    int      si_int;       /* POSIX.1b signal */
	    void    *si_ptr;       /* POSIX.1b signal */
	    int      si_overrun;   /* Timer overrun count;
	                              POSIX.1b timers */
	    int      si_timerid;   /* Timer ID; POSIX.1b timers */
	    void    *si_addr;      /* Memory location which caused fault */
	    long     si_band;      /* Band event (was int in
	                              glibc 2.3.2 and earlier) */
	    int      si_fd;        /* File descriptor */
	    short    si_addr_lsb;  /* Least significant bit of address
	                              (since Linux 2.6.32) */
	    void    *si_lower;     /* Lower bound when address violation
	                              occurred (since Linux 3.19) */
	    void    *si_upper;     /* Upper bound when address violation
	                              occurred (since Linux 3.19) */
	    int      si_pkey;      /* Protection key on PTE that caused
	                              fault (since Linux 4.6) */
	    void    *si_call_addr; /* Address of system call instruction
	                              (since Linux 3.5) */
	    int      si_syscall;   /* Number of attempted system call
	                              (since Linux 3.5) */
	    unsigned int si_arch;  /* Architecture of attempted system call
	                              (since Linux 3.5) */
	}
</details>

<details>
	<summary><strong>[ lseek ]</strong></summary>

	#include <unistd.h>

	off_t lseek(int fd, off_t offset, int whence);

repositions the file offset of the open file description
associated with the file descriptor fd to the argument offset
according to the directive whence as follows:

	SEEK_SET
	       The file offset is set to offset bytes.

	SEEK_CUR
	       The file offset is set to its current location plus offset
	       bytes.

	SEEK_END
	       The file offset is set to the size of the file plus offset
	       bytes.

allows the file offset to be set beyond the end of the
file (but this does not change the size of the file).  If data is
later written at this point, subsequent reads of the data in the
gap (a "hole") return null bytes ('\0') until data is actually
written into the gap.
</details>

<details>
	<summary><strong>[ fstat ]</strong></summary>

	#include <sys/stat.h>

	int fstat(int fildes, struct stat *buf);

obtain information about an open file
associated with the file descriptor fildes, and shall write it to
the area pointed to by buf.
</details>

<details>
	<summary><strong>[ fcntl ]</strong></summary>

	#include <fcntl.h>

	int fcntl(int fd, int cmd, ... /* arg */ );

performs one of the operations described below on the
open file descriptor fd.  The operation is determined by cmd.

F_SETFL (int)

	Set the file status flags to the value specified by arg.
	File access mode (O_RDONLY, O_WRONLY, O_RDWR) and file
	creation flags (i.e., O_CREAT, O_EXCL, O_NOCTTY, O_TRUNC)
	in arg are ignored.  On Linux, this command can change
	only the O_APPEND, O_ASYNC, O_DIRECT, O_NOATIME, and
	O_NONBLOCK flags.  It is not possible to change the
	O_DSYNC and O_SYNC flags; see BUGS, below.
</details>

<details>
	<summary><strong>[ poll ]</strong></summary>

	#include <poll.h>

	int poll(struct pollfd *fds, nfds_t nfds, int timeout);

performs a similar task to select(2): it waits for one of
a set of file descriptors to become ready to perform I/O.  The
Linux-specific epoll(7) API performs a similar task, but offers
features beyond those found in poll().

The set of file descriptors to be monitored is specified in the
fds argument, which is an array of structures of the following
form:

	struct pollfd {
	    int   fd;         /* file descriptor */
	    short events;     /* requested events */
	    short revents;    /* returned events */
	};

The caller should specify the number of items in the fds array in
nfds.

The bits that may be set/returned in events and revents are
defined in <poll.h>:

	POLLIN
	There is data to read

	POLLOUT
	Writing is now possible, though a write larger than the
	available space in a socket or pipe will still block
	(unless O_NONBLOCK is set).

	POLLERR
	Error condition (only returned in revents; ignored in
	events).  This bit is also set for a file descriptor
	referring to the write end of a pipe when the read end has
	been closed.

	POLLHUP
	Hang up (only returned in revents; ignored in events).
	Note that when reading from a channel such as a pipe or a
	stream socket, this event merely indicates that the peer
	closed its end of the channel.  Subsequent reads from the
	channel will return 0 (end of file) only after all
	outstanding data in the channel has been consumed.
</details>

# REFERENCES

[ 1 ]  <https://www.cs.cmu.edu/~srini/15-441/F06/project1/chapter1.html>

[ 2 ] <https://www.bogotobogo.com/cplusplus/sockets_server_client.php>

[ 3 ] <https://www.geeksforgeeks.org/socket-programming-cc/>
