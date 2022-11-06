# Raw_socket
Sockets are the means by which programs on Linux way talk to the internet. The socket system call creates a file descriptor that can be written to and read from. 
The connect system call can then be used to connect the socket to some remote address. After that, writing to the socket sends data to that remote address, 
while reading from the socket file descriptor reads data sent from the remote address.
If you are interested in writing your own implementations of one of these protocols, or need to use a different transport-layer protocol, 
you’ll need to use raw sockets. Raw sockets operate at the network OSI level, which means that transport-level headers such as TCP or UDP headers will not be automatically 
decoded. If you are implementing a a transport-level protocol, you’ll have to write code to decode and encode the transport-level headers in yourapplication.
- Attempting to read from a raw socket
To see what happens when we read data from sockets, let’s use this C program that simply sniffs packets entering the system and prints out some information about 
the packet as an example
- Compiling this program with:
$ gcc raw_sock.c
- And then running it with:
$ ./a.out
