#define WIN32_LEAN_AND_MEAN//避免引入一些早期的宏，引起socket错误
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include<iostream>
#include<Windows.h>
#include<WinSock2.h>//需要在#include<Windows.h>前

#pragma comment(lib,"ws2_32.lib")//加入一个静态库

struct DataPackage
{
	int age;
	char name[32];
};

int main()
{
	WORD ver = MAKEWORD(2, 2);//传入2.2版本号
	WSADATA dat;
	//启动windows socket环境
	WSAStartup(ver, &dat);//LPWSADATA,在定义时去掉LP即为所需变量
	//---------
	//1. 建立一个socket
	SOCKET _sock= socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == _sock)
	{
		std::cout << "creat socket failed" << std::endl;
	}
	else {
		std::cout << "creat socket success" << std::endl;
	}
	//2. 连接服务器
	sockaddr_in _sin = {};
	_sin.sin_family = AF_INET;
	_sin.sin_port = htons(4567);
	_sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	int ret=connect(_sock, (sockaddr*)&_sin, sizeof(sockaddr_in));
	if(SOCKET_ERROR==ret)
	{
		std::cout << "connect socket failed" << std::endl;
	}
	else {
		std::cout << "connect socket success" << std::endl;
	}
	
	while (1)
	{
		//输入请求命令
		char cmdBuf[128] = {};
		std::cin >> cmdBuf;
		//处理请求命令
		if (0 == strcmp(cmdBuf, "exit"))
			break;
		else {
			//向服务器发送请求命令
			send(_sock, cmdBuf, strlen(cmdBuf) + 1, 0);
		}
		//3.接收服务器信息
		char recvBuf[128] = {};
		int nlen = recv(_sock, recvBuf, sizeof(recvBuf), 0);
		if (nlen > 0)
		{
			DataPackage *dp = (DataPackage*)recvBuf;
			std::cout << "receive data: " << dp->age << dp->name << std::endl;
		}
			
	}


	//4. 关闭socket
	closesocket(_sock);

	//---------
	WSACleanup();
	std::cout << "任务结束" << std::endl;
	system("pause");
	return 0;
}