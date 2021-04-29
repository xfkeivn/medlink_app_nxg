#pragma once
#include <string>
#include <vector>
#include <map>
#include "stringbuffer.h"
#include "writer.h"
#include "document.h"
#include "string_util.h"
#include "log_util.h"
using namespace std;
class Hospital
{
private:
	int id;
	wstring cn_name;
	string en_name;

public:
	Hospital(int id, wstring cnname, string enname) {
		this->id = id;
		this->en_name = enname;
		this->cn_name = cnname;
	};
	virtual ~Hospital() {};



public:
	wstring getCNName() { return cn_name; };
	string getENName() { return en_name; };
	int    getId() { return id; };

};

class Department
{
private:
	int id;
	wstring cn_name;
	string en_name;

public:
	Department(int id, wstring cnname, string enname) { 
		this->id = id;  
		this->cn_name = cnname;
		this->en_name = enname;
	};
	virtual ~Department() {};



public:
	string getENName() {
		return en_name;
	};
	wstring getCNName() { 
		return cn_name;
	};
	int getId() {
		return id;
	};
};

class Equipment
{
private:
	int id;
	string name;

public:
	Equipment(int id, string name) {
		this->id = id;
		this->name = name;
	};
	virtual ~Equipment() {};

public:
	string getName() { return this->name; };
	int getId() { return this->id; };
};

class HospitalMgr
{
public:
	HospitalMgr();
	virtual ~HospitalMgr() {};

private:
	vector<Hospital*> hospitals;
	vector<Department*> departments;
	vector<Equipment*> equipments;

public:
	vector<Hospital*> getHospitals() {
		return hospitals;
	};
	vector<Department*> getDepartments() {
		return departments;
	};
	vector<Equipment*> getEquipments() {
		return equipments;
	};

	void handleHospitalsInfo(string rsp);
};