#pragma once
#define WIN32_LEAN_AND_MEAN
#include <stdio.h>
#include <time.h>

#include "objectFreeListTLS/headers/objectFreeListTLS.h"
#include "stack/headers/stack.h"
#include "queue/headers/queue.h"

#include "lanClient/headers/lanClient.h"
#pragma comment(lib, "lib/lanClient/lanClient_iocp")

class CEchoClient: public CLanClient{

public:

	CEchoClient(wchar_t* ip, unsigned short port, bool onNagle,
		int sendNum, int delay, bool beReconnect, int reconnectRatio,
		int maxPacketNum, int workerThreadNum);

	void OnEnterJoinServer();
	void OnLeaveServer();

	void OnRecv(CPacketPtr_Lan packet);
	void OnSend(int sendSize);

	void OnError(int errorCode, const wchar_t* errorMsg);

private:

	unsigned __int64 _echoSendData;
	unsigned __int64 _echoRecvData;
	
	int _sendNum;
	int _delay;

	int _reconnectRatio;
	unsigned int _randSeed;
	bool _beReconnect;
	
	bool _requestDisconnect; 

public:

	wchar_t* _ip;
	unsigned short _port;
	int _maxSendPacketNum;
	int _workerThreadNum;
	bool _onNagle;

};