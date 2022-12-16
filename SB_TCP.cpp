#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <windows.h>
using namespace std; 
#include "SB_TCP.h"

#pragma comment (lib, "ws2_32.lib")
#pragma comment (lib, "mswsock.lib")

#define PORT 666
#define SERVERIP "127.0.0.1"
char PCName[30], ClientName[30], Message[200], ServerName[30], IP[16] = { 0 };
WSAData WSADat; // Властивості WinSock (результат функції WSAStartup)
//sockaddr_in sin; // Властивості(адрес) створеного сокета
SOCKET Sock, Client; // Сокет клієнта

//Отримати назву серверу
string GetServerName(){	return ServerName;}
//Отримати назву клієнта
string GetClientName(){	return ClientName;}

string CharToStr(char *char_array) {
	string s;
	s.assign(char_array);
return s;
}
bool SendMessageToClient(string msg) 
{
	bool retCode = false;
	strcpy(Message, msg.c_str());
	if (send(Client, Message, strlen(Message) + 1, 0) != SOCKET_ERROR) retCode = true; else retCode = false;
	return retCode;
}
string ReceiveMessageFromClient()
{
	bool retCode = false;
	string msg;
	if (recv(Client, Message, 200, 0) != SOCKET_ERROR)
	{
		retCode = true;
		msg = CharToStr(Message);
	}
	else retCode = false;
	return msg;
}
bool SendMessageToServer(string msg)
{
	bool retCode = false;
	strcpy(Message, msg.c_str());
	if (send(Sock, Message, strlen(Message) + 1, 0) != SOCKET_ERROR) retCode = true; else retCode = false;
	return retCode;
}
string ReceiveMessageFromServer()
{
	bool retCode = false; 
	string msg;
	if (recv(Sock, Message, 200, 0) != SOCKET_ERROR) { retCode = true; msg = CharToStr(Message);}
	else { retCode = false; }	
	return msg;
}
void StopServer(void) {
	// Закриття сокетов и закінчення работи с WinSock
	closesocket(Sock);
	closesocket(Client);
	WSACleanup();
}
void ServerMenu() {
	// ***** Меню
	int Menu;
	do {
		cout << "1. Send message;" << endl;
		cout << "2. Get Message;" << endl;
		cout << "3. Quit;" << endl;

		cout << "Make your selection: ";	cin >> Menu;

		switch (Menu) {
		case 1:
			// Відправка повідомлення клієнту
			cout << "Enter message: "; cin.get();
			cin.getline(Message, 200);
			if (SendMessageToClient(Message))cout << "Sent!\n";
			else cout << "Error of sending!\n";
			break;
		case 2:
			// Прийом повідомлень від клієнта
			ReceiveMessageFromClient();
			{
				cout << Message << endl;
				cin.get();
			}
			break;
		default: break;
		};

		cout << endl;
	} while (Menu != 3);
}
void RunServer(void) {
	int retCode = WSAStartup(0x0202, &WSADat); // Ініціалізація WinSock
	gethostname(PCName, 30); // Отримання імені поточного ПК
	sockaddr_in sin;
	sin.sin_family = AF_INET; // Тип адреса
	sin.sin_addr.s_addr = 0;  // IP-адрес сервера
	sin.sin_port = htons(PORT); // Номер порта сервера
	Sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // Створення сокета
	bind(Sock, (sockaddr*)&sin, sizeof(sin)); // Зв'язування створеного сокета з адресою sin

	// ***** Очікування клієнта
	cout << "Чекаємо на клієнта..." << endl;

	listen(Sock, SOMAXCONN); // Прослуховування сокета сервером (для підключення клієнта)
	Client = accept(Sock, (sockaddr*)&sin, 0); // Очікування клієнта
	recv(Client, ClientName, 30, 0); // Отримання ім'я комп'ютера клієнта
	send(Client, PCName, strlen(PCName) + 1, 0); // Відправка ім'я цього комп'ютера (сервера)

	cout << "Клієнта ( " << GetClientName() << " ) підключено!" << endl;
//	system("pause");
}

void ClientMenu() {// ***** Меню
	int Menu;
	do {
		cout << "1. Send message;" << endl;
		cout << "2. Get Message;" << endl;
		cout << "3. Quit;" << endl;

		cout << "Make your selection: ";	cin >> Menu;

		switch (Menu) {
		case 1:
			// Відправка повідомлення серверу
			cout << "Enter message: "; cin.get();
			cin.getline(Message, 200);
			if (send(Sock, Message, strlen(Message) + 1, 0) != SOCKET_ERROR) cout << "Sent!\n";
			else cout << "Error of sending!\n";
			break;
		case 2:
			// Прийом повідомлення від сервера
			if (recv(Sock, Message, 200, 0) != SOCKET_ERROR) {
				cout << Message << endl;
				cin.get();
			}
			else cout << "Error of getting!\n";
			break;
		case 3: break;
		default: break;
		};

		cout << endl;
	} while (Menu != 3);
}
void RunClient(void) {

	int retCode = WSAStartup(0x0202, &WSADat); // Ініціалізация WinSock
	gethostname(PCName, 30);
	sockaddr_in sin;
	sin.sin_family = AF_INET; // Тип адреса
	sin.sin_addr.s_addr = inet_addr(SERVERIP); // IP-адрес сервера
	sin.sin_port = htons(PORT); // Номер порта сервера
	Sock = socket(AF_INET, SOCK_STREAM, 0); // Створення сокета

	// ***** Подключение к серверу
	cout << "Підключення до сервера..." << endl;

	if (connect(Sock, (sockaddr*)&sin, sizeof(sin)) == SOCKET_ERROR) {
		cout << "Помилка підключення до сервера!\n";
		system("pause");
		goto End;
	}
	send(Sock, PCName, strlen(PCName) + 1, 0); // Відправка ім'я цього комп'ютера (клієнта)
	recv(Sock, ServerName, 30, 0); // Отримання іені компь'тера сервера

	cout << "Підключено до сервера \"" << GetServerName() << "\" проведено!" << endl;
	return;

End: {}
	
}
void StopClient(void) {// Закриття сокетов и закінчення работи с WinSock
	closesocket(Sock);
	WSACleanup();
}