#define WIN32_LEAN_AND_MEAN//��������һЩ���ڵĺ꣬����socket����
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include<iostream>
#include<Windows.h>
#include<WinSock2.h>//��Ҫ��#include<Windows.h>ǰ

#pragma comment(lib,"ws2_32.lib")//����һ����̬��

struct DataPackage
{
	int age;
	char name[32];
};
enum CMD
{
	CMD_LOGIN,
	CMD_LOGOUT,
	CMD_ERROR

};
struct Dataheader
{
	short dataLength;//���ݳ���
	short cmd;//����
};
struct Login
{
	char userName[32];
	char passWord[32];
};
struct LoginResult
{
	int result;
};
struct Logout
{
	char userName[32];
};
struct LogoutResult
{
	int result;
};
int main()
{
	WORD ver = MAKEWORD(2, 2);//����2.2�汾��
	WSADATA dat;
	//����windows socket����
	WSAStartup(ver, &dat);//LPWSADATA,�ڶ���ʱȥ��LP��Ϊ�������
	//---------
	//1. ����һ��socket
	SOCKET _sock= socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == _sock)
	{
		std::cout << "creat socket failed" << std::endl;
	}
	else {
		std::cout << "creat socket success" << std::endl;
	}
	//2. ���ӷ�����
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
		//������������
		char cmdBuf[128] = {};
		std::cin >> cmdBuf;
		//������������
		if (0 == strcmp(cmdBuf, "exit"))
			break;
		else if (0 == strcmp(cmdBuf, "login")) {
			//�������������������
			Login login = { "lxw","12345" };
			Dataheader dh = { sizeof(Login),CMD_LOGIN };
			send(_sock, (const char*)&dh, sizeof(Dataheader), 0);
			send(_sock, (const char*)&login, sizeof(Login), 0);
			//���շ�������������
			Dataheader retHeader = {  };
			LoginResult loginRet = {};
			recv(_sock, (char*)&retHeader, sizeof(Dataheader), 0);
			recv(_sock, (char*)&loginRet, sizeof(LoginResult), 0);
			std::cout << "LoginResult: " << loginRet.result << std::endl;
		}
		else if (0 == strcmp(cmdBuf, "logout")) {
			//�������������������
			Logout logout = {};
			Dataheader dh = {sizeof(logout), CMD_LOGOUT };
			send(_sock, (const char*)&dh, sizeof(Dataheader), 0);
			send(_sock, (const char*)&logout, sizeof(Logout), 0);
			//���շ�������������
			Dataheader retHeader = {  };
			LoginResult logoutRet = {};
			recv(_sock, (char*)&retHeader, sizeof(Dataheader), 0);
			recv(_sock, (char*)&logoutRet, sizeof(LogoutResult), 0);
			std::cout << "LogoutResult: " << logoutRet.result << std::endl;
		}
		else {
			std::cout << "��֧�ֵ��������������룡" << std::endl;
		}
		/*
		//3.���շ�������Ϣ
		char recvBuf[128] = {};
		int nlen = recv(_sock, recvBuf, sizeof(recvBuf), 0);
		if (nlen > 0)
		{
			DataPackage *dp = (DataPackage*)recvBuf;
			std::cout << "receive data: " << dp->age << dp->name << std::endl;
		}
		*/
			
	}


	//4. �ر�socket
	closesocket(_sock);

	//---------
	WSACleanup();
	std::cout << "�������" << std::endl;
	system("pause");
	return 0;
}