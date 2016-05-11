Kristopher Thieneman, kthienem
The only file is called udpserver.cpp. This file is used to create a simple udp
server that waits for connections from a client. Once a client has connected, if
there was a message sent the server takes this message, appends a microsecond 
accurate timestamp, then encrypts the message and sends it back to the client.
Once the message has been sent back it waits for another client to connect and
send a new message.

To compile the code simply type the command "g++ udpserver.cpp -o udpserver".
To run the code use the command "./udpserver XXXXX" , where XXXXX is the port
port number that your server is running on and the port number clients will
use to connect to your server.
