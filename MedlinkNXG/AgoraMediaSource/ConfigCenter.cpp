#include "stdafx.h"
#include "ConfigCenter.h"
#include "commonFun.h"
shared_ptr<ConfigCenter> RegConfig::m_config = nullptr;
void RegConfig::loadSetting()
{
	this->m_server_ip= cs2s(readRegKey(WEBSERVERIP, APP_REG_DIR));
	this->m_server_port = cs2s(readRegKey(WEBSERVERPORT, APP_REG_DIR));
	this->m_websocket_port = cs2s(readRegKey(WEBSOCKETPORT, APP_REG_DIR));
	this->m_equipment_name = cs2s(readRegKey(EQUIPMENT_NAME, APP_REG_DIR));
	this->m_rc_enable = CString2BOOL(readRegKey(RCENABLE, APP_REG_DIR));
	this->m_is_host = CString2BOOL(readRegKey(IS_HOST_APP, APP_REG_DIR));
	this->m_hospital_name  = cs2s(readRegKey(HOSPITAL_NAME, APP_REG_DIR));
	this->m_department_name = cs2s(readRegKey(DEPARTMENT_NAME, APP_REG_DIR));
	this->m_user_groug_name = cs2s(readRegKey(USERGROUP_NAME, APP_REG_DIR));
	
	this->m_user_email = cs2s(readRegKey(USEREMAIL, APP_REG_DIR));
	this->m_user_password = cs2s(readRegKey(USERPASSWORD, APP_REG_DIR));
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

void   RegConfig::setEquipmentId(string id)
{
	writeRegKey(EQUIPMENT_ID, utf82cs(id), APP_REG_DIR);
}
void	RegConfig::setEquipmentTypeName(string name) {
	writeRegKey(EQUIPMENT_NAME, utf82cs(name), APP_REG_DIR);
}
void	RegConfig::setHospitalName(string name) {
	writeRegKey(HOSPITAL_NAME, utf82cs(name), APP_REG_DIR);
}
void	RegConfig::setHospitalId(string id) {
	writeRegKey(HOSPITAL_ID, utf82cs(id), APP_REG_DIR);
}
void    RegConfig::setDepartmentName(string name) {
	writeRegKey(DEPARTMENT_NAME, utf82cs(name), APP_REG_DIR);
}
void    RegConfig::setUserGroupName(string name) {
	writeRegKey(USERGROUP_NAME, utf82cs(name), APP_REG_DIR);
}
void   RegConfig::setUserEmail(string user_email) {
	writeRegKey(USEREMAIL, utf82cs(user_email), APP_REG_DIR);
}
void   RegConfig::setUserPassword(string password) {
	writeRegKey(USEREMAIL, utf82cs(password), APP_REG_DIR);
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
	return m_server_port;
}

string RegConfig::getWebSocketURI()
{
	return m_server_ip + ":" + m_websocket_port;

}
string	RegConfig::getWebSocketPort() {
	return m_websocket_port;
}
string	RegConfig::getEquipmentTypeName() {
	return m_equipment_name;
}
string	RegConfig::getHospitalName() {
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
string    RegConfig::getUserGroupName() {
	return m_user_groug_name;
}



