# CLIENT
gcc Client.c -o client
./client 127.0.0.1 5555 input.txt 1


# SERVER
gcc Server.c -o server
./server 5555
