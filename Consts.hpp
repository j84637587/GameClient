#ifndef Consts_hpp
#define Consts_hpp

#include <iostream>
#include <string>
#include <vector>
#include <sstream>

/*		函式宣告	*/
std::vector<std::string> commandParser(const std::string);
void commandHandle(const std::string);

/*		遊戲訊息	*/
#define MSG_CHOSE "請做出選擇: \n(0) 結束探險\n(1) 繼續探險\n"
//#define MSG_WAITTING "Wait for server..."

/*		錯誤訊息	*/
#define ERR_SEND "傳送訊息失敗"

#define WAIT_DELAY 500	//等待封包間隔

/*		操作碼 傳給伺服端用	*/
#define NAME					"name;"
#define CHOSE					"chose;"

/*		操作碼 辨識伺服端封包用 沒有';'	*/
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

int state = FALSE;	//預設 0 (關閉)

/*
 * 辨識指令並做出相對應處理
 * @param cmd 要辨識的字串指令
 */
void commandHandle(const std::string cmd) {
	std::vector<std::string> v_cmd = commandParser(cmd);
	int size = (int)v_cmd.size();
	if (size < 1) { return; }	//DROP  下面的要家 長度辨識
	if (v_cmd[0] == GETCHOSE) {		//伺服端要求選擇
		state = TRUE;
		//std::cerr << "伺服器要求選擇" << std::endl;
		return;
	}
	if (v_cmd[0] == PLAYERLIST) {	//玩家清單
		int size = v_cmd.size();
		std::cerr << "目前玩家: ";
		for (int i=1; i < size; i++) {
			std::cout << i << ". " << v_cmd[i] << std::endl;
		}
		std::cerr << std::endl;
		return;
	}
	if (v_cmd[0] == PLAYERLISTFULL) {
		std::cerr << "目前玩家資訊" << v_cmd[1] << std::endl;
		int pNumber = (size - 1)/3;
		std::string state;
		for (int i = 1; i < size; i += 3) { //		全部玩家資訊 playerlistfull;name;score;state
			state = (v_cmd[i + 2] == "1") ? "探險" : "返回";
			std::cerr << "玩家 " << v_cmd[i] << "  目前獲得分數: " << v_cmd[i + 1] << "  狀態為: " << state << std::endl;
		}
		return;
	}
	if (v_cmd[0] == INFO) {		//遊戲資訊  info;score;*****; {(total, score, status)()()}
		std::cerr << "場上分數: " << v_cmd[1] << std::endl;
		std::cerr << "怪物卡: ";
		for (int i = 0; i < 5; i++) {	//取得每個怪物
			std::cout << "第" << i+1 << "種: " << v_cmd[2][i] << "  ";
		}
		return;
	}
	if (v_cmd[0] == ROUND) {
		std::cerr << "回合 #" << v_cmd[1] << " 開始!" << std::endl;
		return;
	}
	if (v_cmd[0] == CARD) {
		if (v_cmd[1] == "1") 
			std::cout << "翻開了卡片!!  卡片類型: 怪物  種類: " << v_cmd[2] << std::endl;
		else if (v_cmd[1] == "0")
			std::cout << "翻開了卡片!!  卡片類型: 寶石  分數: " << v_cmd[2] << std::endl;
		else
			std::cout << "翻開了卡片!!  卡片類型: 神器卡  分數: " << v_cmd[2] << std::endl;
		return;
	}
	if (v_cmd[0] == GAME_END){ //遊戲結束
		std::cerr << "恭喜遊戲結束!!" << std::endl;
		int pNumber = (size - 1) / 3;
		//std::string state;
		for (int i = 1; i < v_cmd.size(); i+=3) { //		全部玩家資訊 playerlistfull;name;score;state
			std::cout << "第" << (i / 3)+1 << "名   玩家" << v_cmd[i] << "  最後得分: " << v_cmd[i + 1] << std::endl;
		}
		state = -1;
		return;
	}
	return;
}
/*
 * 解析指令
 * @param command 要分割的字串
 * @return 分割後的字串所組成的 vector
 */
std::vector<std::string> commandParser(const std::string command) {
	const char delim = ';';					//分割條件
	std::stringstream ss(command);
	std::vector<std::string> cv;
	std::string str;
	while (std::getline(ss, str, delim)) {
		cv.emplace_back(std::move(str));
	}
	return cv;		//返回結果
}

#endif // !Consts_hpp
