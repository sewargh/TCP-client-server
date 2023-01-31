# CLIENT
gcc Client.c -o client

./client [ServerIP] [PortNum] input.txt [SleepTime-in-Ms]


# SERVER
gcc Server.c -o server

./server [PortNum]
