#include"joggle.h"
using namespace std;

unsigned __stdcall SubThread(void * pThis);//subthread
SOCKET socket_client;//socket
HANDLE handle;
BOOL is_connect = false;
DWORD id = GetCurrentThreadId();
void client_list();


int main() {
	//WinSock
	WORD wVersionRequested;
	WSADATA wsaData;
	int ret;
	int rec;

	//WinSock initialization
	wVersionRequested = MAKEWORD(2, 2);//version
	ret = WSAStartup(wVersionRequested, &wsaData);
	if (ret != 0)
	{
		printf("WSAStartup() failed!\n");
		return 0;
	}
	//WinSock DLL support version 2.2
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		WSACleanup();
		printf("Invalid Winsock version!\n");
		return 0;
	}

	int input=0;
	int flag = 1;//after exit:0
	int spnum;
	char spip[15];
	
	//create socket
	socket_client = socket(AF_INET, SOCK_STREAM, 0);//assign protocol to 0
	if (socket_client == INVALID_SOCKET)//fail to create
	{
		WSACleanup();
		printf("socket() failed!\n");
		return 0;
	}
	int is_first = 1;
	
	while (flag) {//loop
		if (is_connect == true) {
			//create menu
			cout << "Welcome to use the client!" << endl;
			cout << "PRESS 2:disconnecting" << endl;
			cout << "PRESS 3:get time" << endl;
			cout << "PRESS 4:get server_name" << endl;
			cout << "PRESS 5:get client_list" << endl;
			cout << "PRESS 6:send message" << endl;
			cout << "PRESS 7:exit" << endl;
			if (is_first) {
				cout << "please input your order to use the client:";
				is_first = 0;
			}
			else {
				cout << "output:";
			}
		}
		else if (is_connect == false) {
			cout << "Welcome to use the client!" << endl;
			cout << "PRESS 1:conneting" << endl;
			cout << "PRESS 7:exit" << endl;
			cout << "please input your order to use the client:";
		}
		
		
		cin >> input;
		switch (input) {
		case 1: {
			//loop
			//connect server
			cout << "INPUT IP:";
			cin >> spip;
			cout << "INPUT server address:";
			cin >> spnum;
			cout << "Connecting" << endl;

			//server address
			struct sockaddr_in sa_address;//create server
			sa_address.sin_family = AF_INET;//address family
			sa_address.sin_port = htons(spnum);//Port number, converted to network character sequence
			//sa_address.sin_addr.S_un.S_addr = inet_addr(spip);
			inet_pton(AF_INET, spip, &sa_address.sin_addr.S_un.S_addr);
			
			ret = connect(socket_client, (struct sockaddr *)&sa_address, sizeof(sa_address));
			if (ret == SOCKET_ERROR)
			{
				cout << "Connecting failure!" << endl;
				closesocket(socket_client);//close socket
				WSACleanup();
				return 0;
			}
			//create subthread and prompt success
			unsigned null_thread;
			handle = NULL;
			handle = (HANDLE)_beginthreadex(NULL, 0, &SubThread, NULL, 0, &null_thread);//create subthread
			if (handle == NULL) {
				cout << "Creating subthread failure!" << endl;
				return 0;
			}
			else {
				is_connect = true;
				cout << "Connect subthread successfully!" << endl;
			}
			break;
		}
		case 2: {
			//disconnect server
			shutdown(socket_client, SD_BOTH);
			closesocket(socket_client);//close socket
			WSACleanup();
			cout << "Disconnect successfully!" << endl;
			is_connect = false;
			break;
		}
		case 3: {
			//attain time
			SendDataBag sdb;
			ReceiveDataBag rdb;
			sdb.command = TIME;
			int ret = 0;
			//send message to server
			ret= send(socket_client, (char*)&sdb, sizeof(sdb), 0);//socket, request data packet location, size, 0
			if (ret == SOCKET_ERROR) {
				cout << "Sending failure!" << endl;
				return 0;
			}
			break;
		}
		case 4: {
			//attain name
			SendDataBag sdb;
			ReceiveDataBag rdb;
			//ReceiveDataBag rdb;
			sdb.command = NAME;

			//send message to server
			ret = send(socket_client, (char*)&sdb, sizeof(sdb), 0);//socket, request data packet location, size, 0
			if (ret == SOCKET_ERROR) {
				cout << "Sending failure!" << endl;
				return 0;
			}
			break;
		}
		case 5: {
			client_list();
			break;
		}
		case 6: {
			//The user enters the client list number and sent content
			client_list();
			char temp;
			int client_n;
			string message;
			SendDataBag sdb;
			cout << "CLIENT LIST:Press any button to continue" << endl;
			cin >> temp;
			cout << "INPUT client number:";
			cin >> client_n;
			getchar();//get line break
			cout << "INPUT message(enter twice to continue):";
			getline(cin, message);
			
			
			//Send message, the type is MESSAGE
			sdb.command = MESSAGE;
			sdb.suc = client_n;
			strcpy_s(sdb.dataforward, message.data());//Information is transferred to dataforward, string is converted to char[]

			ret = send(socket_client, (char*)&sdb, sizeof(sdb), 0);//socket, request data packet location, size, 0
			if (ret == SOCKET_ERROR) {
				cout << "Sending failure!" << endl;
				break;
			}
			else if (ret == 0) {
				cout << "already existed£¡" << endl;
				break;
			}
			break;
		}
		case 7: {
			//exit
			closesocket(socket_client);//close socket
			WSACleanup();
			cout << "Exit successfully!" << endl;
			flag = 0;//exit
			return 0;
		}
		default:
		{
			cout << "Invalid Order" << endl;
			break;
		}
		}
		if (flag == 0) break;
	}
	return 0;
}

