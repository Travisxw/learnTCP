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
int main()
{
	WORD ver = MAKEWORD(2, 2);//����2.2�汾��
	WSADATA dat;
	//����windows socket����
	WSAStartup(ver, &dat);//LPWSADATA,�ڶ���ʱȥ��LP��Ϊ�������
	//---------
	//1.����һ��socket�׽���
	SOCKET _sock= socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//(ipv4,�������ģ�Э��)������uint
	//2.bind �����ڽ��ܿͻ������ӵ�����˿�
	sockaddr_in _sin = {};
	_sin.sin_family = AF_INET;
	_sin.sin_port = htons(4567);//host to net unsigned short
	_sin.sin_addr.S_un.S_addr = INADDR_ANY;//���⣬�������÷���
	//_sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");//������127.0.0.1
	if (SOCKET_ERROR == bind(_sock, (sockaddr*)&_sin, sizeof(_sin)))//sockaddr_in��sockaddr��������д
	{
		std::cout << "�����ڽ��ܿͻ������ӵ�����˿�ʧ��" << std::endl;
	}
	else {
		std::cout << "�����ڽ��ܿͻ������ӵ�����˿ڳɹ�" << std::endl;
	}
	//3.listen ��������˿ڰ��п���ʧ�ܣ�bind��listen�ֿ�
	if (SOCKET_ERROR == listen(_sock, 5))//�����Եȴ��������ӣ���ʾ���Ƿ������ܾ�(��������������)����֮ǰ������ϵͳ���Թ�����������������nҲ���Կ�����"�Ŷӵ�����"
	{
		std::cout << "�������ڽ��ܿͻ������ӵ�����˿�ʧ��" << std::endl;
	}
	else {
		std::cout << "�������ڽ��ܿͻ������ӵ�����˿ڳɹ�" << std::endl;
	}
	//4. accept�ȴ����ܿͻ�������
	sockaddr_in clientAddr = {};
	int nAddeLen = sizeof(clientAddr);
	SOCKET _csocket = INVALID_SOCKET;
	char msgBuf[] = "Hello ,I am Server.";

	_csocket = accept(_sock, (sockaddr*)&clientAddr, &nAddeLen);//�¼���Ŀͻ���socket
	if (INVALID_SOCKET == _csocket)
	{
		std::cout << "���ܵ���Ч�ͻ���SOCKET" << std::endl;
	}
	std::cout << "new Client IP :" << inet_ntoa(clientAddr.sin_addr) << std::endl;//��ӡIP��ַ
	char _recvBuf[128] = {};
	while (1)
	{
		//���տͻ�������
		int nLen = recv(_csocket, _recvBuf, 128, 0);
		if (nLen <= 0)
		{
			std::cout << "�ͻ������˳� ���������" << std::endl;
			break;
		}
		
		//��������
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
		//5. send ��ͻ��˷���һ������
		else
			send(_csocket, msgBuf, strlen(msgBuf) + 1, 0);//���Ͻ�β��
	}	
	//6. �ر��׽���
	closesocket(_sock);

	//---------
	WSACleanup();
	return 0;
}

