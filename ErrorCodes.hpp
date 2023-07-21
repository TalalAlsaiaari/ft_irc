#pragma once 

// Definitions of IRC error codes


/*  - Used to indicate the nickname parameter supplied to a command is currently unused.
* @return "<nickname> :No such nick/channel"*/
#define ERR_NOSUCHNICK 401

/*  - Used to indicate the server name given currently doesn't exist.
* @return "<server name> :No such server"*/
#define ERR_NOSUCHSERVER 402

/*  - Used to indicate the given channel name is invalid.
* @return "<channel name> :No such channel"*/
#define ERR_NOSUCHCHANNEL 403

/*  - Sent to a user who is either (a) not on a channel which is mode +n or (b) not a chanop (or mode +v) on a channel
* @return "<channel name> :Cannot send to channel"*/
#define ERR_CANNOTSENDTOCHAN 404

/*  - Sent to a user when they have joined the maximum number of allowed channels and they try to join another channel.
* @return "<channel name> :You have joined too many channels"*/
#define ERR_TOOMANYCHANNELS 405

/*  - Returned by WHOWAS to indicate there is no history information for that nickname.
* @return "<nickname> :There was no such nickname"*/
#define ERR_WASNOSUCHNICK 406

/*  - Returned to a client which is attempting to send PRIVMSG/NOTICE using the user\@host destination format and for a user\@host which has several occurrences.
* @return "<mask> :<reason>"*/
#define ERR_TOOMANYTARGETS 407

/*  - PING or PONG message missing the originator parameter which is required since these commands must work without valid prefixes.
* @return ":No origin specified"*/
#define ERR_NOORIGIN 409

/*  - 501 - 504 are returned by WHOWAS to indicate there is no history information for that nickname or channel.
* @return "<server name> :No such server"*/
#define ERR_NORECIPIENT 411

/*  - Returned when no recipient is given with a command.
* @return ":No text to send"*/
#define ERR_NOTEXTTOSEND 412

/*  - Used to indicate the server name given currently doesn't exist.
* @return "<server name> :No such server"*/
#define ERR_NOTOPLEVEL 413

/*  - Used to indicate the given channel name is invalid.
* @return "<channel name> :No such channel"*/
#define ERR_WILDTOPLEVEL 414

/*  - Used to indicate the given command is unknown to the server.
* @return "<command> :Unknown command"*/
#define ERR_UNKNOWNCOMMAND 421

/*  - Returned when there is no MOTD to send the client.
* @return ":MOTD File is missing"*/
#define ERR_NOMOTD 422

/*  - Returned by a server in response to an ADMIN message when there is an error in finding the appropriate information.
* @return "<server> :No administrative info available"*/
#define ERR_NOADMININFO 423

/*  - Generic error message used to report a failed file operation during the processing of a message.
* @return ":File error doing <file op> on <file>"*/
#define ERR_FILEERROR 424

/*  - Returned when a nickname parameter expected for a command and isn't found.
* @return ":No nickname given"*/
#define ERR_NONICKNAMEGIVEN 431

/*  - Returned after receiving a NICK message which contains characters which do not fall in the defined set. See section x.x.x for details on valid nicknames.
* @return "<nick> :Erroneous nickname"*/
#define ERR_ERRONEUSNICKNAME 432

/*  - Returned when a NICK message is processed that results in an attempt to change to a currently existing nickname.
* @return "<nick> :Nickname is already in use"*/
#define ERR_NICKNAMEINUSE 433

/*  - Returned by a server to a client when it detects a nickname collision (registered of a NICK that already exists by another server).
* @return "<nick> :Nickname collision KILL from <user>@<host>"*/
#define ERR_NICKCOLLISION 436

/*  - Returned by the server to indicate that the target user of the command is not on the given channel.
* @return "<nick/channel> :They aren't on that channel"*/
#define ERR_USERNOTINCHANNEL 441

/*  - Returned by the server whenever a client tries to perform a channel effecting command for which the client isn't a member.
* @return "<channel> :You're not on that channel"*/
#define ERR_NOTONCHANNEL 442

/*  - Returned when a client tries to invite a user to a channel they are already on.
* @return "<user> <channel> :is already on channel"*/
#define ERR_USERONCHANNEL 443

/*  - Returned by the summon after a SUMMON command for a user was unable to be performed since they were not logged in.
* @return "<user> :User not logged in"*/
#define ERR_NOLOGIN 444

/*  - Returned as a response to the SUMMON command. Must be returned by any server which does not implement it.
* @return "<server> :SUMMON has been disabled"*/
#define ERR_SUMMONDISABLED 445

