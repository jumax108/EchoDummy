#include <new>

#include "headers/echoClient.h"

#include "textParser/headers/textParser.h"
#pragma comment(lib, "lib/textParser/textParser")

CDump dump;

int main(){

	CTextParser parser("dummyConfig.txt");

	wchar_t ip[20]={0, };
	char ipSingleByte[20]={0,};
	int ipLen;
	int port;

	parser.setNameSpace("Connection");
	parser.getTextByKey("ip", ipSingleByte, 20, &ipLen);
	parser.getIntByKey("port", &port);
	parser.resetNameSpace(); 

	size_t convertSize;
	mbstowcs_s(&convertSize, ip, ipSingleByte, 20);

	int maxSendPacketNum;
	int workerThreadNum;
	int onNagle;
	parser.setNameSpace("NetworkModel");
	parser.getIntByKey("maxSendPacketNum", &maxSendPacketNum);
	parser.getIntByKey("workerThreadNum", &workerThreadNum);
	parser.getIntByKey("onNagle", &onNagle);
	parser.resetNameSpace();

	int overSend;
	int delay;
	int reconnect;
	int reconnectRatio;
	parser.setNameSpace("Echo");
	parser.getIntByKey("overSend", &overSend);
	parser.getIntByKey("delay", &delay);
	parser.getIntByKey("reconncect", &reconnect);
	parser.getIntByKey("reconncectRatio", &reconnectRatio);
	parser.resetNameSpace();

	int clientNum;
	parser.setNameSpace("Dummy");
	parser.getIntByKey("clientNum", &clientNum);
	parser.resetNameSpace();

	CEchoClient* clients = (CEchoClient*)malloc(sizeof(CEchoClient) * clientNum);
	for(int clientCnt = 0; clientCnt < clientNum; ++clientCnt){
		new (&clients[clientCnt]) CEchoClient(ip, port, onNagle, overSend, delay, reconnect, reconnectRatio, maxSendPacketNum, workerThreadNum);
		clients[clientCnt].requestConnect();
	}



	for(;;){
		
		int totalRecvTPS = 0;
		int totalSendTPS = 0;

		for(int clientCnt = 0; clientCnt < clientNum; ++clientCnt){
			totalRecvTPS += clients[clientCnt].getRecvTPS();
			totalSendTPS += clients[clientCnt].getSendTPS();
		}

		printf("recv TPS: %d\n", totalRecvTPS);
		printf("send TPS: %d\n", totalSendTPS);
		printf("packet pool: %I64d Chunks\n\n", CPacketPtr_Lan::getPacketPoolUsage());
		
		Sleep (1000);

	}

	system("PAUSE");


	return 0;

}
