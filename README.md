# ABOUT

This project is about creating an IRC server.
An actual IRC client will be used to connect to the server and test it.

Internet Relay Chat or IRC is a text-based communication protocol on the Internet.
It offers real-time messaging that can be either public or private. Users can exchange
direct messages and join group channels.
IRC clients connect to IRC servers in order to join channels. IRC servers are connected
together to form a network.

> The only network configuration allowed for IRC servers is that of a spanning tree where each server acts as a central node for the rest of the net it sees. <sup>[1](https://www.cs.cmu.edu/~srini/15-441/F06/project1/chapter1.html)</sup>

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
> In addition to the nickname, all servers must have the following information about all clients: the real name of the host that the client is running on, the username of the client on that host, and the server to which the client is connected.
> The real name of the client's host should be obtained via a reverse lookup of the client's IP address. If the reverse lookup fails, use the IP address as the real name.<sup>[1](https://www.cs.cmu.edu/~srini/15-441/F06/project1/chapter1.html)</sup>
> For this project we only focus on client-server communication.


### What is a socket? <sup>[7](https://beej.us/guide/bgnet/)</sup>

They are a way to speak to other programs using standard UNIX file descriptors. UNIX programs use file descriptors for any sort of
I/O. File descriptors are an integer associated with an open file, but that file can be a network connection, a FIFO, a pipe, a terminal, a real on-the-disk file, or just about
anything else. **EVERYTHING IS A FILE DESCRIPTOR.**

# TO DO

- [x] Make awesome README
- [ ] Create Server _(Talal)_
	- [ ] Simple Server with one client
	- [ ] Server accepting multiple clients
- [ ] Parse Client Input
	- [ ] error handling (wrong command/not enough parmas/etc.)
- [ ] Create server operators
- [ ] Create server functions
	- [ ] CAP registration
	- [ ] PASS
	- [ ] NICK
	- [ ] USER
	- [ ] PING/PONG
	- [ ] PRIVMSG
	- [ ] MODE
	- [ ] WHO/WHOIS
- [ ] create channels
	- [ ] create channel operators
	- [ ] forward channel messages to all clients in channel
	- [ ] create channel functions
		- [ ] JOIN
		- [ ] PART
		- [ ] INVITE
		- [ ] KICK
		- [ ] TOPIC
		- [ ] MODE
			- [ ] i (Set/remove Invite-only channel)
			- [ ] t (Set/remove the restrictions of the TOPIC command to channel operators)
			- [ ] k (Set/remove the channel key (password))
			- [ ] o (Give/take channel operator privilege)
			- [ ] l (Set/remove the user limit to channel)

### Requirements

	- You must have operators and regular users.
		- To allow a reasonable amount of order to be kept within the IRC
   		network, a special class of clients (operators) is allowed to perform
   		general maintenance functions on the network.  Although the powers
   		granted to an operator can be considered as 'dangerous', they are
   		nonetheless required.  Operators should be able to perform basic
   		network tasks such as disconnecting and reconnecting servers as
   		needed to prevent long-term use of bad network routing.  In
   		recognition of this need, the protocol discussed herein provides for
   		operators only to be able to perform such functions.  See sections
   		4.1.7 (SQUIT) and 4.3.5 (CONNECT).
   		A more controversial power of operators is the ability  to  remove  a
   		user  from  the connected network by 'force', i.e. operators are able
   		to close the connection between any client and server.   The
   		justification for  this  is delicate since its abuse is both
   		destructive and annoying.  For further details on this type of
   		action, see section 4.6.1 (KILL).

## Run irssi in Docker
``` bash
	docker run -it --name irssi --network host irssi
	/set nick <name>
	/connect host.docker.internal <port>
	/join #<channel_name>
	/msg <nick> <message>
```
## Run Executable
``` bash
	./ircserv <port> <password>
```
### Get unique includes from README.md
``` bash
	cat README.md | grep "#include" | sort -u > headerfile.hpp
```
# Man Pages

<details>
	<summary><strong>[ socket ]</strong></summary>

``` c
	#include <sys/socket.h>

	int socket(int domain, int type, int protocol);

	tcp_socket = socket(AF_INET6, SOCK_STREAM, 0);
```

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

``` c
	#include <unistd.h>

	int close(int fd);
```

closes a file descriptor, so that it no longer refers to
any file and may be reused.  Any record locks (see fcntl(2)) held
on the file it was associated with, and owned by the process, are
removed (regardless of the file descriptor that was used to
obtain the lock).
</details>


<details>
	<summary><strong>[ setsockopt ]</strong></summary>

``` c
	#include <sys/socket.h>

	int setsockopt(int socket, int level, int option_name, const void *option_value, socklen_t option_len);
```

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

``` c
	#include <sys/socket.h>

	int getsockname(int sockfd, struct sockaddr *restrict addr, socklen_t *restrict addrlen);
```

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

``` c
	#include <netdb.h>

	struct protoent *getprotobyname(const char *name);
```

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

``` c
    struct protoent {
        char  *p_name;       /* official protocol name */
        char **p_aliases;    /* alias list */
        int    p_proto;      /* protocol number */
    }
```

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

``` c
	#include <sys/types.h>

	#include <sys/socket.h>

	#include <netdb.h>

	int getaddrinfo(const char *restrict node, const char *restrict service, const struct addrinfo *restrict hints, struct addrinfo **restrict res);
```

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

``` c
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
```

</details>

<details>
	<summary><strong>[ freeaddrinfo ]</strong></summary>

``` c
	#include <sys/types.h>

	#include <sys/socket.h>

	#include <netdb.h>

	void freeaddrinfo(struct addrinfo *res);
```

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

``` c
	#include <sys/socket.h>

	int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
```

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

``` c
    struct sockaddr {
        sa_family_t sa_family;
        char        sa_data[ 14 ];
    }
```

The only purpose of this structure is to cast the structure
pointer passed in addr in order to avoid compiler warnings.
</details>

<details>
	<summary><strong>[ connect ]</strong></summary>

``` c
	#include <sys/socket.h>

	int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
```

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

``` c
	#include <sys/socket.h>

	int listen(int sockfd, int backlog);
```

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

``` c
	#include <sys/socket.h>

	int accept(int sockfd, struct sockaddr *_Nullable restrict addr, socklen_t *_Nullable restrict addrlen);
```

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

``` c
	#include <arpa/inet.h>

	uint16_t htons(uint16_t hostshort);
```

converts the unsigned short integer
hostshort from host byte order to network byte order.
</details>

<details>
	<summary><strong>[ htonl ]</strong></summary>

``` c
	#include <arpa/inet.h>

	uint32_t htonl(uint32_t hostlong);
```
converts the unsigned integer hostlong from
host byte order to network byte order. (see Big-endian / little-endian)
</details>

<details>
	<summary><strong>[ ntohs ]</strong></summary>

``` c
	#include <arpa/inet.h>

	uint16_t ntohs(uint16_t netshort);
```

converts the unsigned short integer netshort
from network byte order to host byte order.
</details>

<details>
	<summary><strong>[ ntohl ]</strong></summary>

``` c
	#include <arpa/inet.h>

	uint32_t ntohl(uint32_t netlong);
```

converts the unsigned integer netlong from
network byte order to host byte order.
</details>

<details>
	<summary><strong>[ inet_addr ]</strong></summary>

``` c
	#include <arpa/inet.h>

	in_addr_t inet_addr(const char *cp);
```

convert the string pointed to by
cp, in the standard IPv4 dotted decimal notation, to an integer
value suitable for use as an Internet address.
</details>

<details>
	<summary><strong>[ inet_ntoa ]</strong></summary>

``` c
	#include <arpa/inet.h>

	char *inet_ntoa(struct in_addr in);
```

convert the Internet host address
specified by in to a string in the Internet standard dot
notation.

The return value of inet_ntoa() may point to static data that may
be overwritten by subsequent calls to inet_ntoa().
</details>

<details>
	<summary><strong>[ send ]</strong></summary>

``` c
	#include <sys/socket.h>

	ssize_t send(int sockfd, const void buf[.len], size_t len, int flags);
```

The send() call may be used only when the socket is in a
connected state (so that the intended recipient is known).  The
only difference between send() and write(2) is the presence of
flags.  With a zero flags argument, send() is equivalent to
write(2).  Also, the following call

``` c
	send(sockfd, buf, len, flags);

	// is equivalent to

	sendto(sockfd, buf, len, flags, NULL, 0);
```

The argument sockfd is the file descriptor of the sending socket.

</details>

<details>
	<summary><strong>[ recv ]</strong></summary>

``` c
	#include <sys/socket.h>

	ssize_t recv(int sockfd, void buf[.len], size_t len, int flags);
```

The only difference between recv() and read(2) is the presence of
flags.  With a zero flags argument, recv() is generally
equivalent to read(2) (but see NOTES).  Also, the following call

``` c
	recv(sockfd, buf, len, flags);

	// is equivalent to

	recvfrom(sockfd, buf, len, flags, NULL, NULL);
```

The recv() call is normally used only on a connected socket (see
connect(2)).  It is equivalent to the call:

	recvfrom(fd, buf, len, flags, NULL, 0);

MSG_DONTWAIT (since Linux 2.2)
	Enables nonblocking operation; if the operation would
	block, the call fails with the error EAGAIN or
	EWOULDBLOCK.  This provides similar behavior to setting
	the O_NONBLOCK flag (via the fcntl(2) F_SETFL operation),
	but differs in that MSG_DONTWAIT is a per-call option,
	whereas O_NONBLOCK is a setting on the open file
	description (see open(2)), which will affect all threads
	in the calling process and as well as other processes that
	hold file descriptors referring to the same open file
	description.

</details>

<details>
	<summary><strong>[ signal ]</strong></summary>

``` c
	#include <signal.h>

	typedef void (*sighandler_t)(int);

	sighandler_t signal(int signum, sighandler_t handler);
```

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

``` c
	#include <signal.h>

	int sigaction(int signum, const struct sigaction *_Nullable restrict act, struct sigaction *_Nullable restrict oldact);
```

The sigaction structure is defined as something like:

``` c
	struct sigaction {
	    void     (*sa_handler)(int);
	    void     (*sa_sigaction)(int, siginfo_t *, void *);
	    sigset_t   sa_mask;
	    int        sa_flags;
	    void     (*sa_restorer)(void);
	};
```

The siginfo_t data type is a structure with the following fields:

``` c
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
```
</details>

<details>
	<summary><strong>[ lseek ]</strong></summary>

``` c
	#include <unistd.h>

	off_t lseek(int fd, off_t offset, int whence);
```

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

``` c
	#include <sys/stat.h>

	int fstat(int fildes, struct stat *buf);
```

obtain information about an open file
associated with the file descriptor fildes, and shall write it to
the area pointed to by buf.
</details>

<details>
	<summary><strong>[ fcntl ]</strong></summary>

``` c
	#include <fcntl.h>

	int fcntl(int fd, int cmd, ... /* arg */ );
```

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

``` c
	#include <poll.h>

	int poll(struct pollfd *fds, nfds_t nfds, int timeout);
```

performs a similar task to select(2): it waits for one of
a set of file descriptors to become ready to perform I/O.  The
Linux-specific epoll(7) API performs a similar task, but offers
features beyond those found in poll().

The set of file descriptors to be monitored is specified in the
fds argument, which is an array of structures of the following
form:

``` c
	struct pollfd {
	    int   fd;         /* file descriptor */
	    short events;     /* requested events */
	    short revents;    /* returned events */
	};
```

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

# Connection registration

The recommended order of commands during registration is as follows:

1. CAP LS 302
2. PASS
3. NICK and USER
4. Capability Negotiation
5. SASL (if negotiated)
6. CAP END

> Upon successful completion of the registration process, the server MUST send, in this order, the RPL_WELCOME (001), RPL_YOURHOST (002), RPL_CREATED (003), RPL_MYINFO (004), and at least one RPL_ISUPPORT (005) numeric to the client.

# Random Info dump

From Karim use rawlog to see commands and expected replies "/help rawlog"

inside irssi container connect to liberachat
```bash
	/connect liberachat
```

after connecting to liberachat
``` bash
	/rawlog save <filename>.log
```

run in terminal to connect to docker container
``` bash
	docker exec -it <container_name> sh
```

once in the container
``` bash
	cat <filename>.log
```

to update rawlog after running more commands use the rawlog save command in irssi again then cat the file from the other terminal

example from rawlog from liberachat

messages starting with "<<" are from the client to the server

messages starting with ">>" are from the server to the client


	> << CAP LS 302
	> << JOIN :
	> >> :lead.libera.chat NOTICE * :*** Checking Ident
	> >> :lead.libera.chat NOTICE * :*** Looking up your hostname...
	> >> :lead.libera.chat NOTICE * :*** Couldn't look up your hostname
	> >> :lead.libera.chat NOTICE * :*** No Ident response
	> >> :lead.libera.chat CAP * LS :account-notify away-notify chghost extended-join multi-prefix sasl=PLAIN,ECDSA-NIST256P-CHALLENGE,EXTERNAL tls account-tag cap-notify echo-message server-time solanum.chat/identify-msg solanum.chat/oper solanum.chat/realhost
	> << CAP REQ :multi-prefix extended-join away-notify chghost account-notify server-time
	> >> :lead.libera.chat 451 * :You have not registered
	> >> :lead.libera.chat CAP * ACK :multi-prefix extended-join away-notify chghost account-notify server-time
	> << CAP END
	> << NICK aballerisabigmotherfuckerandyoucantstopme
	> << USER user user irc.libera.chat :Unknown
	> >> @time=2023-07-20T17:51:26.293Z :lead.libera.chat 001 aballerisabigmot :Welcome to the Libera.Chat Internet Relay Chat Network aballerisabigmot
	> >> @time=2023-07-20T17:51:26.293Z :lead.libera.chat 002 aballerisabigmot :Your host is lead.libera.chat[94.125.182.252/6697], running version solanum-1.0-dev
	> >> @time=2023-07-20T17:51:26.293Z :lead.libera.chat 003 aballerisabigmot :This server was created Fri Jun 16 2023 at 22:50:54 UTC
	> >> @time=2023-07-20T17:51:26.293Z :lead.libera.chat 004 aballerisabigmot lead.libera.chat solanum-1.0-dev DGMQRSZaghilopsuwz CFILMPQRSTbcefgijklmnopqrstuvz bkloveqjfI
	> >> @time=2023-07-20T17:51:26.293Z :lead.libera.chat 005 aballerisabigmot FNC WHOX CALLERID=g ETRACE MONITOR=100 KNOCK SAFELIST ELIST=CMNTU CHANTYPES=# EXCEPTS INVEX CHANMODES=eIbq,k,flj,CFLMPQRSTcgimnprstuz :are supported by this server
	> >> @time=2023-07-20T17:51:26.293Z :lead.libera.chat 005 aballerisabigmot CHANLIMIT=#:250 PREFIX=(ov)@+ MAXLIST=bqeI:100 MODES=4 NETWORK=Libera.Chat STATUSMSG=@+ CASEMAPPING=rfc1459 NICKLEN=16 MAXNICKLEN=16 CHANNELLEN=50 TOPICLEN=390 DEAF=D :are supported by this server
	> >> @time=2023-07-20T17:51:26.293Z :lead.libera.chat 005 aballerisabigmot TARGMAX=NAMES:1,LIST:1,KICK:1,WHOIS:1,PRIVMSG:4,NOTICE:4,ACCEPT:,MONITOR: EXTBAN=$,agjrxz :are supported by this server
	> >> @time=2023-07-20T17:51:26.293Z :lead.libera.chat 251 aballerisabigmot :There are 64 users and 44076 invisible on 28 servers
	> >> @time=2023-07-20T17:51:26.293Z :lead.libera.chat 252 aballerisabigmot 33 :IRC Operators online
	> >> @time=2023-07-20T17:51:26.293Z :lead.libera.chat 253 aballerisabigmot 66 :unknown connection(s)
	> >> @time=2023-07-20T17:51:26.293Z :lead.libera.chat 254 aballerisabigmot 23392 :channels formed
	> >> @time=2023-07-20T17:51:26.293Z :lead.libera.chat 255 aballerisabigmot :I have 2355 clients and 1 servers
	> >> @time=2023-07-20T17:51:26.293Z :lead.libera.chat 265 aballerisabigmot 2355 3239 :Current local users 2355, max 3239
	> >> @time=2023-07-20T17:51:26.293Z :lead.libera.chat 266 aballerisabigmot 44140 45718 :Current global users 44140, max 45718
	> >> @time=2023-07-20T17:51:26.293Z :lead.libera.chat 250 aballerisabigmot :Highest connection count: 3240 (3239 clients) (104844 connections received)
	> >> @time=2023-07-20T17:51:26.293Z :lead.libera.chat 375 aballerisabigmot :- lead.libera.chat Message of the Day -
	> >> @time=2023-07-20T17:51:26.293Z :lead.libera.chat 372 aballerisabigmot :- Welcome to Libera Chat, the IRC network for
	> >> @time=2023-07-20T17:51:26.293Z :lead.libera.chat 372 aballerisabigmot :- free & open-source software and peer directed projects.
	> >> @time=2023-07-20T17:51:26.293Z :lead.libera.chat 372 aballerisabigmot :-
	> >> @time=2023-07-20T17:51:26.293Z :lead.libera.chat 372 aballerisabigmot :- Use of Libera Chat is governed by our network policies.
	> >> @time=2023-07-20T17:51:26.293Z :lead.libera.chat 372 aballerisabigmot :-
	> >> @time=2023-07-20T17:51:26.293Z :lead.libera.chat 372 aballerisabigmot :- To reduce network abuses we perform open proxy checks
	> >> @time=2023-07-20T17:51:26.293Z :lead.libera.chat 372 aballerisabigmot :- on hosts at connection time.
	> >> @time=2023-07-20T17:51:26.293Z :lead.libera.chat 372 aballerisabigmot :-
	> >> @time=2023-07-20T17:51:26.293Z :lead.libera.chat 372 aballerisabigmot :- Please visit us in #libera for questions and support.
	> >> @time=2023-07-20T17:51:26.293Z :lead.libera.chat 372 aballerisabigmot :-
	> >> @time=2023-07-20T17:51:26.293Z :lead.libera.chat 372 aballerisabigmot :- Website and documentation:  https://libera.chat
	> >> @time=2023-07-20T17:51:26.293Z :lead.libera.chat 372 aballerisabigmot :- Webchat:                    https://web.libera.chat
	> >> @time=2023-07-20T17:51:26.293Z :lead.libera.chat 372 aballerisabigmot :- Network policies:           https://libera.chat/policies
	> >> @time=2023-07-20T17:51:26.293Z :lead.libera.chat 372 aballerisabigmot :- Email:                      support@libera.chat
	> >> @time=2023-07-20T17:51:26.293Z :lead.libera.chat 376 aballerisabigmot :End of /MOTD command.
	> >> @time=2023-07-20T17:51:26.293Z :aballerisabigmot MODE aballerisabigmot :+Ziw
	> << MODE aballerisabigmot +i
	> << WHOIS aballerisabigmotherfuckerandyoucantstopme
	> >> @time=2023-07-20T17:51:32.010Z :lead.libera.chat 401 aballerisabigmot aballerisabigmotherfuckerandyoucantstopme :No such nick/channel
	> --> event empty
	> >> @time=2023-07-20T17:51:32.010Z :lead.libera.chat 318 aballerisabigmot aballerisabigmotherfuckerandyoucantstopme :End of /WHOIS list.
	> --> event empty
	> << PING lead.libera.chat
	> >> @time=2023-07-20T17:51:36.853Z :lead.libera.chat PONG lead.libera.chat :lead.libera.chat
	> --> lag pong

	Because of IRC's scandanavian origin, the characters {}| are
	considered to be the lower case equivalents of the characters []\,
	respectively. This is a critical issue when determining the
	equivalence of two nicknames.

# Parsing

	The protocol messages must be extracted from the contiguous stream of
	octets.  The current solution is to designate two characters, CR and
	LF, as message separators.   Empty  messages  are  silently  ignored,
	which permits  use  of  the  sequence  CR-LF  between  messages
	without extra problems.
	The extracted message is parsed into the components <prefix>,
	<command> and list of parameters matched either by <middle> or
	<trailing> components. <sup>[8](https://www.rfc-editor.org/rfc/rfc1459)</sup>

	The BNF representation for this is:

``` bnf
	<message>  ::= [':' <prefix> <SPACE> ] <command> <params> <crlf>
	<prefix>   ::= <servername> | <nick> [ '!' <user> ] [ '@' <host> ]
	<command>  ::= <letter> { <letter> } | <number> <number> <number>
	<SPACE>    ::= ' ' { ' ' }
	<params>   ::= <SPACE> [ ':' <trailing> | <middle> <params> ]

	<middle>   ::= <Any *non-empty* sequence of octets not including SPACE
	               or NUL or CR or LF, the first of which may not be ':'>
	<trailing> ::= <Any, possibly *empty*, sequence of octets not including
	                 NUL or CR or LF>

	<crlf>     ::= CR LF
```
  1)    <SPACE> is consists only of SPACE character(s) (0x20).
        Specially notice that TABULATION, and all other control
        characters are considered NON-WHITE-SPACE.

  2)    After extracting the parameter list, all parameters are equal,
        whether matched by <middle> or <trailing>. <Trailing> is just
        a syntactic trick to allow SPACE within parameter.

  3)    The fact that CR and LF cannot appear in parameter strings is
        just artifact of the message framing. This might change later.

  4)    The NUL character is not special in message framing, and
        basically could end up inside a parameter, but as it would
        cause extra complexities in normal C string handling. Therefore
        NUL is not allowed within messages.

  5)    The last parameter may be an empty string.

  6)    Use of the extended prefix (['!' <user> ] ['@' <host> ]) must
        not be used in server to server communications and is only
        intended for server to client messages in order to provide
        clients with more useful information about who a message is
        from without the need for additional queries.

   Most protocol messages specify additional semantics and syntax for
   the extracted parameter strings dictated by their position in the
   list.  For example, many server commands will assume that the first
   parameter after the command is the list of targets, which can be
   described with:

   <target>     ::= <to> [ "," <target> ]
   <to>         ::= <channel> | <user> '@' <servername> | <nick> | <mask>
   <channel>    ::= ('#' | '&') <chstring>
   <servername> ::= <host>
   <host>       ::= see RFC 952 [DNS:4] for details on allowed hostnames
   <nick>       ::= <letter> { <letter> | <number> | <special> }
   <mask>       ::= ('#' | '$') <chstring>
   <chstring>   ::= <any 8bit code except SPACE, BELL, NUL, CR, LF and
                     comma (',')>

   Other parameter syntaxes are:

   <user>       ::= <nonwhite> { <nonwhite> }
   <letter>     ::= 'a' ... 'z' | 'A' ... 'Z'
   <number>     ::= '0' ... '9'
   <special>    ::= '-' | '[' | ']' | '\' | '`' | '^' | '{' | '}'
   <nonwhite>   ::= <any 8bit code except SPACE (0x20), NUL (0x0), CR
                     (0xd), and LF (0xa)>


![PRIVMSG](http://chi.cs.uchicago.edu/_images/privmsg.png) <sup>[ [10] ](http://chi.cs.uchicago.edu/chirc/irc_examples.html)</sup>

# REFERENCES

[ 1 ]  <https://www.cs.cmu.edu/~srini/15-441/F06/project1/chapter1.html>

[ 2 ] <https://www.bogotobogo.com/cplusplus/sockets_server_client.php>

[ 3 ] <https://www.geeksforgeeks.org/socket-programming-cc/>

[ 4 ] <https://man7.org/linux/man-pages/>

[ 5 ] <https://marketsplash.com/tutorials/cpp/cplusplus-scoket/>

[ 6 ] <https://irssi.org/documentation>

[ 7 ] <https://beej.us/guide/bgnet/>

first documentation for IRC protocol
[ 8 ] <https://www.rfc-editor.org/rfc/rfc1459>

CAP LS command and client registration
[ 9 ] <https://ircv3.net/specs/extensions/capability-negotiation.html>

[ 10 ] <http://chi.cs.uchicago.edu/chirc/irc_examples.html>

more recent documentation for IRC protocol
[ 11 ] <https://www.rfc-editor.org/rfc/rfc2813>

[ 12 ] <https://ircv3.net/registry#capabilities>


modern irc docs (very useful)
[ 13 ] <https://modern.ircdocs.horse/>
