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
WSAData WSADat; // ���������� WinSock (��������� ������� WSAStartup)
//sockaddr_in sin; // ����������(�����) ���������� ������
SOCKET Sock, Client; // ����� �볺���

//�������� ����� �������
string GetServerName(){	return ServerName;}
//�������� ����� �볺���
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
	// �������� ������� � ��������� ������ � WinSock
	closesocket(Sock);
	closesocket(Client);
	WSACleanup();
}
void ServerMenu() {
	// ***** ����
	int Menu;
	do {
		cout << "1. Send message;" << endl;
		cout << "2. Get Message;" << endl;
		cout << "3. Quit;" << endl;

		cout << "Make your selection: ";	cin >> Menu;

		switch (Menu) {
		case 1:
			// ³������� ����������� �볺���
			cout << "Enter message: "; cin.get();
			cin.getline(Message, 200);
			if (SendMessageToClient(Message))cout << "Sent!\n";
			else cout << "Error of sending!\n";
			break;
		case 2:
			// ������ ���������� �� �볺���
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
	int retCode = WSAStartup(0x0202, &WSADat); // ����������� WinSock
	gethostname(PCName, 30); // ��������� ���� ��������� ��
	sockaddr_in sin;
	sin.sin_family = AF_INET; // ��� ������
	sin.sin_addr.s_addr = 0;  // IP-����� �������
	sin.sin_port = htons(PORT); // ����� ����� �������
	Sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // ��������� ������
	bind(Sock, (sockaddr*)&sin, sizeof(sin)); // ��'�������� ���������� ������ � ������� sin

	// ***** ���������� �볺���
	cout << "������ �� �볺���..." << endl;

	listen(Sock, SOMAXCONN); // ��������������� ������ �������� (��� ���������� �볺���)
	Client = accept(Sock, (sockaddr*)&sin, 0); // ���������� �볺���
	recv(Client, ClientName, 30, 0); // ��������� ��'� ����'����� �볺���
	send(Client, PCName, strlen(PCName) + 1, 0); // ³������� ��'� ����� ����'����� (�������)

	cout << "�볺��� ( " << GetClientName() << " ) ���������!" << endl;
//	system("pause");
}

void ClientMenu() {// ***** ����
	int Menu;
	do {
		cout << "1. Send message;" << endl;
		cout << "2. Get Message;" << endl;
		cout << "3. Quit;" << endl;

		cout << "Make your selection: ";	cin >> Menu;

		switch (Menu) {
		case 1:
			// ³������� ����������� �������
			cout << "Enter message: "; cin.get();
			cin.getline(Message, 200);
			if (send(Sock, Message, strlen(Message) + 1, 0) != SOCKET_ERROR) cout << "Sent!\n";
			else cout << "Error of sending!\n";
			break;
		case 2:
			// ������ ����������� �� �������
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

	int retCode = WSAStartup(0x0202, &WSADat); // ����������� WinSock
	gethostname(PCName, 30);
	sockaddr_in sin;
	sin.sin_family = AF_INET; // ��� ������
	sin.sin_addr.s_addr = inet_addr(SERVERIP); // IP-����� �������
	sin.sin_port = htons(PORT); // ����� ����� �������
	Sock = socket(AF_INET, SOCK_STREAM, 0); // ��������� ������

	// ***** ����������� � �������
	cout << "ϳ��������� �� �������..." << endl;

	if (connect(Sock, (sockaddr*)&sin, sizeof(sin)) == SOCKET_ERROR) {
		cout << "������� ���������� �� �������!\n";
		system("pause");
		goto End;
	}
	send(Sock, PCName, strlen(PCName) + 1, 0); // ³������� ��'� ����� ����'����� (�볺���)
	recv(Sock, ServerName, 30, 0); // ��������� ��� �����'���� �������

	cout << "ϳ�������� �� ������� \"" << GetServerName() << "\" ���������!" << endl;
	return;

End: {}
	
}
void StopClient(void) {// �������� ������� � ��������� ������ � WinSock
	closesocket(Sock);
	WSACleanup();
}