/*  - Returned as a response to the USERS command. Must be returned by any server which does not implement it.
* @return "<server> :USERS has been disabled"*/
#define ERR_USERSDISABLED 446

/*  - Returned by the server to indicate that the client must be registered before the server will allow it to be parsed in detail.
* @return ":You have not registered"*/
#define ERR_NOTREGISTERED 451

/*  - Returned by the server by numerous commands to indicate to the client that it didn't supply enough parameters.
* @return "<command> :Not enough parameters"*/
#define ERR_NEEDMOREPARAMS 461

/*  - Returned by the server to any link which tries to change part of the registered details (such as password or user details from second USER message).
* @return ":Unauthorized command (already registered)"*/
#define ERR_ALREADYREGISTRED 462

/*  - Returned to a client which attempts to register with a server which does not been setup to allow connections from the host the attempted connection is tried.
* @return ":Your host isn't among the privileged"*/
#define ERR_NOPERMFORHOST 463

/*  - Returned to indicate a failed attempt at registering a connection for which a password was required and was either not given or incorrect.
* @return ":Password incorrect"*/
#define ERR_PASSWDMISMATCH 464

/*  - Returned after an attempt to connect and register yourself with a server which has been setup to explicitly deny connections to you.
* @return ":You are banned from this server"*/
#define ERR_YOUREBANNEDCREEP 465

/* @return "<channel> :Channel key already set"*/
#define ERR_KEYSET 467

/* @return "<channel> :Channel is full"*/
#define ERR_CHANNELISFULL 471

/* @return "<char> :is unknown mode <char> to me for <channel>"*/
#define ERR_UNKNOWNMODE 472

/* @return "<channel> :Cannot join channel (+l)"*/
#define ERR_INVITEONLYCHAN 473

/* @return "<channel> :Cannot join channel (+i)"*/
#define ERR_BANNEDFROMCHAN 474

/* @return "<channel> :Cannot join channel (+k)"*/
#define ERR_BADCHANNELKEY 475

/* @return "<channel> :Bad Channel Mask"*/
#define ERR_NOPRIVILEGES 481

/* @return "<channel> :You're not channel operator"*/
#define ERR_CHANOPRIVSNEEDED 482

/* @return ":You cant kill a server!"*/
#define ERR_CANTKILLSERVER 483

/* @return ":Your connection is restricted!"*/
#define ERR_NOOPERHOST 491

/* @return ":Unknown MODE flag"*/
#define ERR_UMODEUNKNOWNFLAG 501

/* @return ":Cant change mode for other users"*/
#define ERR_USERSDONTMATCH 502







