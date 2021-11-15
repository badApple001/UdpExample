﻿
//UdpNetClient.cpp

#include <Winsock2.h>
#include <stdio.h>

#pragma comment(lib,"ws2_32.lib")

void main()
{
	//加载套接字库
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(1, 1);

	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0)
	{
		return;
	}

	if (LOBYTE(wsaData.wVersion) != 1 ||     //低字节为主版本
		HIBYTE(wsaData.wVersion) != 1)      //高字节为副版本
	{
		WSACleanup();
		return;
	}

	printf("Client is operating!\n\n");
	//创建用于监听的套接字
	SOCKET sockSrv = socket(AF_INET, SOCK_DGRAM, 0);

	sockaddr_in  addrSrv;
	addrSrv.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");//输入你想通信的她（此处是本机内部）
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(9444);

	sockaddr_in addrS;
	addrS.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");//输入你想通信的她（此处是本机内部）
	addrS.sin_family = AF_INET;
	addrS.sin_port = htons(9443);



	int len = sizeof(SOCKADDR);

	char recvBuf[100];    //收
	char sendBuf[100];    //发
	char tempBuf[100];    //存储中间信息数据

	while (1)
	{

		printf("Please input data: \n");
		gets_s(sendBuf);
		sendBuf[strlen(sendBuf)] = '\0';
		sendto(sockSrv, sendBuf, strlen(sendBuf) + 1, 0, (SOCKADDR*)&addrS, len);

		//等待并数据
		recvfrom(sockSrv, recvBuf, 100, 0, (SOCKADDR*)&addrSrv, &len);

		if ('q' == recvBuf[0])
		{
			sendto(sockSrv, "q", strlen("q") + 1, 0, (SOCKADDR*)&addrS, len);
			printf("Chat end!\n");
			break;
		}
		sprintf_s(tempBuf, "%s say : %s", inet_ntoa(addrSrv.sin_addr), recvBuf);
		printf("%s\n", tempBuf);

		//发送数据

	}
	closesocket(sockSrv);
	WSACleanup();
}