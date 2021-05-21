//#include "stdafx.h"
#include <iostream>

#include "HIDControl.h"

#include "tchar.h"
#include <fileapi.h>
#include <list>
#include <set>
#include <winbase.h>
#include <string>
#include <vector>
#include "MessageProtocal.h"

using namespace std;
HIDControl* HIDControl::mInstance = NULL;

HIDControl::HIDControl(int targetWith, int targetHeight)
{
	m_hCom = nullptr;
	ZeroMemory(&m_ov, sizeof(m_ov));
	target_win_width_ = targetWith;
	target_win_height_ = targetHeight;
	remoteControl = false;
}

void HIDControl::SetRemoteSize(int width, int height)
{
	remote_win_width_ = width;
	remote_win_height_ = height;
}


BOOL HIDControl::Init(string comport, int baudrate)
{
	DCB dcb;
	std::wstring stemp = std::wstring(comport.begin(), comport.end());
	LPCWSTR sw = stemp.c_str();
	m_hCom = CreateFile(sw, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);

	if (m_hCom == INVALID_HANDLE_VALUE)
	{
		logError("Failed to open " + comport);
		return  FALSE;
	}

	BOOL bSuccess = GetCommState(m_hCom, &dcb);

	if (!bSuccess)
	{
		HIDControl::Close();
		return  FALSE;
	}

	dcb.BaudRate = baudrate;

	bSuccess = SetCommState(m_hCom, &dcb);

	if (!bSuccess)
	{
		HIDControl::Close();
		return  FALSE;
	}

	SetupComm(m_hCom, 6000, 6000);

	COMMTIMEOUTS commtimeouts;
	commtimeouts.ReadIntervalTimeout = 0xFFFFFFF;
	commtimeouts.ReadTotalTimeoutMultiplier = 0;
	commtimeouts.ReadTotalTimeoutConstant = 1000;
	commtimeouts.WriteTotalTimeoutMultiplier = 2 * CBR_9600 / 9600;
	commtimeouts.WriteTotalTimeoutConstant = 0;
	SetCommTimeouts(m_hCom, &commtimeouts);

	PurgeComm(m_hCom, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
	EscapeCommFunction(m_hCom, SETDTR);

	return TRUE;
}

void HIDControl::sendMouseMoveEventToHID(int x, int y)
{
	//logInfo("x=" + to_string(x) + ", y=" + to_string(y));
	unsigned char arrMouse_rel[11];
	vector<unsigned char> position;

	position.push_back(x);
	position.push_back(y);

	std::vector<unsigned char> cmd;
	cmd.push_back(0x57);
	cmd.push_back(0xAB);
	cmd.push_back(0x00);
	cmd.push_back(0x05);
	cmd.push_back(0x05);
	cmd.push_back(0x01);
	cmd.push_back(0x00);
	cmd.push_back(position[0]);
	cmd.push_back(position[1]);
	cmd.push_back(0x00);

	int sum = 0;
	for (int i = 0; i < cmd.size(); i++)
	{
		sum += cmd.at(i);
	}
	sum = sum % 256;
	cmd.push_back(sum);
	std::copy(cmd.begin(), cmd.end(), arrMouse_rel);
	write_data(arrMouse_rel, sizeof(arrMouse_rel));
}

void HIDControl::SendResetMouseEventToHID()
{
	logInfo("SendResetMouseEventToHID");
	total_x_space = 0;
	total_y_space = 0;
	for (int i = 0; i < 5; i++)
	{
		Sleep(20);
		sendMouseMoveEventToHID(-127, -127);
	}
}

void HIDControl::SendRelativeEventToHID(RTMMessagEvent& event)
{

	auto rate_x = static_cast<double>(target_win_width_) / event.source_window_width;
	auto rate_y = static_cast<double>(target_win_height_) / event.source_window_height;

	double x_double = event.mouse_x * rate_x;
	double y_double = event.mouse_y * rate_y;
	int x = r(x_double);
	int y = r(y_double);
	double x_space = x_double - x;
	double y_space = y_double - y;
	total_x_space = total_x_space + x_space;
	total_y_space = total_y_space + y_space;
	logInfo("x_target=" + to_string(x) + ", y_target=" + to_string(y) + ",total_x_space=" + to_string(total_x_space) + ",total_y_space=" + to_string(total_y_space));
	int x_symbol = 1;
	int y_symbol = 1;
	if (x < 0)
	{
		x_symbol = -1;
	}
	if (y < 0)
	{
		y_symbol = -1;
	}
	int i;
	int x_level1 = abs(x) / 320;
	int x_level1_left = abs(x) % 320;
	int x_level2 = x_level1_left / 160;
	int x_level2_left = x_level1_left % 160;
	int x_level3 = x_level2_left / 40;
	int x_level3_left = x_level2_left % 40;
	int x_level4 = x_level3_left / 10;
	int x_level4_left = x_level3_left % 10;
	int y_level1 = abs(y) / 320;
	int y_level1_left = abs(y) % 320;
	int y_level2 = y_level1_left / 160;
	int y_level2_left = y_level1_left % 160;
	int y_level3 = y_level2_left / 40;
	int y_level3_left = y_level2_left % 40;
	int y_level4 = y_level3_left / 10;
	int y_level4_left = y_level3_left % 10;

	int level1 = max(x_level1, y_level1);
	int x_rel = 0;
	int y_rel = 0;
	for (i = 0; i < level1; i++)
	{
		x_rel = x_level1 - i > 0 ? (80 * x_symbol) : 0;
		y_rel = y_level1 - i > 0 ? (80 * y_symbol) : 0;
		Sleep(20);
		sendMouseMoveEventToHID(x_rel, y_rel);
	}
	int level2 = max(x_level2, y_level2);
	for (i = 0; i < level2; i++)
	{
		x_rel = x_level2 - i > 0 ? (40 * x_symbol) : 0;
		y_rel = y_level2 - i > 0 ? (40 * y_symbol) : 0;
		Sleep(20);
		sendMouseMoveEventToHID(x_rel, y_rel);
	}
	int level3 = max(x_level3, y_level3);
	for (i = 0; i < level3; i++)
	{
		x_rel = x_level3 - i > 0 ? (20 * x_symbol) : 0;
		y_rel = y_level3 - i > 0 ? (20 * y_symbol) : 0;
		Sleep(20);
		sendMouseMoveEventToHID(x_rel, y_rel);
	}
	int level4 = max(x_level4, y_level4);
	for (i = 0; i < level4; i++)
	{
		x_rel = x_level4 - i > 0 ? (10 * x_symbol) : 0;
		y_rel = y_level4 - i > 0 ? (10 * y_symbol) : 0;
		Sleep(20);
		sendMouseMoveEventToHID(x_rel, y_rel);
	}
	if (x_level4_left > 0 || y_level4_left > 0)
	{
		Sleep(20);
		sendMouseMoveEventToHID(x_level4_left * x_symbol, y_level4_left * y_symbol);
	}
	
	if (total_x_space >= 2)
	{
		logInfo("total_x_space=" + to_string(total_x_space) + ", move 2 pixels in x direction");
		Sleep(20);
		sendMouseMoveEventToHID(2, 0);
		total_x_space = total_x_space - 2;
	}
	else if(total_x_space <= -2)
	{
		logInfo("total_x_space=" + to_string(total_x_space) + ", move 2 pixels in -x direction");
		Sleep(20);
		sendMouseMoveEventToHID(-2, 0);
		total_x_space = total_x_space + 2;
	}
	if (total_y_space >= 2)
	{
		logInfo("total_y_space=" + to_string(total_y_space) + ", move 2 pixels in y direction");
		Sleep(20);
		sendMouseMoveEventToHID(0, 2);
		total_y_space = total_y_space - 2;
	}
	else if (total_y_space <= -2)
	{
		logInfo("total_y_space=" + to_string(total_y_space) + ", move 2 pixels in -y direction");
		Sleep(20);
		sendMouseMoveEventToHID(0, -2);
		total_y_space = total_y_space + 2;
	}

	if (event.msgtype != MOUSE_MOVE)
	{
		Sleep(20);
		SendEventToHID(RTMMessagEvent(event.msgtype, event.source_window_width, event.source_window_height, 0, 0, event.mouseButtonStatus));
	}		
}

BOOL HIDControl::SendEventToHID(RTMMessagEvent& event)
{
	//logInfo("SendEventToHID:x=" + to_string(event.mouse_x) + ",y=" + to_string(event.mouse_y));
	auto type = event.msgtype;
	vector<unsigned char> cmdMouse;
	vector<unsigned char> cmdKey;
	unsigned char arrKey[14];
	unsigned char arrMouse[13];

	switch (type)
	{
	case INVALID: break;
	case ANNOTATE_REQUEST: break;
	case ANNOTATE_GRANT: break;
	case ANNOTATE_REFUSE: break;
	case ANNOTATE_START: break;
	case ANNOTATE_END: break;
	case ANNOTATE_CLEAR: break;
	case REMOTE_CONTROL_GRANT: break;
	case REMOTE_CONTROL_TAKEBACK: break;
	case REMOTE_CONTROL_GIVEBACK: break;
	case MOUSE_MOVE:
	case MOUSE_LEFT_DOWN:
	case MOUSE_LEFT_UP:
	case MOUSE_RIGHT_DOWN:
	case MOUSE_RIGHT_UP:
		cmdMouse = make_mouse_command(event);
		std::copy(cmdMouse.begin(), cmdMouse.end(), arrMouse);
		write_data(arrMouse, sizeof(arrMouse));	
		break;
	case MOUSE_DOUBLE_CLICK:
		logInfo("========MOUSE_DOUBLE_CLICK==========");
		event.mouseButtonStatus = 4;
		cmdMouse = make_mouse_command(event);
		std::copy(cmdMouse.begin(), cmdMouse.end(), arrMouse);
		write_data(arrMouse, sizeof(arrMouse));
		event.mouseButtonStatus = 0;
		cmdMouse = make_mouse_command(event);
		std::copy(cmdMouse.begin(), cmdMouse.end(), arrMouse);
		write_data(arrMouse, sizeof(arrMouse));
		event.mouseButtonStatus = 4;
		cmdMouse = make_mouse_command(event);
		std::copy(cmdMouse.begin(), cmdMouse.end(), arrMouse);
		write_data(arrMouse, sizeof(arrMouse));		
		event.mouseButtonStatus = 0;
		cmdMouse = make_mouse_command(event);
		std::copy(cmdMouse.begin(), cmdMouse.end(), arrMouse);
		write_data(arrMouse, sizeof(arrMouse));
		break;
	case KEY_DOWN:
		logInfo("========KEY_DOWN==========");
	case KEY_UP:
		logInfo("========KEY_UP==========");
		cmdKey = make_key_Cmd(event);

		if (event.msgtype == KEY_UP) {
			int i = 0;
			//printf("UP:", event.keyCode);
			std::cout << "UP:" << event.keyCode << endl;
			//cout << port : << argv[1] << endl;
		}

		if (event.msgtype == KEY_DOWN) {
			int i = 0;
			//printf("DOWN:", event.keyCode);

			std::cout << "DOWN:" << event.keyCode << endl;
		}

		std::copy(cmdKey.begin(), cmdKey.end(), arrKey);

		write_data(arrKey, sizeof(arrKey));

		break;
	case KEY_RELEASEALL:

		modifyKeySet[0] = 0;
		modifyKeySet[1] = 0;
		modifyKeySet[2] = 0;
		modifyKeySet[3] = 0;
		modifyKeySet[4] = 0;
		modifyKeySet[5] = 0;
		modifyKeySet[6] = 0;
		modifyKeySet[7] = 0;

		keys.clear();
		cmdKey = make_key_Cmd(event);

		std::copy(cmdKey.begin(), cmdKey.end(), arrKey);

		write_data(arrKey, sizeof(arrKey));

		break;
	default:;
	}

	return TRUE;
}

BOOL HIDControl::Close()
{
	if (m_hCom > 0)
	{
		CloseHandle(m_hCom);
	}

	return TRUE;
}

DWORD HIDControl::write_data(LPCVOID data, DWORD len)
{
	BOOL bSuccess;
	DWORD written = 0;
	if (len < 1)return (0);

	m_ov.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (m_ov.hEvent == INVALID_HANDLE_VALUE)
	{
		return(-1);
	}

	WriteFile(m_hCom, data, len, &written, &m_ov);
	CloseHandle(m_ov.hEvent);

	return written;
}

int HIDControl::transformKeyCode(int keyValue) {
	int resultValue;
	//×ÖÄ¸
	if (keyValue >= 65 && keyValue <= 90) {
		resultValue = 0x04;

		int offset = keyValue - 65;

		resultValue += offset;

		return resultValue;
	}
	//×ÖÄ¸ÇøÓòÊý×Ö
	else if (keyValue >= 48 && keyValue <= 57) {
		if (keyValue == 48) {
			resultValue = 0x27;
		}

		else {
			resultValue = 0x1E;
			int offset = keyValue - 49;
			resultValue += offset;
		}

		return resultValue;
	}
	//Ð¡¼üÅÌÊý×Ö
	else if (keyValue >= 96 && keyValue <= 105) {
		if (keyValue == 96) {
			resultValue = 0x62;
		}
		else
		{
			resultValue = 0x59;
			int offset = keyValue - 97;
			resultValue += offset;
		}

		return resultValue;
	}
	else if (keyValue >= 112 && keyValue <= 123)
	{
		resultValue = 0x3A;

		int offset = keyValue - 112;

		resultValue += offset;
	}
	else
	{
		switch (keyValue)
		{
		case 27://esc
			return 0x29;
		case 8://backapace
			return 0x2A;
		case 9://tab
			return 0x2B;
		case 20://lock
			return 0x39;
		case 13://enter
			return 0x28;

		case 16://shift

			return 0x02;
		case 17://ctrl
			return 0x01;
			//case 18://alt
			//	return 0x04;
		case 32://¿Õ¸ñ
			return 0x2C;
			//case 12://clear
		case 45://Insert
			return 0x49;
		case 46://Delete
			return 0x4C;
		case 36://Home
			return 0x4A;
		case 35://End
			return 0x4D;
		case 33://Page up
			return 0x4B;

		case 34://Page down
			return 0x4E;

		case 37://¡û
			return 0x50;
		case 38://¡ü
			return 0x52;
		case 39://¡ú
			return 0x4F;
		case 40://¡ý
			return 0x51;
		case 144://Num lock
			return 0x53;
		case 192://`
			return 0x35;
		case 189://×ÖÄ¸ -
			return 0x2D;
		case 187://×ÖÄ¸+
			return 0x2E;
		case 219://{
			return 0x2F;
		case 221://}
			return 0x30;
		case 186://;
			return 0x33;
		case 222://'
			return 0x34;
		case 220:// ·´Ð±¸Ü
			return 0x31;
		case 188://¶ººÅ
			return 0x36;
		case 190://¾äºÅ
			return 0x37;
		case 191: // /
			return 0x38;
		case 111: // /
			return 0x38;
		case 106://Ð¡¼üÅÌ*
			return 0x55;
		case 109://Ð¡¼üÅÌ-
			return 0x56;
		case 107://Ð¡¼üÅÌ+
			return 0x57;
		case 110://Ð¡¼üÅÌ .
			return 0x63;
		case 91://Win
			return 0x08;

		default:
			break;
		}
	}
}

vector<unsigned char> HIDControl::get_mouse_position_rel(int x, int y) {
	//int target_win_width_ = 1920;
	//int target_win_height_ = 1536;
	auto rate_x = static_cast<double>(target_win_width_) / remote_win_width_;
	auto rate_y = static_cast<double>(target_win_height_) / remote_win_height_;

	double x_double = x * rate_x * 3 / 2;
	double y_double = y * rate_y * 3 / 2;
	int X_Cur = (x)* rate_x * 3 / 2;
	int Y_Cur = (y)* rate_y * 3 / 2;
	double x_space = x_double - X_Cur;
	double y_space = y_double - Y_Cur;
	logInfo("x_cur=" + to_string(X_Cur) + ", y_cur=" + to_string(Y_Cur) + ",x_space" + to_string(x_space) + ",y_space=" + to_string(y_space));
	vector<unsigned char> pos;

	pos.push_back(X_Cur);
	pos.push_back(Y_Cur);
	

	return pos;


}

vector<unsigned char> HIDControl::get_mouse_position(int x, int y)
{
	auto rate_x = static_cast<double>(x) / remote_win_width_;
	auto rate_y = static_cast<double>(y) / remote_win_height_;

	int target_x = (int)(target_win_width_ * rate_x);
	int target_y = (int)(target_win_height_ * rate_y);

	int X_Cur = (target_x * 4096) / target_win_width_;
	int Y_Cur = (target_y * 4096) / target_win_height_;

	X_Cur = X_Cur * 2 / 3;

	char xhigh = X_Cur / 256;
	char xlow = X_Cur % 256;
	char yhigh = Y_Cur / 256;
	char ylow = Y_Cur % 256;

	vector<unsigned char> pos;
	//unsigned char pos[4];

	/*pos[0] = xhigh;
	pos[1] = xlow;
	pos[2] = yhigh;
	pos[3] = ylow;*/

	pos.push_back(xlow);
	pos.push_back(xhigh);
	pos.push_back(ylow);
	pos.push_back(yhigh);

	return pos;
}

vector<unsigned char> HIDControl::make_key_Cmd(RTMMessagEvent& event) {
	bool flg = false;
	//bool& isModify = flg;

	int key = transformKeyCode(event.keyCode);
	bool isModify = true;

	switch (key)
	{
		//case 0x80: //Rwin
		//	//modifyKeySet[0] = 1;
		//
		//	break;
		//case 0x08:  //Lwin
		//	//modifyKeySet[1] = 1;
		//
		//	break;
		//case 0x40: //Ralt
		//	//modifyKeySet[2] = 1;
		//
		//	break;
		//case 0x04:  //Lalt
		//	//modifyKeySet[3] = 1;
		//
		//	break;
		//case 0x20: //Rshift
			//modifyKeySet[4] = 1;

		//	break;
	case 0x02:  //Lshift
		//modifyKeySet[5] = 1;
		break;
		//case 0x10:  //Rctrl
			//modifyKeySet[6] = 1;
		break;
	case 0x01:  //Lctrl
		//modifyKeySet[7] = 1;
		break;

		/*case 0xE0:
		case 0xE1:
		case 0xE2:
		case 0xE3:
		case 0xE4:
		case 0xE5:
		case 0xE6:
		case 0xE7:
			break;*/
	default:
		isModify = false;
		break;
	}

	//Win
	if (((event.modifierKey >> 0) & 1) == 1) {
		modifyKeySet[3] = 1;
	}
	else
	{
		modifyKeySet[3] = 0;
	}

	//Alt
	if ((((event.modifierKey >> 1) & 1) == 1)) {
		modifyKeySet[2] = 1;
	}
	else
	{
		modifyKeySet[2] = 0;
	}

	//Shift
	if (((event.modifierKey >> 2) & 1) == 1) {
		modifyKeySet[1] = 1;
	}
	else
	{
		modifyKeySet[1] = 0;
	}

	//Control
	if (((event.modifierKey >> 3) & 1) == 1) {
		modifyKeySet[0] = 1;
	}
	else
	{
		modifyKeySet[0] = 0;
	}

	if (event.msgtype == KEY_DOWN) {
		//ÆÕÍ¨°´¼ü
		if (!isModify) {
			if (keys.size() < 6) {
				keys.insert(key);
			}
		}
	}
	else if (event.msgtype == KEY_UP)
	{
		if (!isModify) {
			keys.erase(key);
		}
	}

	std::vector<unsigned char> cmd;
	cmd.push_back(0x57);
	cmd.push_back(0xAB);
	cmd.push_back(0x00);
	cmd.push_back(0x02);
	cmd.push_back(0x08);
	//1
	cmd.push_back(modifyKeySet.to_ulong());
	//2
	cmd.push_back(0x00);

	//µü´úÆ÷ Êä³ö
	int count = 0;
	for (set<int>::iterator it = keys.begin(); it != keys.end(); ++it) {
		cmd.push_back(*it);
		count++;
	}

	if (count < 6) {
		int last = 6 - count;

		for (int i = 0; i < last; i++) {
			cmd.push_back(0x00);
		}
	}

	int sum = 0;
	for (int i = 0; i < cmd.size(); i++)
	{
		sum += cmd.at(i);
	}
	sum = sum % 256;
	cmd.push_back(sum);

	return cmd;
}

vector<unsigned char> HIDControl::make_mouse_command(RTMMessagEvent& event)
{
	SetRemoteSize(event.source_window_width, event.source_window_height);

	if (event.msgtype == MOUSE_LEFT_DOWN) {
		int test = 0;
	}

	vector<unsigned char> position;
	position = get_mouse_position(event.mouse_x, event.mouse_y);
	char buttonstatus = 0;
	int st = event.mouseButtonStatus;
	if (((st >> 2) & 1) == 1)
	{
		buttonstatus |= (1 << 0);
	}

	if (((st >> 0) & 1) == 1)
	{
		buttonstatus |= (1 << 1);
	}

	if (((st >> 1) & 1) == 1)
	{
		buttonstatus |= (1 << 2);
	}

	std::vector<unsigned char> cmd;

	cmd.push_back(0x57);
	cmd.push_back(0xAB);
	cmd.push_back(0x00);
	cmd.push_back(0x04);
	cmd.push_back(0x07);
	cmd.push_back(0x02);
	cmd.push_back(buttonstatus);
	cmd.push_back(position[0]);
	cmd.push_back(position[1]);
	cmd.push_back(position[2]);
	cmd.push_back(position[3]);
	cmd.push_back(0x00);

	int sum = 0;
	for (int i = 0; i < cmd.size(); i++)
	{
		sum += cmd.at(i);
	}
	sum = sum % 256;

	cmd.push_back(sum);	

	return cmd;
}