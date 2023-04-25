#pragma once
#include "windows.h"
#include <apiquery2.h>
#include <bitset>
#include <consoleapi.h>
#include <rpcndr.h>
#include <set>
#include "log_util.h"
#include "MessageProtocal.h"
#include <queue>
#include "commonFun.h"
class HIDControl
{
	typedef void (*OnError)(int errorcode);

private:
	bool remoteControl;
	HANDLE m_hCom;
	OVERLAPPED  m_ov;
	bitset<8> modifyKeySet;
	set<int> keys;
	double total_x_space = 0;
	double total_y_space = 0;
	//Remote Client Size
	int remote_win_width_;
	int remote_win_height_;
	//Polaris Size
	int target_win_width_;
	int target_win_height_;

	DWORD write_data(LPCVOID data, DWORD len);
	vector<unsigned char> make_key_Cmd(RTMMessagEvent& event);
	int transformKeyCode(int keyValue);

	vector<unsigned char> make_mouse_command(RTMMessagEvent& event);
	vector<unsigned char> get_mouse_position(int x, int y);
	vector<unsigned char> get_mouse_position_rel(int x, int y);
	static HIDControl* mInstance;
	HIDControl(int width, int height);
	void sendMouseMoveEventToHID(int x_symbol, int y_symbol);

public:
	queue<RTMMessagEvent> relativeEvents;
	static HIDControl *GetInstance()
	{
		if (mInstance == NULL)
			mInstance = new HIDControl(1280, 1024);
		return mInstance;
	}
	void SetRemoteSize(int width, int height);
	BOOL Init(string comport, int baudrate);
	BOOL SendEventToHID(RTMMessagEvent& event);
	void SendResetMouseEventToHID();
	void SendRelativeEventToHID(RTMMessagEvent& event);
	BOOL Close();
};
