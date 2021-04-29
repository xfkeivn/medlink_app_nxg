#include "VideoScreenControl.h"

VideoScreenControl* VideoScreenControl::mInstance = NULL;

VideoScreenControl::VideoScreenControl()
{
	m_hCom = nullptr;
	m_circleOperas = new CircleOperationQueue();
	init = false;
}

bool VideoScreenControl::Close()
{
	if (m_hCom > 0)
	{
		CloseHandle(m_hCom);
	}
	init = false;
	return true;
}

bool VideoScreenControl::Init(string com)
{
	std::wstring stemp = std::wstring(com.begin(), com.end());
	LPCWSTR sw = stemp.c_str();
	m_hCom = CreateFile(sw, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);//同步

	if (m_hCom == INVALID_HANDLE_VALUE)
	{
		logError("Failed to open " + com);
		return  FALSE;
	}
	SetupComm(m_hCom, 4096, 2048);
	COMMTIMEOUTS commtimeouts;
	commtimeouts.ReadIntervalTimeout = 100;
	commtimeouts.ReadTotalTimeoutMultiplier = 0;
	commtimeouts.ReadTotalTimeoutConstant = 1000; 
	commtimeouts.WriteTotalTimeoutMultiplier = 500;
	commtimeouts.WriteTotalTimeoutConstant = 2000;
	SetCommTimeouts(m_hCom, &commtimeouts);

	DCB dcb;
	GetCommState(m_hCom, &dcb);
	dcb.BaudRate = 115200;
	dcb.ByteSize = 8; //每个字节有8位 
	dcb.Parity = NOPARITY; //无奇偶校验位 
	dcb.StopBits = ONESTOPBIT; //两个停止位 
	SetCommState(m_hCom, &dcb);
	PurgeComm(m_hCom, PURGE_TXCLEAR | PURGE_RXCLEAR);
	init = true;
	return TRUE;
}

void VideoScreenControl::sendKeyRightOp()
{
	char operations[10];
	vector<char> cmd;
	cmd.push_back(0x4B);
	cmd.push_back(0x65);
	cmd.push_back(0x79);
	cmd.push_back(0x72);
	cmd.push_back(0x69);
	cmd.push_back(0x67);
	cmd.push_back(0x68);
	cmd.push_back(0x74);
	cmd.push_back(0x0D);
	cmd.push_back(0x0A);
	std::copy(cmd.begin(), cmd.end(), operations);
	writeData(operations, 10);
}

void VideoScreenControl::sendKeyDownOp()
{
	char operations[9];
	vector<char> cmd;
	cmd.push_back(0x4B);
	cmd.push_back(0x65);
	cmd.push_back(0x79);
	cmd.push_back(0x64);
	cmd.push_back(0x6F);
	cmd.push_back(0x77);
	cmd.push_back(0x6E);
	cmd.push_back(0x0D);
	cmd.push_back(0x0A);
	std::copy(cmd.begin(), cmd.end(), operations);
	writeData(operations, 9);
}

void VideoScreenControl::sendKeyLeftOp()
{
	char operations[9];
	vector<char> cmd;
	cmd.push_back(0x4B);
	cmd.push_back(0x65);
	cmd.push_back(0x79);
	cmd.push_back(0x6C);
	cmd.push_back(0x65);
	cmd.push_back(0x66);
	cmd.push_back(0x74);
	cmd.push_back(0x0D);
	cmd.push_back(0x0A);
	std::copy(cmd.begin(), cmd.end(), operations);
	writeData(operations, 9);
}

void VideoScreenControl::sendKeyUpOp()
{
	char operations[7];
	vector<char> cmd;
	cmd.push_back(0x4B);
	cmd.push_back(0x65);
	cmd.push_back(0x79);
	cmd.push_back(0x75);
	cmd.push_back(0x70);
	cmd.push_back(0x0D);
	cmd.push_back(0x0A);
	std::copy(cmd.begin(), cmd.end(), operations);
	writeData(operations, 7);
}

void VideoScreenControl::sendHD1Op()
{
	if (!init)
	{
		logWarn("Unabel to send HD1 cmd for the serial port is not initialized.");
		return;
	}
	char operations[5];
	vector<char> cmd;
	cmd.push_back(0x48);
	cmd.push_back(0x44);
	cmd.push_back(0x31);
	cmd.push_back(0x0D);
	cmd.push_back(0x0A);
	std::copy(cmd.begin(), cmd.end(), operations);
	writeData(operations, 5);
}