unsigned __stdcall SubThread(void * pThis) {//subthread
/*Receive Message:
¢Ù Message arrives: processing
¢ÚResponse message/instruction message: print*/
	ReceiveDataBag rdb;
	int rec;
	char* ptr;//Pointer, which initially points to the beginning of the buffer
	int sp_buffer;//Remaining buffer size
	int temp = 0;
	
	while (1) {
		char* ptr = (char*)&rdb;//Pointer, which initially points to the beginning of the buffer
		int sp_buffer = sizeof(ReceiveDataBag);//Remaining buffer size
		
		while (sp_buffer > 0) {
			rec = recv(socket_client, ptr, sp_buffer, 0);//Returns the size of the received message
			if (rec == SOCKET_ERROR) {
				closesocket(socket_client);//close socket
				WSACleanup();
				cout << "Receiving failure!" << endl;
				return 0;
			}
			else if (rec == 0) {
				cout << "already exited£¡" << endl;
				return 0;
			}
			else { 
			sp_buffer -= rec;
			ptr += rec;
			}
		}
		if (rdb.command == IDC) {//Receive the instruction message and send the SEND_SUCCESS packet to the server
			cout << rdb.databack << endl;
		}
		else if (rdb.command == TIME) {//Received TIME response message
			char time[40];
			ctime_s(time, sizeof(time), (time_t*)rdb.databack);
			cout << time << endl;
			cout << "**" << ++temp << "**" << endl;
		}
		else if (rdb.command == NAME) {//Received TIME response message
			cout << rdb.databack << endl;
		}
		else if (rdb.command == CLIENTLIST) {//Received CLIENTLIST response message (optimization: classify the list content)
			cout << "socket"<<"\t"<<"ip"<<"\t"<<"port"<<"\t"<<"num"<< endl;
			int temp = atoi(rdb.databack);
			for (int i = 0; i <temp; i++) {
				cout << rdb.list[i].sNum << "\t"<<rdb.list[i].ip<<"\t"<<rdb.list[i].port <<"\t"<<i<< endl;
			}
		}
		else if (rdb.command == MESSAGE) {//Received CLIENTLIST response message (optimization: classify the list content)
			cout << "Server accept the message!" << endl;
			cout << rdb.databack << endl;
		}
		else if (rdb.command == SEND_SUCCESS) {//Received CLIENTLIST response message (optimization: classify the list content)
			cout << "SEND_SUCCESS" << endl;
			//cout << rdb.databack << endl;
		}
		else if (rdb.command == SEND_FAIL) {//Received CLIENTLIST response message (optimization: classify the list content)
			cout << "SEND_FAIL" << endl;
			cout << rdb.databack << endl;
		}
	}
	
	return 0;
}

void client_list() {
	//get client list
	SendDataBag sdb;
	ReceiveDataBag rdb;
	sdb.command = CLIENTLIST;
	int ret;
	//send message to server
	ret = send(socket_client, (char*)&sdb, sizeof(sdb), 0);//socket, request data packet location, size, 0
	if (ret == SOCKET_ERROR) {
		cout << "Sending failure!" << endl;
		return;
	}
}
