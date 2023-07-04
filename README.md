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