#include "headers/echoClient.h"

CEchoClient::CEchoClient(wchar_t* ip, unsigned short port, bool onNagle,
	int sendNum, int delay, bool beReconnect, int reconnectRatio,
	int maxPacketNum, int workerThreadNum):
	CLanClient(ip, port, onNagle, maxPacketNum, workerThreadNum)
{

	_echoSendData = 0;
	_echoRecvData = 0;

	_sendNum = sendNum;
	_delay = delay;

	_beReconnect = beReconnect;
	_reconnectRatio = reconnectRatio;

	_requestDisconnect = false;

	_ip = ip;
	_port = port;
	_maxSendPacketNum = maxPacketNum;
	_workerThreadNum = workerThreadNum;
	_onNagle = onNagle;

	_randSeed = 0;
	srand(_randSeed);
}

void CEchoClient::OnEnterJoinServer(){
	
	_echoSendData = 0;
	_echoRecvData = 0;

	for(int sendCnt = 0; sendCnt < _sendNum; ++sendCnt){

		CPacketPtr_Lan packet;
		packet << _echoSendData;
		_echoSendData += 1;
		
		if(sendPacket(packet) == false){
			CDump::crash();
		}
	}

}

void CEchoClient::OnLeaveServer(){

	if(_requestDisconnect == false){
		CDump::crash();
	}

	requestConnect();
	
}

void CEchoClient::OnRecv(CPacketPtr_Lan packet){

	unsigned __int64 data = -1;
	packet >> data;
	
	if(_echoRecvData == data){
		
		_echoRecvData += 1;

		CPacketPtr_Lan echoPacket;
		echoPacket << _echoSendData;

		_echoSendData += 1;
		
		sendPacket(echoPacket);

	} else {

		printf("error: sendData: %d, recvData: %d\n", _echoRecvData, data);
  		CDump::crash();

	}

}

void CEchoClient::OnSend(int sendSize){
	if(_delay > 0){
		Sleep(_delay);
	}

	//printf("send size:%d\n", sendSize);

	if(_beReconnect == true && _echoSendData > 1000000){

		if(rand()%100 < _reconnectRatio){
			// request disconnect
			_requestDisconnect = true;
			//printf("request disconnect: %d\n", this->_sock);
			disconnect();

		}

	}
}

void CEchoClient::OnError(int errorCode, const wchar_t* errorMsg){

	wprintf(L"\ncode: %d\n", errorCode);
	wprintf(L"msg: %s\n\n", errorMsg);

	// connect fail
	if(errorCode == 30000){
		CDump::crash();
	}

}