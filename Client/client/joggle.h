#pragma once
#ifndef __JOGGLE_H__
#define __JOGGLE_H__
#define WIN32_LEAN_AND_MEAN


#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <vector>
#include <queue>
#include <stdio.h>
#include <winsock2.h>
#include <WS2tcpip.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <process.h>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "User32.lib")


typedef enum {
	TIME,//ʱ��
	NAME,//����
	CLIENTLIST,//�ͻ����б�
	MESSAGE,//������Ϣ
	IDC,//ָʾ
	SEND_SUCCESS,
	SEND_FAIL
}Command;//���Ͷ���

//�ͻ����������ݰ�
typedef struct {
	Command command;
	int suc;
	char dataforward[1024];
}SendDataBag;

//�ͻ�����Ϣ
typedef struct {
	SOCKET sNum;//�˿ں�
	char ip[15];
	int port;
}client_info;

//�������Ӧ���ݰ�
typedef struct {
	Command command;
	//int suc;
	char databack[1024];
	client_info list[10];
}ReceiveDataBag;

#endif