// /* @return "Welcome to the Internet Relay Network <nick>!<user>@<host>"*/
// #define RPL_NONE 300
// /* @return "<user> :UserID"*/
// #define RPL_USERHOST 302
// /* @return ":Server Up %d days %d:%02d:%02d"*/
// #define RPL_ISON 303
// /* @return "<nick> <server> <time> <away>"*/
// #define RPL_AWAY 301
// /* @return ":You are no longer marked as being away"*/
// #define RPL_UNAWAY 305
// /* @return ":You have been marked as being away"*/
// #define RPL_NOWAWAY 306
// /* @return "<nick> <user> <host> * :<real name>"*/
// #define RPL_WHOISUSER 311
// /* @return "<nick> <server> :<server info>"*/
// #define RPL_WHOISSERVER 312
// /* @return "<nick> :is an IRC operator"*/
// #define RPL_WHOISOPERATOR 313
// /* @return "<nick> <integer> :seconds idle"*/
// #define RPL_WHOISIDLE 317
// /* @return "<nick> :End of WHOIS list"*/
// #define RPL_ENDOFWHOIS 318
// /* @return "<nick> :*( ( "@" / "+" ) <channel> " " )"*/
// #define RPL_WHOISCHANNELS 319
// /* @return "<nick> <user> <host> * :<real name>"*/
// #define RPL_WHOWASUSER 314
// /* @return "<nick> :End of WHOWAS"*/
// #define RPL_ENDOFWHOWAS 369
// /* @return "<channel> <# visible> :<topic>"*/
// #define RPL_LISTSTART 321
// /* @return "<channel> <# visible> :<topic>"*/
// #define RPL_LIST 322
// /* @return ":End of LIST"*/
// #define RPL_LISTEND 323
// /* @return "<channel> <mode> <mode params>"*/
// #define RPL_CHANNELMODEIS 324
// /* @return "<channel> <nickname>"*/
// #define RPL_NOTOPIC 331
// /* @return "<channel> :<topic>"*/
// #define RPL_TOPIC 332
// /* @return "<channel> <nick>"*/
// #define RPL_INVITING 341
// /* @return "<user> :Summoning user to IRC"*/
// #define RPL_SUMMONING 342
// /* @return "<channel> <invitemask>"*/
// #define RPL_VERSION 351
// /* @return "<channel> <nickname>"*/
// #define RPL_WHOREPLY 352
// /* @return "<name> :End of WHO list"*/
// #define RPL_ENDOFWHO 315
// /* @return "<channel> :End of Channel Link List"*/
// #define RPL_NAMREPLY 353
// /* @return "<channel> :End of NAMES list"*/
// #define RPL_ENDOFNAMES 366
// /* @return "<mask> <server> <nick> <H|G>[*][@|+] :<hopcount> <real name>"*/
// #define RPL_LINKS 364
// /* @return "<mask> :End of LINKS list"*/
// #define RPL_ENDOFLINKS 365
// /* @return "<channel> <banid>"*/
// #define RPL_BANLIST 367
// /* @return "<channel> :End of Channel Ban List"*/
// #define RPL_ENDOFBANLIST 368
// /* @return "<nick> :End of WHOWAS"*/
// #define RPL_INFO 371
// /* @return ":- <server> Message of the day - "*/
// #define RPL_ENDOFINFO 374
// /* @return ":- <server> Message of the day - "*/
// #define RPL_MOTDSTART 375
// /* @return ":- <text>"*/
// #define RPL_MOTD 372
// /* @return ":End of MOTD command"*/
// #define RPL_ENDOFMOTD 376
// /* @return ":You are now an IRC operator"*/
// #define RPL_YOUREOPER 381
// /* @return "<config file> :Rehashing"*/
// #define RPL_REHASHING 382
// /* @return "<server> :%s"*/
// #define RPL_TIME 391
// /* @return "UserID Terminal Host"*/
// #define RPL_USERSSTART 392
// /* @return ":End of users"*/
// #define RPL_USERS 393
// /* @return ":Nobody logged in"*/
// #define RPL_ENDOFUSERS 394
// /* @return ":No such nick/channel"*/
// #define RPL_NOUSERS 395
// /* @return "<server> :Server Up %d days %d:%02d:%02d"*/
// #define RPL_TRACELINK 200

// #define RPL_TRACECONNECTING 201
// #define RPL_TRACEHANDSHAKE 202
// #define RPL_TRACEUNKNOWN 203
// #define RPL_TRACEOPERATOR 204
// #define RPL_TRACEUSER 205
// #define RPL_TRACESERVER 206
// #define RPL_TRACENEWTYPE 208
// #define RPL_TRACELOG 261
// #define RPL_STATSLINKINFO 211
// #define RPL_STATSCOMMANDS 212
// #define RPL_STATSCLINE 213
// #define RPL_STATSNLINE 214
// #define RPL_STATSILINE 215
// #define RPL_STATSKLINE 216
// #define RPL_STATSYLINE 218
// #define RPL_ENDOFSTATS 219
// #define RPL_STATSLLINE 241
// #define RPL_STATSUPTIME 242
// #define RPL_STATSOLINE 243
// #define RPL_STATSHLINE 244
// #define RPL_UMODEIS 221
// #define RPL_LUSERCLIENT 251
// #define RPL_LUSEROP 252
// #define RPL_LUSERUNKNOWN 253
// #define RPL_LUSERCHANNELS 254
// #define RPL_LUSERME 255
// #define RPL_ADMINME 256
// #define RPL_ADMINLOC1 257
// #define RPL_ADMINLOC2 258
// #define RPL_ADMINEMAIL 259
// #define RPL_TRYAGAIN 263




//         402     ERR_NOSUCHSERVER
//                         "<server name> :No such server"

//                 - Used to indicate the server name given currently
//                   doesn't exist.

//         403     ERR_NOSUCHCHANNEL
//                         "<channel name> :No such channel"

//                 - Used to indicate the given channel name is invalid.

//         404     ERR_CANNOTSENDTOCHAN
//                         "<channel name> :Cannot send to channel"

//                 - Sent to a user who is either (a) not on a channel
//                   which is mode +n or (b) not a chanop (or mode +v) on
//                   a channel which has mode +m set and is trying to send
//                   a PRIVMSG message to that channel.

//         405     ERR_TOOMANYCHANNELS
//                         "<channel name> :You have joined too many \
//                          channels"
//                 - Sent to a user when they have joined the maximum
//                   number of allowed channels and they try to join
//                   another channel.