void VideoScreenControl::sendHD2Op()
{
	if (!init)
	{
		logWarn("Unabel to send HD2 cmd for the serial port is not initialized.");
		return;
	}
	char operations[5];
	vector<char> cmd;
	cmd.push_back(0x48);
	cmd.push_back(0x44);
	cmd.push_back(0x32);
	cmd.push_back(0x0D);
	cmd.push_back(0x0A);
	std::copy(cmd.begin(), cmd.end(), operations);
	writeData(operations, 5);
}
void VideoScreenControl::sendPipModeOp()
{
	if (!init)
	{
		logWarn("Unabel to send Pip cmd for the serial port is not initialized.");
		return;
	}
	char operations[9];
	vector<char> cmd;
	cmd.push_back(0x50);
	cmd.push_back(0x69);
	cmd.push_back(0x70);
	cmd.push_back(0x4D);
	cmd.push_back(0x6F);
	cmd.push_back(0x64);
	cmd.push_back(0x65);
	cmd.push_back(0x0D);
	cmd.push_back(0x0A);
	std::copy(cmd.begin(), cmd.end(), operations);
	writeData(operations, 9);
	//if (!getReadFileStatus())
	//{
	//	logError("Send Pip mode cmd to serial port, not getting feedback message.");
	//	//::MessageBox(NULL, L"Failed to switch pip mode, please check the device connection!", L"Error", MB_OK);
	//}
}

void VideoScreenControl::sendPupModeOp()
{
	if (!init)
	{
		logWarn("Unabel to send Pup cmd for the serial port is not initialized.");
		return;
	}
	char operations[9];
	vector<char> cmd;
	cmd.push_back(0x50);
	cmd.push_back(0x75);
	cmd.push_back(0x70);
	cmd.push_back(0x4D);
	cmd.push_back(0x6F);
	cmd.push_back(0x64);
	cmd.push_back(0x65);
	cmd.push_back(0x0D);
	cmd.push_back(0x0A);
	std::copy(cmd.begin(), cmd.end(), operations);
	writeData(operations, 9);
	//if (!getReadFileStatus())
	//{
	//	logError("Send Pup mode cmd to serial port, not getting feedback message.");
	//	//::MessageBox(NULL, L"Failed to switch pup mode, please check the device connection!", L"Error", MB_OK);
	//}
}

void VideoScreenControl::sendPbypModeOp()
{
	if (!init)
	{
		logWarn("Unabel to send Pbyp cmd for the serial port is not initialized.");
		return;
	}
	char operations[10];
	vector<char> cmd;
	cmd.push_back(0x50);
	cmd.push_back(0x62);
	cmd.push_back(0x79);
	cmd.push_back(0x70);
	cmd.push_back(0x4D);
	cmd.push_back(0x6F);
	cmd.push_back(0x64);
	cmd.push_back(0x65);
	cmd.push_back(0x0D);
	cmd.push_back(0x0A);
	std::copy(cmd.begin(), cmd.end(), operations);
	writeData(operations, 10);
	//if (!getReadFileStatus())
	//{
	//	logError("Send Pbyp mode cmd to serial port, not getting feedback message.");
	//	//::MessageBox(NULL, L"Failed to switch pbyp mode, please check the device connection!", L"Error", MB_OK);
	//}
}

void VideoScreenControl::sendSwapPortOp()
{
	if (!init)
	{
		logWarn("Unabel to send Swap cmd for the serial port is not initialized.");
		return;
	}
	char operations[10];
	vector<char> cmd;
	cmd.push_back(0x53);
	cmd.push_back(0x77);
	cmd.push_back(0x61);
	cmd.push_back(0x70);
	cmd.push_back(0x50);
	cmd.push_back(0x6F);
	cmd.push_back(0x72);
	cmd.push_back(0x74);
	cmd.push_back(0x0D);
	cmd.push_back(0x0A);
	std::copy(cmd.begin(), cmd.end(), operations);
	writeData(operations, 10);
	//if (!getReadFileStatus())
	//{
	//	logError("Send Swap mode cmd to serial port, not getting feedback message.");
	//	//::MessageBox(NULL, L"Failed to switch video, please check the device connection!", L"Error", MB_OK);
	//}
}

void VideoScreenControl::sendPicSizeOp()
{
	if (!init)
	{
		logWarn("Unabel to send Pic cmd for the serial port is not initialized.");
		return;
	}
	char operations[9];
	vector<char> cmd;
	cmd.push_back(0x50);
	cmd.push_back(0x69);
	cmd.push_back(0x63);
	cmd.push_back(0x53);
	cmd.push_back(0x69);
	cmd.push_back(0x7A);
	cmd.push_back(0x65);
	cmd.push_back(0x0D);
	cmd.push_back(0x0A);
	std::copy(cmd.begin(), cmd.end(), operations);
	writeData(operations, 9);
	//if (!getReadFileStatus())
	//{
	//	logError("Send PicSize cmd to serial port, not getting feedback message.");
	//	//::MessageBox(NULL, L"Failed to change video size, please check the device connection!", L"Error", MB_OK);
	//}
}

