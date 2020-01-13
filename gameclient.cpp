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
	std::vector<std::string> cmd;	/*	�����x�s���O	*/
	char buf[BUF_LENGH];

	/*	Recive Loop		*/
	for (;;) {
		memset(buf, 0, BUF_LENGH);
		recv_len = recvfrom(sock_id, buf, BUF_LENGH, 0, (struct sockaddr*)&si_client, &sadlen);
		if (recv_len <= 0) {
			perror("recvform() failed.");
			return;
		}
		//printf("������ -> %s \n", buf);
		commandHandle(buf);
	}
}
/*
 * ���~�B�z
 * @param err_msg Error message.
*/
void perror(char* err_msg) {
	printf("%s Error Code: %d\n", err_msg, WSAGetLastError());
	system("pause");
}
/*		�{���i�J�I	*/
int main() {
	/* Socket �P �ܼ�	 */
	struct sockaddr_in si_client;
	SOCKET sock;
	int length = sizeof(si_client);
	WSADATA wsa;
	std::string name;			//�O�s�W�l
	int chose = 0;				//�������
	/*		Socket ���Ҷ}��		*/
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
	std::cout << "Socket �إߧ���. ID: " << sock << std::endl;
	/*	���A�� Setup socket	*/
	memset(&si_client, 0, length);
	si_client.sin_family = AF_INET;
	si_client.sin_addr.s_addr = INADDR_ANY; //�Ҧ����d
	si_client.sin_port = htons(PORT);
	inet_pton(AF_INET, SERVER_ADDRESS, &si_client.sin_addr.s_addr);
	
	/*�ǰe�W�ٵ����A���ûP���A���j�w  �P�ɥ[�J��ж�*/
	do
	{
		printf("�п�J�z���m�W: ");
	} while ((std::cin >> name) && (name.length() < 4 || name.length() > 16)); //get

	if (sendto(sock, (NAME + name).c_str(), (int)strlen((NAME + name).c_str()),
		0, (struct sockaddr*)&si_client, length) == -1)
	{
		perror(ERR_SEND);
		return -1;
	}
	/*		�����ťPort		*/
	_beginthread(recv_thread, 0, (void *)sock);
	/*		�P�Ȥ�ݤ��ʴ`��		*/
	for (;;)
	{
		//�C������
		if (state == -1) { break; }
		//���ݦ��A�����n�ʥ]
		if (state == FALSE) {
			Sleep(WAIT_DELAY);
			continue;
		}
		std::cout << std::endl;
		//���o���a��ܨöǰe�����A��
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
		//���U�@�Ӫ��A
		state = 0;
	}
	closesocket(sock);
	WSACleanup();
	system("pause");
	return 0;
}