//         406     ERR_WASNOSUCHNICK
//                         "<nickname> :There was no such nickname"

//                 - Returned by WHOWAS to indicate there is no history
//                   information for that nickname.

//         407     ERR_TOOMANYTARGETS
//                         "<target> :Duplicate recipients. No message \



// Oikarinen & Reed                                               [Page 43]

// RFC 1459              Internet Relay Chat Protocol              May 1993


//                          delivered"

//                 - Returned to a client which is attempting to send a
//                   PRIVMSG/NOTICE using the user@host destination format
//                   and for a user@host which has several occurrences.

//         409     ERR_NOORIGIN
//                         ":No origin specified"

//                 - PING or PONG message missing the originator parameter
//                   which is required since these commands must work
//                   without valid prefixes.

//         411     ERR_NORECIPIENT
//                         ":No recipient given (<command>)"
//         412     ERR_NOTEXTTOSEND
//                         ":No text to send"
//         413     ERR_NOTOPLEVEL
//                         "<mask> :No toplevel domain specified"
//         414     ERR_WILDTOPLEVEL
//                         "<mask> :Wildcard in toplevel domain"

//                 - 412 - 414 are returned by PRIVMSG to indicate that
//                   the message wasn't delivered for some reason.
//                   ERR_NOTOPLEVEL and ERR_WILDTOPLEVEL are errors that
//                   are returned when an invalid use of
//                   "PRIVMSG $<server>" or "PRIVMSG #<host>" is attempted.

//         421     ERR_UNKNOWNCOMMAND
//                         "<command> :Unknown command"

//                 - Returned to a registered client to indicate that the
//                   command sent is unknown by the server.

//         422     ERR_NOMOTD
//                         ":MOTD File is missing"

//                 - Server's MOTD file could not be opened by the server.

//         423     ERR_NOADMININFO
//                         "<server> :No administrative info available"

//                 - Returned by a server in response to an ADMIN message
//                   when there is an error in finding the appropriate
//                   information.

//         424     ERR_FILEERROR
//                 ":File error doing <file op> on <file>"



// Oikarinen & Reed                                               [Page 44]

// RFC 1459              Internet Relay Chat Protocol              May 1993


//                 - Generic error message used to report a failed file
//                   operation during the processing of a message.

//         431     ERR_NONICKNAMEGIVEN
//                         ":No nickname given"

//                 - Returned when a nickname parameter expected for a
//                   command and isn't found.

//         432     ERR_ERRONEUSNICKNAME
//                         "<nick> :Erroneus nickname"

//                 - Returned after receiving a NICK message which contains
//                   characters which do not fall in the defined set.  See
//                   section x.x.x for details on valid nicknames.

//         433     ERR_NICKNAMEINUSE
//                         "<nick> :Nickname is already in use"

//                 - Returned when a NICK message is processed that results
//                   in an attempt to change to a currently existing
//                   nickname.

//         436     ERR_NICKCOLLISION
//                         "<nick> :Nickname collision KILL"

//                 - Returned by a server to a client when it detects a
//                   nickname collision (registered of a NICK that
//                   already exists by another server).

//         441     ERR_USERNOTINCHANNEL
//                         "<nick> <channel> :They aren't on that channel"

//                 - Returned by the server to indicate that the target
//                   user of the command is not on the given channel.

//         442     ERR_NOTONCHANNEL
//                         "<channel> :You're not on that channel"

//                 - Returned by the server whenever a client tries to
//                   perform a channel effecting command for which the
//                   client isn't a member.

//         443     ERR_USERONCHANNEL
//                         "<user> <channel> :is already on channel"

//                 - Returned when a client tries to invite a user to a
//                   channel they are already on.



// Oikarinen & Reed                                               [Page 45]

// RFC 1459              Internet Relay Chat Protocol              May 1993


//         444     ERR_NOLOGIN
//                         "<user> :User not logged in"

//                 - Returned by the summon after a SUMMON command for a
//                   user was unable to be performed since they were not
//                   logged in.

//         445     ERR_SUMMONDISABLED
//                         ":SUMMON has been disabled"

//                 - Returned as a response to the SUMMON command.  Must be
//                   returned by any server which does not implement it.

//         446     ERR_USERSDISABLED
//                         ":USERS has been disabled"

//                 - Returned as a response to the USERS command.  Must be
//                   returned by any server which does not implement it.

//         451     ERR_NOTREGISTERED
//                         ":You have not registered"

//                 - Returned by the server to indicate that the client
//                   must be registered before the server will allow it
//                   to be parsed in detail.

