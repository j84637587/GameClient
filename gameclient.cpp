#define _CRT_SECURE_NO_WARNINGS
#include <winsock2.h>
#include <WS2tcpip.h>
#include <process.h>
#include "Consts.hpp"
/*		Defination	*/
const uint16_t PORT = 7171;
#define BUF_LENGH 512
#define SERVER_ADDRESS "127.0.0.1"

/*
	Multiple recv handling
	@param *param The binded socket ID.
*/
void recv_thread(void *param) {
	int recv_len;
	SOCKET sock_id = (SOCKET)param;
	struct sockaddr_in si_client;
	int sadlen = sizeof(si_client);
	std::vector<std::string> cmd;	/*	拿來儲存指令	*/
	char buf[BUF_LENGH];

	/*	Recive Loop		*/
	for (;;) {
		memset(buf, 0, BUF_LENGH);
		recv_len = recvfrom(sock_id, buf, BUF_LENGH, 0, (struct sockaddr*)&si_client, &sadlen);
		if (recv_len <= 0) {
			perror("recvform() failed.");
			return;
		}
		//printf("接收到 -> %s \n", buf);
		commandHandle(buf);
	}
}
/*
 * 錯誤處理
 * @param err_msg Error message.
*/
void perror(char* err_msg) {
	printf("%s Error Code: %d\n", err_msg, WSAGetLastError());
	system("pause");
}
/*		程式進入點	*/
int main() {
	/* Socket 與 變數	 */
	struct sockaddr_in si_client;
	SOCKET sock;
	int length = sizeof(si_client);
	WSADATA wsa;
	std::string name;			//保存名子
	int chose = 0;				//紀錄選擇
	/*		Socket 環境開啟		*/
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		perror("WSAStartup failed.");
		return -1;
	}
	printf("Windows Socket Env. Initialization Successful. \n");
	if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR) {
		perror("socket() failed.\n");
		exit(EXIT_FAILURE);
	}
	std::cout << "Socket 建立完成. ID: " << sock << std::endl;
	/*	伺服器 Setup socket	*/
	memset(&si_client, 0, length);
	si_client.sin_family = AF_INET;
	si_client.sin_addr.s_addr = INADDR_ANY; //所有網卡
	si_client.sin_port = htons(PORT);
	inet_pton(AF_INET, SERVER_ADDRESS, &si_client.sin_addr.s_addr);
	
	/*傳送名稱給伺服器並與伺服器綁定  同時加入到房間*/
	do
	{
		printf("請輸入您的姓名: ");
	} while ((std::cin >> name) && (name.length() < 4 || name.length() > 16)); //get

	if (sendto(sock, (NAME + name).c_str(), (int)strlen((NAME + name).c_str()),
		0, (struct sockaddr*)&si_client, length) == -1)
	{
		perror(ERR_SEND);
		return -1;
	}
	/*		持續監聽Port		*/
	_beginthread(recv_thread, 0, (void *)sock);
	/*		與客戶端互動循環		*/
	for (;;)
	{
		//遊戲結束
		if (state == -1) { break; }
		//等待伺服器的要封包
		if (state == FALSE) {
			Sleep(WAIT_DELAY);
			continue;
		}
		std::cout << std::endl;
		//取得本地選擇並傳送給伺服器
		do
		{
			chose = -1;
			std::cout << MSG_CHOSE << std::endl;
		} while (!(std::cin >> chose) || !(chose == 1 || chose == 0));

		if (sendto(sock, (CHOSE + det_Chose(chose)).c_str(),
			(int)strlen((CHOSE + det_Chose(chose)).c_str()), 0, (struct sockaddr*)&si_client, length) == -1) 
		{
			perror(ERR_SEND);
			system("pause");
			exit(EXIT_FAILURE);
		}
		//轉到下一個狀態
		state = 0;
	}
	closesocket(sock);
	WSACleanup();
	system("pause");
	return 0;
}