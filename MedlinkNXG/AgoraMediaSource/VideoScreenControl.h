#pragma once
#include "windows.h"
#include <iostream>
#include <queue>
#include "log_util.h"
using namespace std;


class CircleOperationQueue
{
	//RIGHT 0
	//DOWN  1
	//LEFT  2
	//UP    3
public:
	CircleOperationQueue()
	{
		m_pointer = 0;
		m_pQueue = new int[4];
		for (int i = 0; i < 4; i++)
		{
			m_pQueue[i] = i;
		}
	};

	virtual ~CircleOperationQueue() {
		delete[] m_pQueue;
	};

	int getOperation()
	{
		if (m_pointer == 4)
		{
			m_pointer = 0;
		}
		int operation = m_pQueue[m_pointer];
		m_pointer++;
		return operation;		
	}
private:
	int* m_pQueue;
	int m_pointer;
};

class VideoScreenControl
{
private:
	static VideoScreenControl* mInstance;
	VideoScreenControl();
	HANDLE m_hCom;
	CircleOperationQueue* m_circleOperas;

	void sendKeyRightOp();
	void sendKeyDownOp();
	void sendKeyLeftOp();
	void sendKeyUpOp();


	bool writeData(LPCVOID data, DWORD len);
	bool init;
public:
	static VideoScreenControl *GetInstance()
	{
		if (mInstance == NULL)
			mInstance = new VideoScreenControl();
		return mInstance;
	}

	bool Init(string com);
	bool Close();
	void sendRotateOp();
	void sendHD1Op();
	void sendHD2Op();
	void sendPipModeOp();//»­ÖÐ»­
	void sendPupModeOp();//×óÓÒ
	void sendPbypModeOp();//ÉÏÏÂ
	void sendSwapPortOp();
	void sendPicSizeOp();
	void sendWinMode1Op();
	void sendWinMode2Op();
	void sendWinMode4Op();
	void sendWinMode6Op();
	void sendWinMode7Op();
	void sendWinMode8Op();
	void sendHDMI11Op();
	void sendHDMI21Op();
	void sendHDMI31Op();
	void sendHDMI41Op();
	void sendHDMI12Op();
	void sendHDMI22Op();
	void sendHDMI32Op();
	void sendHDMI42Op();
	void sendHDMI13Op();
	void sendHDMI23Op();
	void sendHDMI33Op();
	void sendHDMI43Op();
	void sendHDMI14Op();
	void sendHDMI24Op();
	void sendHDMI34Op();
	void sendHDMI44Op();
	bool testComPort(string com, int divided);
	bool getReadFileStatus();
};


