#ifndef Consts_hpp
#define Consts_hpp

#include <iostream>
#include <string>
#include <vector>
#include <sstream>

/*		�禡�ŧi	*/
std::vector<std::string> commandParser(const std::string);
void commandHandle(const std::string);

/*		�C���T��	*/
#define MSG_CHOSE "�а��X���: \n(0) �������I\n(1) �~���I\n"
//#define MSG_WAITTING "Wait for server..."

/*		���~�T��	*/
#define ERR_SEND "�ǰe�T������"

#define WAIT_DELAY 500	//���ݫʥ]���j

/*		�ާ@�X �ǵ����A�ݥ�	*/
#define NAME					"name;"
#define CHOSE					"chose;"

/*		�ާ@�X ���Ѧ��A�ݫʥ]�� �S��';'	*/
#define GETCHOSE				"getchose"
#define PLAYERLIST			"playerlist"
#define PLAYERLISTFULL	"playerlistfull"
#define INFO						"info"
#define ROUND					"round"
#define CARD						"card"
#define GAME_END			"end"

std::string det_Chose(int c) {
	return c ? "1" : "0";
}

int state = FALSE;	//�w�] 0 (����)

/*
 * ���ѫ��O�ð��X�۹����B�z
 * @param cmd �n���Ѫ��r����O
 */
void commandHandle(const std::string cmd) {
	std::vector<std::string> v_cmd = commandParser(cmd);
	int size = (int)v_cmd.size();
	if (size < 1) { return; }	//DROP  �U�����n�a ���׿���
	if (v_cmd[0] == GETCHOSE) {		//���A�ݭn�D���
		state = TRUE;
		//std::cerr << "���A���n�D���" << std::endl;
		return;
	}
	if (v_cmd[0] == PLAYERLIST) {	//���a�M��
		int size = v_cmd.size();
		std::cerr << "�ثe���a: ";
		for (int i=1; i < size; i++) {
			std::cout << i << ". " << v_cmd[i] << std::endl;
		}
		std::cerr << std::endl;
		return;
	}
	if (v_cmd[0] == PLAYERLISTFULL) {
		std::cerr << "�ثe���a��T" << v_cmd[1] << std::endl;
		int pNumber = (size - 1)/3;
		std::string state;
		for (int i = 1; i < size; i += 3) { //		�������a��T playerlistfull;name;score;state
			state = (v_cmd[i + 2] == "1") ? "���I" : "��^";
			std::cerr << "���a " << v_cmd[i] << "  �ثe��o����: " << v_cmd[i + 1] << "  ���A��: " << state << std::endl;
		}
		return;
	}
	if (v_cmd[0] == INFO) {		//�C����T  info;score;*****; {(total, score, status)()()}
		std::cerr << "���W����: " << v_cmd[1] << std::endl;
		std::cerr << "�Ǫ��d: ";
		for (int i = 0; i < 5; i++) {	//���o�C�өǪ�
			std::cout << "��" << i+1 << "��: " << v_cmd[2][i] << "  ";
		}
		return;
	}
	if (v_cmd[0] == ROUND) {
		std::cerr << "�^�X #" << v_cmd[1] << " �}�l!" << std::endl;
		return;
	}
	if (v_cmd[0] == CARD) {
		if (v_cmd[1] == "1") 
			std::cout << "½�}�F�d��!!  �d������: �Ǫ�  ����: " << v_cmd[2] << std::endl;
		else if (v_cmd[1] == "0")
			std::cout << "½�}�F�d��!!  �d������: �_��  ����: " << v_cmd[2] << std::endl;
		else
			std::cout << "½�}�F�d��!!  �d������: �����d  ����: " << v_cmd[2] << std::endl;
		return;
	}
	if (v_cmd[0] == GAME_END){ //�C������
		std::cerr << "���߹C������!!" << std::endl;
		int pNumber = (size - 1) / 3;
		//std::string state;
		for (int i = 1; i < v_cmd.size(); i+=3) { //		�������a��T playerlistfull;name;score;state
			std::cout << "��" << (i / 3)+1 << "�W   ���a" << v_cmd[i] << "  �̫�o��: " << v_cmd[i + 1] << std::endl;
		}
		state = -1;
		return;
	}
	return;
}
/*
 * �ѪR���O
 * @param command �n���Ϊ��r��
 * @return ���Ϋ᪺�r��Ҳզ��� vector
 */
std::vector<std::string> commandParser(const std::string command) {
	const char delim = ';';					//���α���
	std::stringstream ss(command);
	std::vector<std::string> cv;
	std::string str;
	while (std::getline(ss, str, delim)) {
		cv.emplace_back(std::move(str));
	}
	return cv;		//��^���G
}

#endif // !Consts_hpp
