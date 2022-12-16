#ifndef _SB_TCP_H_
#define _SB_TCP_H_

#include <string>

bool SendMessageToClient(string msg);
string ReceiveMessageFromClient();
bool SendMessageToServer(string msg);
string ReceiveMessageFromServer();
void RunServer(void);
void StopServer(void);
void RunClient(void);
void StopClient(void);
string GetServerName();
string GetClientName();
#endif