//         461     ERR_NEEDMOREPARAMS
//                         "<command> :Not enough parameters"

//                 - Returned by the server by numerous commands to
//                   indicate to the client that it didn't supply enough
//                   parameters.

//         462     ERR_ALREADYREGISTRED
//                         ":You may not reregister"

//                 - Returned by the server to any link which tries to
//                   change part of the registered details (such as
//                   password or user details from second USER message).


//         463     ERR_NOPERMFORHOST
//                         ":Your host isn't among the privileged"

//                 - Returned to a client which attempts to register with
//                   a server which does not been setup to allow
//                   connections from the host the attempted connection
//                   is tried.



// Oikarinen & Reed                                               [Page 46]

// RFC 1459              Internet Relay Chat Protocol              May 1993


//         464     ERR_PASSWDMISMATCH
//                         ":Password incorrect"

//                 - Returned to indicate a failed attempt at registering
//                   a connection for which a password was required and
//                   was either not given or incorrect.

//         465     ERR_YOUREBANNEDCREEP
//                         ":You are banned from this server"

//                 - Returned after an attempt to connect and register
//                   yourself with a server which has been setup to
//                   explicitly deny connections to you.

//         467     ERR_KEYSET
//                         "<channel> :Channel key already set"
//         471     ERR_CHANNELISFULL
//                         "<channel> :Cannot join channel (+l)"
//         472     ERR_UNKNOWNMODE
//                         "<char> :is unknown mode char to me"
//         473     ERR_INVITEONLYCHAN
//                         "<channel> :Cannot join channel (+i)"
//         474     ERR_BANNEDFROMCHAN
//                         "<channel> :Cannot join channel (+b)"
//         475     ERR_BADCHANNELKEY
//                         "<channel> :Cannot join channel (+k)"
//         481     ERR_NOPRIVILEGES
//                         ":Permission Denied- You're not an IRC operator"

//                 - Any command requiring operator privileges to operate
//                   must return this error to indicate the attempt was
//                   unsuccessful.

//         482     ERR_CHANOPRIVSNEEDED
//                         "<channel> :You're not channel operator"

//                 - Any command requiring 'chanop' privileges (such as
//                   MODE messages) must return this error if the client
//                   making the attempt is not a chanop on the specified
//                   channel.

//         483     ERR_CANTKILLSERVER
//                         ":You cant kill a server!"

//                 - Any attempts to use the KILL command on a server
//                   are to be refused and this error returned directly
//                   to the client.




// Oikarinen & Reed                                               [Page 47]

// RFC 1459              Internet Relay Chat Protocol              May 1993


//         491     ERR_NOOPERHOST
//                         ":No O-lines for your host"

//                 - If a client sends an OPER message and the server has
//                   not been configured to allow connections from the
//                   client's host as an operator, this error must be
//                   returned.

//         501     ERR_UMODEUNKNOWNFLAG
//                         ":Unknown MODE flag"

//                 - Returned by the server to indicate that a MODE
//                   message was sent with a nickname parameter and that
//                   the a mode flag sent was not recognized.

//         502     ERR_USERSDONTMATCH
//                         ":Cant change mode for other users"

//                 - Error sent to any user trying to view or change the
//                   user mode for a user other than themselves.

// 6.2 Command responses.

//         300     RPL_NONE
//                         Dummy reply number. Not used.

//         302     RPL_USERHOST
//                         ":[<reply>{<space><reply>}]"

//                 - Reply format used by USERHOST to list replies to
//                   the query list.  The reply string is composed as
//                   follows:

//                   <reply> ::= <nick>['*'] '=' <'+'|'-'><hostname>

//                   The '*' indicates whether the client has registered
//                   as an Operator.  The '-' or '+' characters represent
//                   whether the client has set an AWAY message or not
//                   respectively.

//         303     RPL_ISON
//                         ":[<nick> {<space><nick>}]"

//                 - Reply format used by ISON to list replies to the
//                   query list.

//         301     RPL_AWAY
//                         "<nick> :<away message>"



// Oikarinen & Reed                                               [Page 48]

// RFC 1459              Internet Relay Chat Protocol              May 1993


//         305     RPL_UNAWAY
//                         ":You are no longer marked as being away"
//         306     RPL_NOWAWAY
//                         ":You have been marked as being away"

//                 - These replies are used with the AWAY command (if
//                   allowed).  RPL_AWAY is sent to any client sending a
//                   PRIVMSG to a client which is away.  RPL_AWAY is only
//                   sent by the server to which the client is connected.
//                   Replies RPL_UNAWAY and RPL_NOWAWAY are sent when the
//                   client removes and sets an AWAY message.

