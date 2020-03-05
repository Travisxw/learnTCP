#define WIN32_LEAN_AND_MEAN//避免引入一些早期的宏，引起socket错误
#include<iostream>
#include<Windows.h>
#include<WinSock2.h>//需要在#include<Windows.h>前

//#pragma comment(lib,"ws2_32.lib")//加入一个静态库

int main()
{
	WORD ver = MAKEWORD(2, 2);//传入2.2版本号
	WSADATA dat;
	//启动windows socket环境
	WSAStartup(ver, &dat);//LPWSADATA,在定义时去掉LP即为所需变量
	//---------


	//---------
	WSACleanup();
	system("pause");
	return 0;
}