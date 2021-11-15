#include <WinSock2.h>
#include "UdpNetSrv.h"

#include <iostream>
#include <cstring>//必须是cstring，否则strlen()方法不能用
#include <stdio.h>
using namespace std;

#pragma comment(lib,"ws2_32.lib")
void UdpNetSrv::Init()
{

	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	//加载套接字库
	wVersionRequested = MAKEWORD(1, 1);
	err = WSAStartup(wVersionRequested, &wsaData);//错误会返回WSASYSNOTREADY
	if (err != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());//显示错误信息
		return;
	}

	//检查版本信息
	if (LOBYTE(wsaData.wVersion) != 1 ||     //低字节为主版本
		HIBYTE(wsaData.wVersion) != 1)      //高字节为副版本
	{
		WSACleanup();
		printf("Failed. Error Code : %d", WSAGetLastError());//显示错误信息
		return;
	}

	//创建用于监听的套接字
	SOCKET sockSrv = socket(AF_INET, SOCK_DGRAM, 0);//失败会返回 INVALID_SOCKET
	if (INVALID_SOCKET == sockSrv) {
		printf("Failed. Error Code : %d", WSAGetLastError());//显示错误信息
		return;
	}

	//地址蔟
	SOCKADDR_IN addrSrv;     //定义sockSrv发送和接收数据包的地址
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(9443);	//端口

	//绑定套接字, 绑定到端口
	err = bind(sockSrv, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));//会返回一个SOCKET_ERROR
	if (err == SOCKET_ERROR)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());//显示错误信息
		return;
	}

	SOCKADDR_IN addrClient;   //用来接收客户端的地址信息
	int len = sizeof(SOCKADDR);
	char recvBuf[100];    //收
	char sendBuf[100];    //发
	char tempBuf[100];    //存储中间信息数据

	while (1)
	{

		//等待并数据
		recvfrom(sockSrv, recvBuf, 100, 0, (SOCKADDR*)&addrClient, &len);
		if ('q' == recvBuf[0])
		{
			sendto(sockSrv, "q", strlen("q") + 1, 0, (SOCKADDR*)&addrClient, len);
			printf("Chat end!\n");
			break;
		}
		sprintf_s(tempBuf, "%s say : %s", inet_ntoa(addrClient.sin_addr), recvBuf);
		printf("%s\n", tempBuf);

		//发送数据
		printf("Please input data: \n");

		gets_s(sendBuf);
		sendBuf[len] = '\0';

		sendto(sockSrv, sendBuf, strlen(sendBuf) + 1, 0, (SOCKADDR*)&addrClient, len);
	}

	closesocket(sockSrv);
	WSACleanup();

}


 