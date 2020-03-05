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
	//1.建立一个socket套接字
	SOCKET _sock= socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//(ipv4,面向流的，协议)，返回uint
	//2.bind 绑定用于接受客户端连接的网络端口
	sockaddr_in _sin = {};
	_sin.sin_family = AF_INET;
	_sin.sin_port = htons(4567);//host to net unsigned short
	_sin.sin_addr.S_un.S_addr = INADDR_ANY;//随意，都可以让访问
	//_sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");//内网：127.0.0.1
	if (SOCKET_ERROR == bind(_sock, (sockaddr*)&_sin, sizeof(_sin)))//sockaddr_in比sockaddr更方便填写
	{
		std::cout << "绑定用于接受客户端连接的网络端口失败" << std::endl;
	}
	else {
		std::cout << "绑定用于接受客户端连接的网络端口成功" << std::endl;
	}
	//3.listen 监听网络端口绑定有可能失败，bind和listen分开
	if (SOCKET_ERROR == listen(_sock, 5))//最大可以等待几个链接，表示的是服务器拒绝(超过限制数量的)连接之前，操作系统可以挂起的最大连接数量。n也可以看作是"排队的数量"
	{
		std::cout << "监听用于接受客户端连接的网络端口失败" << std::endl;
	}
	else {
		std::cout << "监听用于接受客户端连接的网络端口成功" << std::endl;
	}
	//4. accept等待接受客户端连接
	sockaddr_in clientAddr = {};
	int nAddeLen = sizeof(clientAddr);
	SOCKET _csocket = INVALID_SOCKET;
	char msgBuf[] = "Hello ,I am Server.";

	_csocket = accept(_sock, (sockaddr*)&clientAddr, &nAddeLen);//新加入的客户的socket
	if (INVALID_SOCKET == _csocket)
	{
		std::cout << "接受到无效客户端SOCKET" << std::endl;
	}
	std::cout << "new Client IP :" << inet_ntoa(clientAddr.sin_addr) << std::endl;//打印IP地址
	char _recvBuf[128] = {};
	while (1)
	{
		//接收客户端数据
		int nLen = recv(_csocket, _recvBuf, 128, 0);
		if (nLen <= 0)
		{
			std::cout << "客户端已退出 ，任务结束" << std::endl;
			break;
		}
		
		//处理请求
		if (0 == strcmp(_recvBuf, "name"))
		{
			char msgBuf[] = "Hello ,I am Lihua";
			send(_csocket, msgBuf, strlen(msgBuf) + 1, 0);
			std::cout << _recvBuf << std::endl;
		}
		else if (0 == strcmp(_recvBuf, "age")) {
			char msgBuf[] = "Hello ,I am 18.";
			send(_csocket, msgBuf, strlen(msgBuf) + 1, 0);
			std::cout << _recvBuf << std::endl;
		}		
		else if (0 == strcmp(_recvBuf, "info")) {
			DataPackage dp = { 20,"LIhua" };
			send(_csocket, (const char *)&dp, sizeof(DataPackage), 0);
			std::cout << _recvBuf << std::endl;
		}
		//5. send 向客户端发送一条数据
		else
			send(_csocket, msgBuf, strlen(msgBuf) + 1, 0);//加上结尾符
	}	
	//6. 关闭套接字
	closesocket(_sock);

	//---------
	WSACleanup();
	return 0;
}

