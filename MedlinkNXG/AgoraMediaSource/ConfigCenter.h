#pragma once
#include <string>
using namespace std;
class ConfigCenter
{
protected:
	string m_server_ip;
	string m_server_port;
	string m_websocket_port;
	string m_equipment_name;
	string m_hospital_name;
	string m_department_name;
	string m_user_groug_name;
	string m_support_contact_name;
	string m_support_contact_phone;
	bool m_rc_enable;
	bool m_is_host;
	string m_user_email;
	string m_user_password;
	string m_equipment_type_name;
	string m_hospital_type_name;
	string m_host_unique_serial_number;
public:
	
	// those configuration is used for host side only to identify the host
	virtual void    setEquipmentId(string id) = 0;
	virtual void	setEquipmentTypeName(string name) = 0;
	virtual void   setHospitalId(string id) = 0;
	virtual void	setHospitalName(string name) = 0;
	virtual void    setDepartmentName(string name) = 0;
	virtual void    setUserGroupName(string name) = 0;
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
	virtual string	getEquipmentTypeName() = 0;
	virtual string	getHospitalName() = 0;
	virtual bool    getRCEnable() = 0;
	virtual string    getUserEmail() = 0;
	virtual string    getUserPassword() = 0;
	virtual string    getUserGroupName() = 0;
	virtual bool   getIsHost() = 0;
	virtual string getWebSocketURI() = 0;

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
	 
	 void	setHospitalName(string name);
	 void    setDepartmentName(string name);
	 void    setUserGroupName(string name);
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
	 string	getEquipmentTypeName();
	 string	getHospitalName();
	 bool    getRCEnable();
	 bool    getIsHost();
	 string    getUserEmail();
	 string    getUserPassword();
	 string    getUserGroupName();



private:
	static shared_ptr<ConfigCenter>  m_config;
	void loadSetting();
};

