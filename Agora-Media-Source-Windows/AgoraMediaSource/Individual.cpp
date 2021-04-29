#include "stdafx.h"
#include "Individual.h"

Individual::Individual(wstring name, string userid, string telephone)
{
	this->wname = name;
	this->status = UserStatus::USER_STATUS_UNKNOWN;
	this->telephone = telephone;
	this->user_id = userid;
	this->imagepath = "";
}

Individual::~Individual()
{

}


wstring Individual::getWName()
{
	return this->wname;
}


void Individual::setWName(wstring w_name)
{
	this->wname = w_name;
}
UserStatus Individual::getStatus()
{
	return this->status;
}

void Individual::setStatus(UserStatus status)
{
	this->status = status;
}
string Individual::getTelephone()
{
	return this->telephone;
}
void Individual::setTelephone(const char* tel)
{
	this->telephone = tel;
}

string& Individual::getUserId()
{
	return this->user_id;
}


void Individual::setImagePath(const char* path)
{
	this->imagepath = path;
}

string Individual::getImagePath()
{
	return this->imagepath;
}

void Individual::setAdditional(const char* additional)
{
	this->additional = additional;
}

string Individual::getAdditional()
{
	return this->additional;
}
