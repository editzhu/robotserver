//socket server test
#include "stdafx.h"
#include <stdio.h>    
#include <winsock2.h> 
#include<WS2tcpip.h>
#include <iostream>  
#include <time.h>  
#include "dlltest1.h"
#include <fstream>

using namespace std;
#pragma comment(lib,"ws2_32.lib") 
#pragma comment(lib,"dlltest1.lib") 


int tcp(){
	//��ʼ��WSA windows�Դ���socket
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		return 0;
	}

	//����������׽���
	SOCKET slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (slisten == INVALID_SOCKET)
	{
		logd("socket error !");
		return 0;
	}

	//�������Ҫ��ip�Ͷ˿�
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(18888);
	sin.sin_addr.S_un.S_addr = INADDR_ANY; //��������ĵ�ַ
	if (bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR) //��������׽����������ip�Ͷ˿ڰ� 
	{
		logd("bind error !");
	}

	//��ʼ����
	if (listen(slisten, 5) == SOCKET_ERROR)  //��listen���� ����ǰ��󶨺õ�slisten�׽���
	{
		logd("listen error !");
		return 0;
	}

	//ѭ����������
	SOCKET sClient;  //�������ӵ��׽���
	sockaddr_in remoteAddr;
	int nAddrlen = sizeof(remoteAddr); //���ڽ��ܿͻ��˵�ַ
	char revData[255]; //�洢���ܵ�����
	while (1)
	{
		logd("�ȴ�����...\n");
		sClient = accept(slisten, (SOCKADDR *)&remoteAddr, &nAddrlen); //�Ϳͻ��� connect������Ӧ
		if (sClient == INVALID_SOCKET)
		{
			logd("accept error !");
			continue;
		}
		char sendBuf[20] = { '\0' };
		printf("���ܵ�һ�����ӣ�%s \r\n", inet_ntop(AF_INET, (void*)&remoteAddr.sin_addr, sendBuf, 16));

		//���ݽ���
		int ret = recv(sClient, revData, 255, 0);
		if (ret > 0)
		{
			revData[ret] = 0x00;
			logd(revData);
			logd("\n");
		}

		//�������ݸ��ͻ���
		//const char *sendData = "��ã�TCP�ͻ��ˣ� \n";
		//send(sClient, sendData, strlen(sendData), 0);
		//closesocket(sClient);  //�ر��ѽ�ͨ���׽���
	}


	closesocket(slisten); //�رռ������׽���
	WSACleanup();
	return 0;
}


int udp()
{
	WSADATA wsaData;
	WORD sockVersion = MAKEWORD(2, 2);
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		return 0;
	}

	SOCKET serSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (serSocket == INVALID_SOCKET)
	{
		logd("socket error !");
		return 0;
	}

	sockaddr_in serAddr;
	serAddr.sin_family = AF_INET;
	serAddr.sin_port = htons(18888);
	serAddr.sin_addr.S_un.S_addr = INADDR_ANY;
	if (bind(serSocket, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
	{
		logd("bind error !");
		closesocket(serSocket);
		return 0;
	}

	sockaddr_in remoteAddr;
	int nAddrLen = sizeof(remoteAddr);
	while (true)
	{
		logd("�ȴ�����...\n");
		char recvData[255];
		int ret = recvfrom(serSocket, recvData, 255, 0, (sockaddr*)&remoteAddr, &nAddrLen);
		if (ret > 0)
		{
			recvData[ret] = 0x00;
			char sendBuf[20] = { '\0' };
			printf("���ܵ�һ�����ӣ�%s \r\n", inet_ntop(AF_INET, (void*)&remoteAddr.sin_addr, sendBuf, 16));
			printf("%x ", recvData[0]);
			printf("%x ", recvData[1]);
			printf("%x ", recvData[2]);
			printf("%x ", recvData[3]);
			printf("%x ", recvData[4]);
			printf("%x ", recvData[5]);
			printf("%x ", recvData[6]);
			printf("%x ", recvData[7]);
			printf("%x ", recvData[8]);
			printf("%x ", recvData[9]);
			printf("\n");
		}

		//�ô����޷�ֱ�Ӿܾ����ӣ�ֻ�ܶԷǷ���������¼
		if (recvData[0] == 22 && recvData[1] == 44){
			//��֤�ͻ��˺Ϸ���
			if (recvData[2] == 66 && recvData[3] == 66){
				logd("��֤ͨ��\n");
				const char * sendData = "yes1";
				sendto(serSocket, sendData, strlen(sendData), 0, (sockaddr *)&remoteAddr, nAddrLen);
				logd("send yes to client \n");
			}
			else{
				logd("��֤Ϊ�Ƿ��ͻ���\n");
				const char * sendData = "no1";
				sendto(serSocket, sendData, strlen(sendData), 0, (sockaddr *)&remoteAddr, nAddrLen);
				logd("send no to client\n");
			}
		}

		if (recvData[0] == 64 && recvData[1] == 64 && recvData[9] == 1){
			logd("��������1\n");
			system("cd /d D:\\Yuanchuan\\bat && start-task.bat 1");
		}
		if (recvData[0] == 64 && recvData[1] == 64 && recvData[9] == 2){
			logd("��������2\n");
			system("cd /d D:\\Yuanchuan\\bat && start-task.bat 2");
		}
		if (recvData[0] == 64 && recvData[1] == 64 && recvData[9] == 3){
			logd("��������3\n");
			system("cd /d D:\\Yuanchuan\\bat && start-task.bat 3");
		}
		if (recvData[0] == 64 && recvData[1] == 64 && recvData[9] == 4){
			logd("��������4\n");
			system("cd /d D:\\Yuanchuan\\bat && start-task.bat 4");
		}
		if (recvData[0] == 64 && recvData[1] == 64 && recvData[9] == 5){
			logd("��������5\n");
			system("cd /d D:\\Yuanchuan\\bat && start-task.bat 5");
		}
		if (recvData[0] == 64 && recvData[1] == 64 && recvData[9] == 6){
			logd("��������6\n");
			system("cd /d D:\\Yuanchuan\\bat && start-task.bat 6");
		}
		if (recvData[0] == 64 && recvData[1] == 64 && recvData[9] == 7){
			logd("��ֹ����7\n");
			system("cd /d D:\\Yuanchuan\\bat && stop-task.bat");
		}
		//const char * sendData = "һ�����Է���˵�UDP���ݰ�...\n";
		//sendto(serSocket, sendData, strlen(sendData), 0, (sockaddr *)&remoteAddr, nAddrLen);

	}
	closesocket(serSocket);
	WSACleanup();
	return 0;
}


int _tmain(int argc, _TCHAR* argv[])
{
	//printf("%d\n", add(2, 3));
	//logd("����ʼ\n");
	printf("��׿������շ�����\n\n");


	udp();
	
	return 1;
}


