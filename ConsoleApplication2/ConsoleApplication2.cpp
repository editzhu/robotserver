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
	//初始化WSA windows自带的socket
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		return 0;
	}

	//创建服务端套接字
	SOCKET slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (slisten == INVALID_SOCKET)
	{
		logd("socket error !");
		return 0;
	}

	//服务端需要绑定ip和端口
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(18888);
	sin.sin_addr.S_un.S_addr = INADDR_ANY; //监听任意的地址
	if (bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR) //将服务端套接字与上面的ip和端口绑定 
	{
		logd("bind error !");
	}

	//开始监听
	if (listen(slisten, 5) == SOCKET_ERROR)  //用listen（） 监听前面绑定好的slisten套接字
	{
		logd("listen error !");
		return 0;
	}

	//循环接受数据
	SOCKET sClient;  //创建连接的套接字
	sockaddr_in remoteAddr;
	int nAddrlen = sizeof(remoteAddr); //用于接受客户端地址
	char revData[255]; //存储接受的数据
	while (1)
	{
		logd("等待连接...\n");
		sClient = accept(slisten, (SOCKADDR *)&remoteAddr, &nAddrlen); //和客户端 connect（）对应
		if (sClient == INVALID_SOCKET)
		{
			logd("accept error !");
			continue;
		}
		char sendBuf[20] = { '\0' };
		printf("接受到一个连接：%s \r\n", inet_ntop(AF_INET, (void*)&remoteAddr.sin_addr, sendBuf, 16));

		//数据接收
		int ret = recv(sClient, revData, 255, 0);
		if (ret > 0)
		{
			revData[ret] = 0x00;
			logd(revData);
			logd("\n");
		}

		//发送数据给客户端
		//const char *sendData = "你好，TCP客户端！ \n";
		//send(sClient, sendData, strlen(sendData), 0);
		//closesocket(sClient);  //关闭已接通的套接字
	}


	closesocket(slisten); //关闭监听的套接字
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
		logd("等待连接...\n");
		char recvData[255];
		int ret = recvfrom(serSocket, recvData, 255, 0, (sockaddr*)&remoteAddr, &nAddrLen);
		if (ret > 0)
		{
			recvData[ret] = 0x00;
			char sendBuf[20] = { '\0' };
			printf("接受到一个连接：%s \r\n", inet_ntop(AF_INET, (void*)&remoteAddr.sin_addr, sendBuf, 16));
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

		//该代码无法直接拒绝连接，只能对非法入侵做记录
		if (recvData[0] == 22 && recvData[1] == 44){
			//验证客户端合法性
			if (recvData[2] == 66 && recvData[3] == 66){
				logd("验证通过\n");
				const char * sendData = "yes1";
				sendto(serSocket, sendData, strlen(sendData), 0, (sockaddr *)&remoteAddr, nAddrLen);
				logd("send yes to client \n");
			}
			else{
				logd("验证为非法客户端\n");
				const char * sendData = "no1";
				sendto(serSocket, sendData, strlen(sendData), 0, (sockaddr *)&remoteAddr, nAddrLen);
				logd("send no to client\n");
			}
		}

		if (recvData[0] == 64 && recvData[1] == 64 && recvData[9] == 1){
			logd("启动任务1\n");
			system("cd /d D:\\Yuanchuan\\bat && start-task.bat 1");
		}
		if (recvData[0] == 64 && recvData[1] == 64 && recvData[9] == 2){
			logd("启动任务2\n");
			system("cd /d D:\\Yuanchuan\\bat && start-task.bat 2");
		}
		if (recvData[0] == 64 && recvData[1] == 64 && recvData[9] == 3){
			logd("启动任务3\n");
			system("cd /d D:\\Yuanchuan\\bat && start-task.bat 3");
		}
		if (recvData[0] == 64 && recvData[1] == 64 && recvData[9] == 4){
			logd("启动任务4\n");
			system("cd /d D:\\Yuanchuan\\bat && start-task.bat 4");
		}
		if (recvData[0] == 64 && recvData[1] == 64 && recvData[9] == 5){
			logd("启动任务5\n");
			system("cd /d D:\\Yuanchuan\\bat && start-task.bat 5");
		}
		if (recvData[0] == 64 && recvData[1] == 64 && recvData[9] == 6){
			logd("启动任务6\n");
			system("cd /d D:\\Yuanchuan\\bat && start-task.bat 6");
		}
		if (recvData[0] == 64 && recvData[1] == 64 && recvData[9] == 7){
			logd("中止任务7\n");
			system("cd /d D:\\Yuanchuan\\bat && stop-task.bat");
		}
		//const char * sendData = "一个来自服务端的UDP数据包...\n";
		//sendto(serSocket, sendData, strlen(sendData), 0, (sockaddr *)&remoteAddr, nAddrLen);

	}
	closesocket(serSocket);
	WSACleanup();
	return 0;
}


int _tmain(int argc, _TCHAR* argv[])
{
	//printf("%d\n", add(2, 3));
	//logd("程序开始\n");
	printf("安卓命令接收服务开启\n\n");


	udp();
	
	return 1;
}


