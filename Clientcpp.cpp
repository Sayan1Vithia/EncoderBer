#include <iostream>
#include <string>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

void main()
{
	//прописываем ip и port
	string ipAddress = "127.0.0.1";
	int port = 54000;
	//инициализируем winsock
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0)
	{
		cerr << "Can't start winsock, Error!" << wsResult << endl;
		return;
	}
	//создаем socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		cerr << "Can't create socket, ERROR" << WSAGetLastError() << endl;
		WSACleanup();
		return;
	}

	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

	//соединяемся с сервером
	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR)
	{
		cerr << "Can't connect to server" << WSAGetLastError() << endl;
		closesocket(sock);
		WSACleanup();
		return;
	}

	char buf[4096];
	string userInput;
	//позволяем пользователю писать, отправлять текст
	do
	{
		//cout << "> ";
		//getline(cin, userInput);
		
		//START
		int i, x;
		char str[100];
		cout << "Please enter a string:\t";
		cin >> str;
		string str2 = str;
		int len = strlen(str);
		cout << "\nPlease choose following options:\n";
		cout << "1 = Encrypt the string.\n";
		cout << "2 = Decrypt the string.\n";
		cin >> x;
		//using switch case statements
		switch (x)
		{
			//first case for encrypting a string
		case 1:
			for (i = 0; (i < 100 && str[i] != '\0'); i++)
				str[i] = str[i] + 4; //the key for encryption is 3 that is added to ASCII value
			cout << "\nEncrypted string: " << str << "0" << len << str2 << endl;
			break;

			//second case for decrypting a string
		case 2:
			for (i = 0; (i < 100 && str[i] != '\0'); i++)
				str[i] = str[i] - 4; //the key for encryption is 3 that is subtracted to ASCII value

			cout << "\nDecrypted string: " << str << endl;
			break;

		default:
			cout << "\nInvalid Input !!!\n";
			return;
		}
		//END

		if (userInput.size() > 0)
		{
			int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0);
			if (sendResult != SOCKET_ERROR)
			{
				ZeroMemory(buf, 4096);
				int bytesReceived = recv(sock, buf, 4096, 0);
				if (bytesReceived > 0)
				{
					cout << "SERVER> " << string(buf, 0, bytesReceived) << endl;
				}
			}
		}
	} while (userInput.size() > 0);
	//закрываем все
	closesocket(sock);
	WSACleanup();
}