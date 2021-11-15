#include <WinSock2.h>
#include "UdpNetSrv.h"

#include <iostream>
#include <cstring>//������cstring������strlen()����������
#include <stdio.h>
using namespace std;

#pragma comment(lib,"ws2_32.lib")
void UdpNetSrv::Init()
{

	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	//�����׽��ֿ�
	wVersionRequested = MAKEWORD(1, 1);
	err = WSAStartup(wVersionRequested, &wsaData);//����᷵��WSASYSNOTREADY
	if (err != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());//��ʾ������Ϣ
		return;
	}

	//���汾��Ϣ
	if (LOBYTE(wsaData.wVersion) != 1 ||     //���ֽ�Ϊ���汾
		HIBYTE(wsaData.wVersion) != 1)      //���ֽ�Ϊ���汾
	{
		WSACleanup();
		printf("Failed. Error Code : %d", WSAGetLastError());//��ʾ������Ϣ
		return;
	}

	//�������ڼ������׽���
	SOCKET sockSrv = socket(AF_INET, SOCK_DGRAM, 0);//ʧ�ܻ᷵�� INVALID_SOCKET
	if (INVALID_SOCKET == sockSrv) {
		printf("Failed. Error Code : %d", WSAGetLastError());//��ʾ������Ϣ
		return;
	}

	//��ַ��
	SOCKADDR_IN addrSrv;     //����sockSrv���ͺͽ������ݰ��ĵ�ַ
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(9443);	//�˿�

	//���׽���, �󶨵��˿�
	err = bind(sockSrv, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));//�᷵��һ��SOCKET_ERROR
	if (err == SOCKET_ERROR)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());//��ʾ������Ϣ
		return;
	}

	SOCKADDR_IN addrClient;   //�������տͻ��˵ĵ�ַ��Ϣ
	int len = sizeof(SOCKADDR);
	char recvBuf[100];    //��
	char sendBuf[100];    //��
	char tempBuf[100];    //�洢�м���Ϣ����

	while (1)
	{

		//�ȴ�������
		recvfrom(sockSrv, recvBuf, 100, 0, (SOCKADDR*)&addrClient, &len);
		if ('q' == recvBuf[0])
		{
			sendto(sockSrv, "q", strlen("q") + 1, 0, (SOCKADDR*)&addrClient, len);
			printf("Chat end!\n");
			break;
		}
		sprintf_s(tempBuf, "%s say : %s", inet_ntoa(addrClient.sin_addr), recvBuf);
		printf("%s\n", tempBuf);

		//��������
		printf("Please input data: \n");

		gets_s(sendBuf);
		sendBuf[len] = '\0';

		sendto(sockSrv, sendBuf, strlen(sendBuf) + 1, 0, (SOCKADDR*)&addrClient, len);
	}

	closesocket(sockSrv);
	WSACleanup();

}


 