void VideoScreenControl::sendWinMode1Op()
{
	char operations[10];
	vector<char> cmd;
	cmd.push_back(0x57);
	cmd.push_back(0x49);
	cmd.push_back(0x4E);
	cmd.push_back(0x4D);
	cmd.push_back(0x4F);
	cmd.push_back(0x44);
	cmd.push_back(0x45);
	cmd.push_back(0x31);
	cmd.push_back(0x0D);
	cmd.push_back(0x0A);
	std::copy(cmd.begin(), cmd.end(), operations);
	writeData(operations, 10);
}
void VideoScreenControl::sendWinMode2Op()
{
	char operations[10];
	vector<char> cmd;
	cmd.push_back(0x57);
	cmd.push_back(0x49);
	cmd.push_back(0x4E);
	cmd.push_back(0x4D);
	cmd.push_back(0x4F);
	cmd.push_back(0x44);
	cmd.push_back(0x45);
	cmd.push_back(0x32);
	cmd.push_back(0x0D);
	cmd.push_back(0x0A);
	std::copy(cmd.begin(), cmd.end(), operations);
	writeData(operations, 10);
}
void VideoScreenControl::sendWinMode4Op()
{
	char operations[10];
	vector<char> cmd;
	cmd.push_back(0x57);
	cmd.push_back(0x49);
	cmd.push_back(0x4E);
	cmd.push_back(0x4D);
	cmd.push_back(0x4F);
	cmd.push_back(0x44);
	cmd.push_back(0x45);
	cmd.push_back(0x34);
	cmd.push_back(0x0D);
	cmd.push_back(0x0A);
	std::copy(cmd.begin(), cmd.end(), operations);
	writeData(operations, 10);
}
void VideoScreenControl::sendWinMode6Op()
{
	char operations[10];
	vector<char> cmd;
	cmd.push_back(0x57);
	cmd.push_back(0x49);
	cmd.push_back(0x4E);
	cmd.push_back(0x4D);
	cmd.push_back(0x4F);
	cmd.push_back(0x44);
	cmd.push_back(0x45);
	cmd.push_back(0x36);
	cmd.push_back(0x0D);
	cmd.push_back(0x0A);
	std::copy(cmd.begin(), cmd.end(), operations);
	writeData(operations, 10);
}
void VideoScreenControl::sendWinMode7Op()
{
	char operations[10];
	vector<char> cmd;
	cmd.push_back(0x57);
	cmd.push_back(0x49);
	cmd.push_back(0x4E);
	cmd.push_back(0x4D);
	cmd.push_back(0x4F);
	cmd.push_back(0x44);
	cmd.push_back(0x45);
	cmd.push_back(0x37);
	cmd.push_back(0x0D);
	cmd.push_back(0x0A);
	std::copy(cmd.begin(), cmd.end(), operations);
	writeData(operations, 10);
}
void VideoScreenControl::sendWinMode8Op()
{
	char operations[10];
	vector<char> cmd;
	cmd.push_back(0x57);
	cmd.push_back(0x49);
	cmd.push_back(0x4E);
	cmd.push_back(0x4D);
	cmd.push_back(0x4F);
	cmd.push_back(0x44);
	cmd.push_back(0x45);
	cmd.push_back(0x38);
	cmd.push_back(0x0D);
	cmd.push_back(0x0A);
	std::copy(cmd.begin(), cmd.end(), operations);
	writeData(operations, 10);
}