//         311     RPL_WHOISUSER
//                         "<nick> <user> <host> * :<real name>"
//         312     RPL_WHOISSERVER
//                         "<nick> <server> :<server info>"
//         313     RPL_WHOISOPERATOR
//                         "<nick> :is an IRC operator"
//         317     RPL_WHOISIDLE
//                         "<nick> <integer> :seconds idle"
//         318     RPL_ENDOFWHOIS
//                         "<nick> :End of /WHOIS list"
//         319     RPL_WHOISCHANNELS
//                         "<nick> :{[@|+]<channel><space>}"

//                 - Replies 311 - 313, 317 - 319 are all replies
//                   generated in response to a WHOIS message.  Given that
//                   there are enough parameters present, the answering
//                   server must either formulate a reply out of the above
//                   numerics (if the query nick is found) or return an
//                   error reply.  The '*' in RPL_WHOISUSER is there as
//                   the literal character and not as a wild card.  For
//                   each reply set, only RPL_WHOISCHANNELS may appear
//                   more than once (for long lists of channel names).
//                   The '@' and '+' characters next to the channel name
//                   indicate whether a client is a channel operator or
//                   has been granted permission to speak on a moderated
//                   channel.  The RPL_ENDOFWHOIS reply is used to mark
//                   the end of processing a WHOIS message.

//         314     RPL_WHOWASUSER
//                         "<nick> <user> <host> * :<real name>"
//         369     RPL_ENDOFWHOWAS
//                         "<nick> :End of WHOWAS"

//                 - When replying to a WHOWAS message, a server must use
//                   the replies RPL_WHOWASUSER, RPL_WHOISSERVER or
//                   ERR_WASNOSUCHNICK for each nickname in the presented



// Oikarinen & Reed                                               [Page 49]

// RFC 1459              Internet Relay Chat Protocol              May 1993


//                   list.  At the end of all reply batches, there must
//                   be RPL_ENDOFWHOWAS (even if there was only one reply
//                   and it was an error).

//         321     RPL_LISTSTART
//                         "Channel :Users  Name"
//         322     RPL_LIST
//                         "<channel> <# visible> :<topic>"
//         323     RPL_LISTEND
//                         ":End of /LIST"

//                 - Replies RPL_LISTSTART, RPL_LIST, RPL_LISTEND mark
//                   the start, actual replies with data and end of the
//                   server's response to a LIST command.  If there are
//                   no channels available to return, only the start
//                   and end reply must be sent.

//         324     RPL_CHANNELMODEIS
//                         "<channel> <mode> <mode params>"

//         331     RPL_NOTOPIC
//                         "<channel> :No topic is set"
//         332     RPL_TOPIC
//                         "<channel> :<topic>"

//                 - When sending a TOPIC message to determine the
//                   channel topic, one of two replies is sent.  If
//                   the topic is set, RPL_TOPIC is sent back else
//                   RPL_NOTOPIC.

//         341     RPL_INVITING
//                         "<channel> <nick>"

//                 - Returned by the server to indicate that the
//                   attempted INVITE message was successful and is
//                   being passed onto the end client.

//         342     RPL_SUMMONING
//                         "<user> :Summoning user to IRC"

//                 - Returned by a server answering a SUMMON message to
//                   indicate that it is summoning that user.

//         351     RPL_VERSION
//                         "<version>.<debuglevel> <server> :<comments>"

//                 - Reply by the server showing its version details.
//                   The <version> is the version of the software being



// Oikarinen & Reed                                               [Page 50]

// RFC 1459              Internet Relay Chat Protocol              May 1993


//                   used (including any patchlevel revisions) and the
//                   <debuglevel> is used to indicate if the server is
//                   running in "debug mode".

//                   The "comments" field may contain any comments about
//                   the version or further version details.

//         352     RPL_WHOREPLY
//                         "<channel> <user> <host> <server> <nick> \
//                          <H|G>[*][@|+] :<hopcount> <real name>"
//         315     RPL_ENDOFWHO
//                         "<name> :End of /WHO list"

//                 - The RPL_WHOREPLY and RPL_ENDOFWHO pair are used
//                   to answer a WHO message.  The RPL_WHOREPLY is only
//                   sent if there is an appropriate match to the WHO
//                   query.  If there is a list of parameters supplied
//                   with a WHO message, a RPL_ENDOFWHO must be sent
//                   after processing each list item with <name> being
//                   the item.

