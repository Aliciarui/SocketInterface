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
	TIME,//时间
	NAME,//姓名
	CLIENTLIST,//客户端列表
	MESSAGE,//发送消息
	IDC,//指示
	SEND_SUCCESS,
	SEND_FAIL
}Command;//类型定义

//客户端请求数据包
typedef struct {
	Command command;
	int suc;
	char dataforward[1024];
}SendDataBag;

//客户端信息
typedef struct {
	SOCKET sNum;//端口号
	char ip[15];
	int port;
}client_info;

//服务端响应数据包
typedef struct {
	Command command;
	//int suc;
	char databack[1024];
	client_info list[10];
}ReceiveDataBag;

#endif