void VideoScreenControl::sendHDMI11Op()
{
	char operations[8];
	vector<char> cmd;
	cmd.push_back(0x48);
	cmd.push_back(0x44);
	cmd.push_back(0x4D);
	cmd.push_back(0x49);
	cmd.push_back(0x31);
	cmd.push_back(0x31);
	cmd.push_back(0x0D);
	cmd.push_back(0x0A);
	std::copy(cmd.begin(), cmd.end(), operations);
	writeData(operations, 8);
}
void VideoScreenControl::sendHDMI21Op()
{
	char operations[8];
	vector<char> cmd;
	cmd.push_back(0x48);
	cmd.push_back(0x44);
	cmd.push_back(0x4D);
	cmd.push_back(0x49);
	cmd.push_back(0x32);
	cmd.push_back(0x31);
	cmd.push_back(0x0D);
	cmd.push_back(0x0A);
	std::copy(cmd.begin(), cmd.end(), operations);
	writeData(operations, 8);
}
void VideoScreenControl::sendHDMI31Op()
{
	char operations[8];
	vector<char> cmd;
	cmd.push_back(0x48);
	cmd.push_back(0x44);
	cmd.push_back(0x4D);
	cmd.push_back(0x49);
	cmd.push_back(0x33);
	cmd.push_back(0x31);
	cmd.push_back(0x0D);
	cmd.push_back(0x0A);
	std::copy(cmd.begin(), cmd.end(), operations);
	writeData(operations, 8);
}
void VideoScreenControl::sendHDMI41Op()
{
	char operations[8];
	vector<char> cmd;
	cmd.push_back(0x48);
	cmd.push_back(0x44);
	cmd.push_back(0x4D);
	cmd.push_back(0x49);
	cmd.push_back(0x34);
	cmd.push_back(0x31);
	cmd.push_back(0x0D);
	cmd.push_back(0x0A);
	std::copy(cmd.begin(), cmd.end(), operations);
	writeData(operations, 8);
}
void VideoScreenControl::sendHDMI12Op()
{
	char operations[8];
	vector<char> cmd;
	cmd.push_back(0x48);
	cmd.push_back(0x44);
	cmd.push_back(0x4D);
	cmd.push_back(0x49);
	cmd.push_back(0x31);
	cmd.push_back(0x32);
	cmd.push_back(0x0D);
	cmd.push_back(0x0A);
	std::copy(cmd.begin(), cmd.end(), operations);
	writeData(operations, 8);
}
void VideoScreenControl::sendHDMI22Op()
{
	char operations[8];
	vector<char> cmd;
	cmd.push_back(0x48);
	cmd.push_back(0x44);
	cmd.push_back(0x4D);
	cmd.push_back(0x49);
	cmd.push_back(0x32);
	cmd.push_back(0x32);
	cmd.push_back(0x0D);
	cmd.push_back(0x0A);
	std::copy(cmd.begin(), cmd.end(), operations);
	writeData(operations, 8);
}
void VideoScreenControl::sendHDMI32Op()
{
	char operations[8];
	vector<char> cmd;
	cmd.push_back(0x48);
	cmd.push_back(0x44);
	cmd.push_back(0x4D);
	cmd.push_back(0x49);
	cmd.push_back(0x33);
	cmd.push_back(0x32);
	cmd.push_back(0x0D);
	cmd.push_back(0x0A);
	std::copy(cmd.begin(), cmd.end(), operations);
	writeData(operations, 8);
}
void VideoScreenControl::sendHDMI42Op()
{
	char operations[8];
	vector<char> cmd;
	cmd.push_back(0x48);
	cmd.push_back(0x44);
	cmd.push_back(0x4D);
	cmd.push_back(0x49);
	cmd.push_back(0x34);
	cmd.push_back(0x32);
	cmd.push_back(0x0D);
	cmd.push_back(0x0A);
	std::copy(cmd.begin(), cmd.end(), operations);
	writeData(operations, 8);
}
void VideoScreenControl::sendHDMI13Op()
{
	char operations[8];
	vector<char> cmd;
	cmd.push_back(0x48);
	cmd.push_back(0x44);
	cmd.push_back(0x4D);
	cmd.push_back(0x49);
	cmd.push_back(0x31);
	cmd.push_back(0x33);
	cmd.push_back(0x0D);
	cmd.push_back(0x0A);
	std::copy(cmd.begin(), cmd.end(), operations);
	writeData(operations, 8);
}
void VideoScreenControl::sendHDMI23Op()
{
	char operations[8];
	vector<char> cmd;
	cmd.push_back(0x48);
	cmd.push_back(0x44);
	cmd.push_back(0x4D);
	cmd.push_back(0x49);
	cmd.push_back(0x32);
	cmd.push_back(0x33);
	cmd.push_back(0x0D);
	cmd.push_back(0x0A);
	std::copy(cmd.begin(), cmd.end(), operations);
	writeData(operations, 8);
}
void VideoScreenControl::sendHDMI33Op()
{
	char operations[8];
	vector<char> cmd;
	cmd.push_back(0x48);
	cmd.push_back(0x44);
	cmd.push_back(0x4D);
	cmd.push_back(0x49);
	cmd.push_back(0x33);
	cmd.push_back(0x33);
	cmd.push_back(0x0D);
	cmd.push_back(0x0A);
	std::copy(cmd.begin(), cmd.end(), operations);
	writeData(operations, 8);
}
void VideoScreenControl::sendHDMI43Op()
{
	char operations[8];
	vector<char> cmd;
	cmd.push_back(0x48);
	cmd.push_back(0x44);
	cmd.push_back(0x4D);
	cmd.push_back(0x49);
	cmd.push_back(0x34);
	cmd.push_back(0x33);
	cmd.push_back(0x0D);
	cmd.push_back(0x0A);
	std::copy(cmd.begin(), cmd.end(), operations);
	writeData(operations, 8);
}
void VideoScreenControl::sendHDMI14Op()
{
	char operations[8];
	vector<char> cmd;
	cmd.push_back(0x48);
	cmd.push_back(0x44);
	cmd.push_back(0x4D);
	cmd.push_back(0x49);
	cmd.push_back(0x31);
	cmd.push_back(0x34);
	cmd.push_back(0x0D);
	cmd.push_back(0x0A);
	std::copy(cmd.begin(), cmd.end(), operations);
	writeData(operations, 8);
}
void VideoScreenControl::sendHDMI24Op()
{
	char operations[8];
	vector<char> cmd;
	cmd.push_back(0x48);
	cmd.push_back(0x44);
	cmd.push_back(0x4D);
	cmd.push_back(0x49);
	cmd.push_back(0x32);
	cmd.push_back(0x34);
	cmd.push_back(0x0D);
	cmd.push_back(0x0A);
	std::copy(cmd.begin(), cmd.end(), operations);
	writeData(operations, 8);
}
void VideoScreenControl::sendHDMI34Op()
{
	char operations[8];
	vector<char> cmd;
	cmd.push_back(0x48);
	cmd.push_back(0x44);
	cmd.push_back(0x4D);
	cmd.push_back(0x49);
	cmd.push_back(0x33);
	cmd.push_back(0x34);
	cmd.push_back(0x0D);
	cmd.push_back(0x0A);
	std::copy(cmd.begin(), cmd.end(), operations);
	writeData(operations, 8);
}
void VideoScreenControl::sendHDMI44Op()
{
	char operations[8];
	vector<char> cmd;
	cmd.push_back(0x48);
	cmd.push_back(0x44);
	cmd.push_back(0x4D);
	cmd.push_back(0x49);
	cmd.push_back(0x34);
	cmd.push_back(0x34);
	cmd.push_back(0x0D);
	cmd.push_back(0x0A);
	std::copy(cmd.begin(), cmd.end(), operations);
	writeData(operations, 8);
}
bool VideoScreenControl::writeData(LPCVOID data, DWORD len)
{
	COMSTAT ComStat;
	DWORD dwErrorFlags;
	BOOL bWriteStat;
	ClearCommError(m_hCom, &dwErrorFlags, &ComStat);
	DWORD dwBytesWrite = 0;
	bWriteStat = WriteFile(m_hCom, data, len, &dwBytesWrite, NULL);
	if (!bWriteStat) {
		return false;
	}

	PurgeComm(m_hCom, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
	return true;
}
void VideoScreenControl::sendRotateOp()
{
	if (!init)
	{
		logWarn("Unabel to send Rotate cmd for the serial port is not initialized.");
		return;
	}
	switch (m_circleOperas->getOperation())
	{
	case 0:
		sendKeyRightOp();
		break;
	case 1:
		sendKeyDownOp();
		break;
	case 2:
		sendKeyLeftOp();
		break;
	case 3:
		sendKeyUpOp();
		break;
	}
}

bool VideoScreenControl::testComPort(string com, int divided)
{
	if(Init(com))
	{
		if (divided == 2)
		{
			sendHD1Op();
		}
		else if (divided == 4)
		{
			sendWinMode1Op();
		}
		else
		{
			return false;
		}
		//读取串口返回值
		char str[100];
		DWORD wCount;//读取的字节数 
		BOOL bReadStat;
		bReadStat = ReadFile(m_hCom, str, 100, &wCount, NULL);
		Close();
		if (wCount > 0)
		{
			logInfo("Success to read serial port return value in " + com);
			return true;
		}
		else
		{
			logWarn("Failed to read serial port return value in " + com);
			return false;
		}
	}
	logWarn("Failed to test " + com + " for the serial port is not initialized.");
	return false;	
}

bool VideoScreenControl::getReadFileStatus()
{
	//读取串口返回值
	char str[100];
	DWORD wCount;//读取的字节数 
	BOOL bReadStat;
	bReadStat = ReadFile(m_hCom, str, 100, &wCount, NULL);
	if (wCount > 0)
		return true;
	else
		return false;
}