//         353     RPL_NAMREPLY
//                         "<channel> :[[@|+]<nick> [[@|+]<nick> [...]]]"
//         366     RPL_ENDOFNAMES
//                         "<channel> :End of /NAMES list"

//                 - To reply to a NAMES message, a reply pair consisting
//                   of RPL_NAMREPLY and RPL_ENDOFNAMES is sent by the
//                   server back to the client.  If there is no channel
//                   found as in the query, then only RPL_ENDOFNAMES is
//                   returned.  The exception to this is when a NAMES
//                   message is sent with no parameters and all visible
//                   channels and contents are sent back in a series of
//                   RPL_NAMEREPLY messages with a RPL_ENDOFNAMES to mark
//                   the end.

//         364     RPL_LINKS
//                         "<mask> <server> :<hopcount> <server info>"
//         365     RPL_ENDOFLINKS
//                         "<mask> :End of /LINKS list"

//                 - In replying to the LINKS message, a server must send
//                   replies back using the RPL_LINKS numeric and mark the
//                   end of the list using an RPL_ENDOFLINKS reply.

//         367     RPL_BANLIST
//                         "<channel> <banid>"
//         368     RPL_ENDOFBANLIST



// Oikarinen & Reed                                               [Page 51]

// RFC 1459              Internet Relay Chat Protocol              May 1993


//                         "<channel> :End of channel ban list"

//                 - When listing the active 'bans' for a given channel,
//                   a server is required to send the list back using the
//                   RPL_BANLIST and RPL_ENDOFBANLIST messages.  A separate
//                   RPL_BANLIST is sent for each active banid.  After the
//                   banids have been listed (or if none present) a
//                   RPL_ENDOFBANLIST must be sent.

//         371     RPL_INFO
//                         ":<string>"
//         374     RPL_ENDOFINFO
//                         ":End of /INFO list"

//                 - A server responding to an INFO message is required to
//                   send all its 'info' in a series of RPL_INFO messages
//                   with a RPL_ENDOFINFO reply to indicate the end of the
//                   replies.

//         375     RPL_MOTDSTART
//                         ":- <server> Message of the day - "
//         372     RPL_MOTD
//                         ":- <text>"
//         376     RPL_ENDOFMOTD
//                         ":End of /MOTD command"

//                 - When responding to the MOTD message and the MOTD file
//                   is found, the file is displayed line by line, with
//                   each line no longer than 80 characters, using
//                   RPL_MOTD format replies.  These should be surrounded
//                   by a RPL_MOTDSTART (before the RPL_MOTDs) and an
//                   RPL_ENDOFMOTD (after).

//         381     RPL_YOUREOPER
//                         ":You are now an IRC operator"

//                 - RPL_YOUREOPER is sent back to a client which has
//                   just successfully issued an OPER message and gained
//                   operator status.

//         382     RPL_REHASHING
//                         "<config file> :Rehashing"

//                 - If the REHASH option is used and an operator sends
//                   a REHASH message, an RPL_REHASHING is sent back to
//                   the operator.

//         391     RPL_TIME



// Oikarinen & Reed                                               [Page 52]

// RFC 1459              Internet Relay Chat Protocol              May 1993


//                         "<server> :<string showing server's local time>"

//                 - When replying to the TIME message, a server must send
//                   the reply using the RPL_TIME format above.  The string
//                   showing the time need only contain the correct day and
//                   time there.  There is no further requirement for the
//                   time string.

//         392     RPL_USERSSTART
//                         ":UserID   Terminal  Host"
//         393     RPL_USERS
//                         ":%-8s %-9s %-8s"
//         394     RPL_ENDOFUSERS
//                         ":End of users"
//         395     RPL_NOUSERS
//                         ":Nobody logged in"

//                 - If the USERS message is handled by a server, the
//                   replies RPL_USERSTART, RPL_USERS, RPL_ENDOFUSERS and
//                   RPL_NOUSERS are used.  RPL_USERSSTART must be sent
//                   first, following by either a sequence of RPL_USERS
//                   or a single RPL_NOUSER.  Following this is
//                   RPL_ENDOFUSERS.

//         200     RPL_TRACELINK
//                         "Link <version & debug level> <destination> \
//                          <next server>"
//         201     RPL_TRACECONNECTING
//                         "Try. <class> <server>"
//         202     RPL_TRACEHANDSHAKE
//                         "H.S. <class> <server>"
//         203     RPL_TRACEUNKNOWN
//                         "???? <class> [<client IP address in dot form>]"
//         204     RPL_TRACEOPERATOR
//                         "Oper <class> <nick>"
//         205     RPL_TRACEUSER
//                         "User <class> <nick>"
//         206     RPL_TRACESERVER
//                         "Serv <class> <int>S <int>C <server> \
//                          <nick!user|*!*>@<host|server>"
//         208     RPL_TRACENEWTYPE
//                         "<newtype> 0 <client name>"
//         261     RPL_TRACELOG
//                         "File <logfile> <debug level>"

