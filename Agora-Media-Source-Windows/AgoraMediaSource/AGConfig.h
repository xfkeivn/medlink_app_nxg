#pragma once
class CAGConfig
{
public:
	

	int GetCodec();
	BOOL SetCodec(int nCodec);

/*	int GetResolution();
	BOOL SetResolution(int nResolution);

	int GetFPS();
	BOOL SetFPS(int nPFS);

	int GetMaxRate();
	BOOL SetMaxRate(int nMaxRate);
*/
	BOOL EnableAutoSave(BOOL bEnable);
	BOOL IsAutoSaveEnabled();

    BOOL SetSolution(int nIndex);
    int GetSolution();

    //CString GetAppid();
    //void SetAppid(CString appid);

	//BOOL IsHost();
	//BOOL SetIsHost(BOOL isHost);
	//CString GetEmail();
	//void SetEmail(CString email);



	//CString GetPassword();
	//void SetPassword(CString password);

	CString GetRememberMe();
	void SetRememberMe(CString isRememberMe);

	//CString GetWebServerIP();
	//void setWebServerIP(CString ip);

	//CString GetWebServerPort();
	//void setWebServerPort(CString port);

	/*CString GetHospitalId();
	void SetHospitalId(CString hospital_id);

	CString GetEquipmentId();
	void SetEquipmentId(CString equipment_id);

	CString GetDepartmentId();
	void SetDepartmentId(CString department_id);

	CString GetHospitalName();
	void SetHospitalName(CString hospital_name);

	CString GetEquipmentName();
	void SetEquipmentName(CString equipment_name);

	CString GetDepartmentName();
	void SetDepartmentName(CString department_name);*/

	//void setRCEnable(BOOL enable);
	//BOOL isRCEnable();

	int GetTargetWinWidth();
	int GetTargetWinHeight();
	double GetRate();
	CString GetVersion();
	BOOL isRemoteMouseShow();

    CString GetFilePath() {return m_szConfigFile;}
	CString GetVideoScreenCom();
	void SetVideoScreenCom(CString com);
	CString GetDeviceCom();
	void SetDeviceCom(CString com);
	int GetBaudrate();
	vector<CString> GetExcludedCameraNames();
	vector<CString> GetCOMDeviceName();
	int GetPictureDivider();
public:

	static CAGConfig* GetInstance();
	~CAGConfig();
private:
	//CString appid;
	static CAGConfig* m_agConfigInstance;
	CAGConfig();
	CString  m_szConfigFile;

};

