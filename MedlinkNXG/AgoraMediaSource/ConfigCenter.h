#pragma once
#include <string>
using namespace std;
class ConfigCenter
{
protected:
	string m_server_ip;
	string m_server_port;
	string m_websocket_port;
	wstring m_equipment_name;
	wstring m_hospital_name;
	string m_hospital_id;
	wstring m_department_name;
	string m_department_id;
	wstring m_user_groug_name;
	string m_support_contact_name;
	string m_support_contact_phone;
	bool m_rc_enable;
	bool m_is_host;
	bool m_is_relative_mouse;
	string m_user_email;
	string m_user_password;
	string m_equipment_type_name;
	string m_hospital_type_name;
	string m_host_unique_serial_number;
public:
	
	// those configuration is used for host side only to identify the host
	virtual void    setEquipmentId(string id) = 0;
	virtual void	setEquipmentTypeName(wstring name) = 0;
	virtual void   setHospitalId(string id) = 0;
	virtual void	setHospitalName(wstring name) = 0;
	virtual void    setDepartmentId(string did) = 0;
	virtual string    getDepartmentId() = 0;
	virtual void    setDepartmentName(wstring name) = 0;
	virtual void    setUserGroupName(wstring name) = 0;
	// this is for storing the user login information so that it can login automatically, client used only
	virtual void   setUserEmail(string user_email) = 0;
	virtual void   setUserPassword(string password) = 0;

	// this is the unique serial number, only for host use, to identify the host device for backend.
	virtual string  getHostUniqueSerialNumber() = 0;
	// the backend can set those information, so the user can see
	virtual string    getSupportContactName() = 0;
	virtual string    getSupportContactPhone() = 0;

	virtual string	getWebServerIP() = 0;
	virtual string	getWebServerPort() = 0;
	virtual string	getWebSocketPort() = 0;
	virtual wstring	getEquipmentTypeName() = 0;
	virtual wstring	getHospitalName() = 0;
	virtual string  getHospitalId() = 0;
	virtual wstring  getDepartmentName() = 0;
	virtual bool    getRCEnable() = 0;
	virtual string    getUserEmail() = 0;
	virtual string    getUserPassword() = 0;
	virtual wstring    getUserGroupName() = 0;
	virtual bool   getIsHost() = 0;
	virtual string getWebSocketURI() = 0;

	virtual void setIsRelativeMouse(bool relativeMouse) = 0;
	virtual bool IsRelativeMouse() = 0;

	virtual void updateClientLoginAuth(bool saveAuth,wstring username,wstring password,bool authened) = 0;


};

class RegConfig :public ConfigCenter
{

public:
	static shared_ptr<ConfigCenter>  Instance();

public:
	// those configuration is used for host side only to identify the host
	 void	setEquipmentTypeName(string name);
	 void   setEquipmentId(string id);
	 void   setHospitalId(string id);
	 
	 void	setHospitalName(wstring name);
	 void    setDepartmentName(wstring name);
	 virtual void    setDepartmentId(string did);
	 virtual string    getDepartmentId();

	 void    setUserGroupName(wstring name);
	// this is for storing the user login information so that it can login automatically, client used only
	 void   setUserEmail(string user_email);
	 void   setUserPassword(string password);

	// this is the unique serial number, only for host use, to identify the host device for backend.
	 string  getHostUniqueSerialNumber();
	// the backend can set those information, so the user can see
	 string    getSupportContactName();
	 string    getSupportContactPhone();

	 string	getWebServerIP();
	 string	getWebServerPort();
	 string	getWebSocketPort();
	 string getWebSocketURI();
	 wstring	getEquipmentTypeName();
	 wstring getDepartmentName();
	 wstring	getHospitalName();
	 string  getHospitalId();
	 bool    getRCEnable();
	 bool    getIsHost();
	 string    getUserEmail();
	 string    getUserPassword();
	 wstring    getUserGroupName();
	 void setIsRelativeMouse(bool relativeMouse);
	 bool IsRelativeMouse();
	 void	setEquipmentTypeName(wstring name);

	 void updateClientLoginAuth(bool saveAuth,wstring username,wstring password,bool authened);

private:
	static shared_ptr<ConfigCenter>  m_config;
	void loadSetting();
};

