#pragma once

#include <string>
#include "client_common.h"
#include "string_util.h"
using namespace std;

class Group;
class Individual
{
public:
	Individual(wstring name, string user_id, string telephone);
	virtual ~Individual();
	wstring getWName();
	void setWName(wstring w_name);
	UserStatus getStatus();
	void setStatus(UserStatus status);
	string getTelephone();
	void setTelephone(const char* tel);
	string& getUserId();
	bool isGroup() {
		return false;
	};

	void setImagePath(const char* path);
	string getImagePath();
	void setAdditional(const char* additional);
	string getAdditional();
private:
	wstring wname;
	UserStatus status;
	string telephone;
	string additional;
	string user_id;
	string imagepath;
	Group* group;
};