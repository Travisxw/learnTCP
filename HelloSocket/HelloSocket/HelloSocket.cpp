#define WIN32_LEAN_AND_MEAN//��������һЩ���ڵĺ꣬����socket����
#include<iostream>
#include<Windows.h>
#include<WinSock2.h>//��Ҫ��#include<Windows.h>ǰ

//#pragma comment(lib,"ws2_32.lib")//����һ����̬��

int main()
{
	WORD ver = MAKEWORD(2, 2);//����2.2�汾��
	WSADATA dat;
	//����windows socket����
	WSAStartup(ver, &dat);//LPWSADATA,�ڶ���ʱȥ��LP��Ϊ�������
	//---------


	//---------
	WSACleanup();
	system("pause");
	return 0;
}