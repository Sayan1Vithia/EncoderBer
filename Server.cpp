#include <iostream>
#include <ws2tcpip.h>

#pragma comment (lib, "ws2_32.lib")

using namespace std;

void main()
{
    //инициализируем winsock
    WSADATA wsData;
    WORD ver = MAKEWORD(2, 2);

    int wsOk = WSAStartup(ver, &wsData);
    if (wsOk != 0)
    {
        cerr << "Can't initialize winsock! Quitting" << endl;
        return;
    }
    //создаем socket
    SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == INVALID_SOCKET)
    {
        cerr << "Can't create a socket! Quitting" << endl;
        return;
    }

    //биндим на ip и port
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);
    hint.sin_addr.S_un.S_addr = INADDR_ANY;
    bind(listening, (sockaddr*)&hint, sizeof(hint));


    //говорим winsock for listening
    listen(listening, SOMAXCONN);


    //ждем соединение
    sockaddr_in client;
    int clientsize = sizeof(client);

    SOCKET clientsocket = accept(listening, (sockaddr*)&client, &clientsize);

    char host[NI_MAXHOST];
    char service[NI_MAXHOST];

    ZeroMemory(host, NI_MAXHOST);
    ZeroMemory(service, NI_MAXHOST);

    if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
    {
        cout << host << " connected on port " << service << endl;
    }
    else
    {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        cout << host << " connected on port " <<
            ntohs(client.sin_port) << endl;
    }
    //закрываем sock
    closesocket(listening);
    //while loop
    char buf[4096];

    while (true)
    {
        ZeroMemory(buf, 4096);
        int bytesReceived = recv(clientsocket, buf, 4096, 0);
        if (bytesReceived == SOCKET_ERROR)
        {
            cerr << "Error in recv(). Quit" << endl;
            break;
        }
        if (bytesReceived == 0)
        {
            cout << "Client disconnected" << endl;
            break;
        }
        send(clientsocket, buf, bytesReceived + 1, 0);


    }
    //shutdown
    closesocket(clientsocket);
    WSACleanup();
}
