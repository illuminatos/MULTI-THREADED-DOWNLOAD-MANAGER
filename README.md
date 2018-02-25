# MULTI-THREADED DOWNLOAD MANAGER

In this project i designed and implemented a multi-threaded download 
manager by using POSIX sockets. There is two programs as server-side, and the other client-side that were writing in C program language.


Firstly download client.c, server.c and 1.mp4 (31 MB) files in your computer.

After downloading video file and client and server source codes, take them in a file. Than in this file, open a file named such as client again and relocate client.c inside it. 


Open a terminal and with the code as “cd [file name]” enter file that has server.c

After that write to compile the server from terminal as  

“gcc -g -Wall -o server server.c -lpthread -lm”

After compile write the terminal  as “./server” to run server.

While server is running, compile the client side with the code that write in makefile as well from another terminal as

“gcc -g -Wall -o client client.c -lpthread -lm”  
and then run client side as 

prog. Name [ip of server] [number of threads] [file name to be downloaded]

such as
 ./client 127.0.0.1 5 1.mp4

With my code ./client 127.0.0.1 5 1.mp4 and download 1.mp4 video for 5 threads in the client file. If you want, you can change thread numbers.