//                 - The RPL_TRACE* are all returned by the server in
//                   response to the TRACE message.  How many are
//                   returned is dependent on the the TRACE message and



// Oikarinen & Reed                                               [Page 53]

// RFC 1459              Internet Relay Chat Protocol              May 1993


//                   whether it was sent by an operator or not.  There
//                   is no predefined order for which occurs first.
//                   Replies RPL_TRACEUNKNOWN, RPL_TRACECONNECTING and
//                   RPL_TRACEHANDSHAKE are all used for connections
//                   which have not been fully established and are either
//                   unknown, still attempting to connect or in the
//                   process of completing the 'server handshake'.
//                   RPL_TRACELINK is sent by any server which handles
//                   a TRACE message and has to pass it on to another
//                   server.  The list of RPL_TRACELINKs sent in
//                   response to a TRACE command traversing the IRC
//                   network should reflect the actual connectivity of
//                   the servers themselves along that path.
//                   RPL_TRACENEWTYPE is to be used for any connection
//                   which does not fit in the other categories but is
//                   being displayed anyway.

//         211     RPL_STATSLINKINFO
//                         "<linkname> <sendq> <sent messages> \
//                          <sent bytes> <received messages> \
//                          <received bytes> <time open>"
//         212     RPL_STATSCOMMANDS
//                         "<command> <count>"
//         213     RPL_STATSCLINE
//                         "C <host> * <name> <port> <class>"
//         214     RPL_STATSNLINE
//                         "N <host> * <name> <port> <class>"
//         215     RPL_STATSILINE
//                         "I <host> * <host> <port> <class>"
//         216     RPL_STATSKLINE
//                         "K <host> * <username> <port> <class>"
//         218     RPL_STATSYLINE
//                         "Y <class> <ping frequency> <connect \
//                          frequency> <max sendq>"
//         219     RPL_ENDOFSTATS
//                         "<stats letter> :End of /STATS report"
//         241     RPL_STATSLLINE
//                         "L <hostmask> * <servername> <maxdepth>"
//         242     RPL_STATSUPTIME
//                         ":Server Up %d days %d:%02d:%02d"
//         243     RPL_STATSOLINE
//                         "O <hostmask> * <name>"
//         244     RPL_STATSHLINE
//                         "H <hostmask> * <servername>"

//         221     RPL_UMODEIS
//                         "<user mode string>"




// Oikarinen & Reed                                               [Page 54]

// RFC 1459              Internet Relay Chat Protocol              May 1993


//                         - To answer a query about a client's own mode,
//                           RPL_UMODEIS is sent back.

//         251     RPL_LUSERCLIENT
//                         ":There are <integer> users and <integer> \
//                          invisible on <integer> servers"
//         252     RPL_LUSEROP
//                         "<integer> :operator(s) online"
//         253     RPL_LUSERUNKNOWN
//                         "<integer> :unknown connection(s)"
//         254     RPL_LUSERCHANNELS
//                         "<integer> :channels formed"
//         255     RPL_LUSERME
//                         ":I have <integer> clients and <integer> \
//                           servers"

//                         - In processing an LUSERS message, the server
//                           sends a set of replies from RPL_LUSERCLIENT,
//                           RPL_LUSEROP, RPL_USERUNKNOWN,
//                           RPL_LUSERCHANNELS and RPL_LUSERME.  When
//                           replying, a server must send back
//                           RPL_LUSERCLIENT and RPL_LUSERME.  The other
//                           replies are only sent back if a non-zero count
//                           is found for them.

//         256     RPL_ADMINME
//                         "<server> :Administrative info"
//         257     RPL_ADMINLOC1
//                         ":<admin info>"
//         258     RPL_ADMINLOC2
//                         ":<admin info>"
//         259     RPL_ADMINEMAIL
//                         ":<admin info>"

//                         - When replying to an ADMIN message, a server
//                           is expected to use replies RLP_ADMINME
//                           through to RPL_ADMINEMAIL and provide a text
//                           message with each.  For RPL_ADMINLOC1 a
//                           description of what city, state and country
//                           the server is in is expected, followed by
//                           details of the university and department
//                           (RPL_ADMINLOC2) and finally the administrative
//                           contact for the server (an email address here
//                           is required) in RPL_ADMINEMAIL.