# CLIENT
gcc Client.c -o client

./client 127.0.0.1 [PortNum] input.txt [SleepTime-in-Ms]


# SERVER
gcc Server.c -o server

./server 5555
