#include "stdafx.h"
#include "ConfigCenter.h"
#include "commonFun.h"
#include "AGConfig.h"
shared_ptr<ConfigCenter> RegConfig::m_config = nullptr;
void RegConfig::loadSetting()
{
	this->m_server_ip= cs2s(readRegKey(WEBSERVERIP, APP_REG_DIR));
	this->m_server_port = cs2s(readRegKey(WEBSERVERPORT, APP_REG_DIR));
	this->m_websocket_port = cs2s(readRegKey(WEBSOCKETPORT, APP_REG_DIR));
	this->m_equipment_name = (readRegKey(EQUIPMENT_NAME, APP_REG_DIR));
	this->m_rc_enable = CString2BOOL(readRegKey(RCENABLE, APP_REG_DIR));
	this->m_is_host = CString2BOOL(readRegKey(IS_HOST_APP, APP_REG_DIR));
	this->m_hospital_name  = (readRegKey(HOSPITAL_NAME, APP_REG_DIR));
	this->m_department_name = (readRegKey(DEPARTMENT_NAME, APP_REG_DIR));
	this->m_user_groug_name = (readRegKey(USERGROUP_NAME, APP_REG_DIR));
	this->m_department_id = cs2s(readRegKey(DEPARTMENT_ID, APP_REG_DIR));
	this->m_user_email = cs2s(readRegKey(USEREMAIL, APP_REG_DIR));
	this->m_user_password = cs2s(readRegKey(USERPASSWORD, APP_REG_DIR));
	this->m_hospital_id = cs2s(readRegKey(HOSPITAL_ID, APP_REG_DIR));
	this->m_is_relative_mouse = CString2BOOL(readRegKey(RELATIVE_MOUSE_POS, APP_REG_DIR));
}

shared_ptr<ConfigCenter>  RegConfig::Instance()
{
	if (m_config == nullptr)
	{
		
		auto rc = new RegConfig();
		rc->loadSetting();
		m_config = shared_ptr<RegConfig>(rc);
	}
	return m_config;
}

void	RegConfig::setEquipmentTypeName(wstring name)
{
	writeRegKey(EQUIPMENT_NAME, name.c_str(), APP_REG_DIR);
}

void   RegConfig::setEquipmentId(string id)
{
	writeRegKey(EQUIPMENT_ID, utf82cs(id), APP_REG_DIR);
}

void	RegConfig::setHospitalName(wstring name) {
	writeRegKey(HOSPITAL_NAME, name.c_str(), APP_REG_DIR);
}
void	RegConfig::setHospitalId(string id) {
	writeRegKey(HOSPITAL_ID, utf82cs(id), APP_REG_DIR);
}
string  RegConfig::getHospitalId()
{
	return m_hospital_id;
}
void    RegConfig::setDepartmentName(wstring name) {
	writeRegKey(DEPARTMENT_NAME, (name).c_str(), APP_REG_DIR);
}
wstring RegConfig::getDepartmentName()
{
	return this->m_department_name;
}
void    RegConfig::setUserGroupName(wstring name) {
	writeRegKey(USERGROUP_NAME, (name).c_str(), APP_REG_DIR);
}
void   RegConfig::setUserEmail(string user_email) {
	writeRegKey(USEREMAIL, utf82cs(user_email), APP_REG_DIR);
}
void   RegConfig::setIsRelativeMouse(bool relativeMouse) {
	if (relativeMouse)
		writeRegKey(RELATIVE_MOUSE_POS, L"True", APP_REG_DIR);
	else
		writeRegKey(RELATIVE_MOUSE_POS, L"False", APP_REG_DIR);
}
bool RegConfig::IsRelativeMouse()
{

	return m_is_relative_mouse;
}

void   RegConfig::setUserPassword(string password) {
	writeRegKey(USEREMAIL, utf82cs(password), APP_REG_DIR);
}

 void    RegConfig::setDepartmentId(string did)
{
	 writeRegKey(DEPARTMENT_ID, utf82cs(did), APP_REG_DIR);
}
 string    RegConfig::getDepartmentId()
{
	 return m_department_id;
}


string  RegConfig::getHostUniqueSerialNumber() {
	return "";
}

string    RegConfig::getSupportContactName() {
	return "";
}
string    RegConfig::getSupportContactPhone() {

	return "";
}

string	RegConfig::getWebServerIP() {
	return m_server_ip;
}
string	RegConfig::getWebServerPort() {
	return "7777";
	return m_server_port;
}

string RegConfig::getWebSocketURI()
{
	return m_server_ip + ":" + m_websocket_port;

}
string	RegConfig::getWebSocketPort() {
	return "24388";
	return m_websocket_port;
}
wstring	RegConfig::getEquipmentTypeName() {
	return m_equipment_name;
}
wstring	RegConfig::getHospitalName() {
	return m_hospital_name;
}
bool    RegConfig::getRCEnable() {
	return m_rc_enable;
}
bool    RegConfig::getIsHost() {
	return true;
	return m_is_host;
}
string    RegConfig::getUserEmail() {
	return m_user_email;
}
string    RegConfig::getUserPassword() {
	return m_user_password;
}
wstring    RegConfig::getUserGroupName() {
	return m_user_groug_name;
}

void RegConfig::updateClientLoginAuth(bool isRememberMe,wstring username,wstring password,bool authened)
{
		if (isRememberMe)
		{
			CAGConfig::GetInstance()->SetRememberMe(_T("True"));
			wstring email = username;
			CString emailCstr(email.c_str());
			writeRegKey(USEREMAIL, emailCstr, APP_REG_DIR);
			this->setUserEmail(StringUtil::wstring2utf8string(username));
			//CAGConfig::GetInstance()->SetEmail(emailCstr);
			if (authened)
			{
				
				CString passwordCstr(password.c_str());
				//CAGConfig::GetInstance()->SetPassword(passwordCstr);
				this->setUserPassword(StringUtil::wstring2utf8string(password));
				
			}
			else
			{
				//CAGConfig::GetInstance()->SetPassword(_T(""));
				this->setUserPassword("");
				
			}
		}
		else
		{
			CAGConfig::GetInstance()->SetRememberMe(_T("False"));
			//CAGConfig::GetInstance()->SetEmail(_T(""));
			//CAGConfig::GetInstance()->SetPassword(_T(""));
			writeRegKey(USEREMAIL, _T(""), APP_REG_DIR);
			writeRegKey(USERPASSWORD, _T(""), APP_REG_DIR);
		}
	
}


