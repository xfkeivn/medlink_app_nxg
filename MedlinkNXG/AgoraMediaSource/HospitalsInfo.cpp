#include "HospitalsInfo.h"

HospitalMgr::HospitalMgr()
{

}

void HospitalMgr::handleHospitalsInfo(string rsp)
{
	rapidjson::Document doc;
	if (!doc.Parse(rsp.data()).HasParseError())
	{
		logInfo("Receive hospital infos from webserver:" + rsp);
		if (doc.HasMember("Result") && doc["Result"].IsBool())
		{
			if (doc["Result"].GetBool())
			{
				if (doc.HasMember("Info") && doc["Info"].IsObject())
				{
					const rapidjson::Value& info = doc["Info"];
					if (info.HasMember("Hospitals") && info["Hospitals"].IsArray())
					{
						const rapidjson::Value& array = info["Hospitals"];
						size_t len = array.Size();
						for (size_t i = 0; i < len; i++)
						{
							const rapidjson::Value& hospital = array[i];
							if (hospital.IsObject())
							{
								string en_name = "";
								wstring cn_name = L"";
								if (hospital.HasMember("english_name") && hospital["english_name"].IsString())
								{
									en_name = hospital["english_name"].GetString();
								}
								if (hospital.HasMember("chinese_name") && hospital["chinese_name"].IsString())
								{
									cn_name = StringUtil::utf8string2wstring(hospital["chinese_name"].GetString());
								}
								Hospital* p_hospital = new Hospital(hospital["id"].GetInt(), cn_name, en_name);
								hospitals.push_back(p_hospital);
							}
						}
					}
					if (info.HasMember("Departments") && info["Departments"].IsArray())
					{
						const rapidjson::Value& array = info["Departments"];
						size_t len = array.Size();
						for (size_t i = 0; i < len; i++)
						{
							const rapidjson::Value& department = array[i];
							if (department.IsObject())
							{
								string en_name = "";
								wstring cn_name = L"";
								if (department.HasMember("english_name") && department["english_name"].IsString())
								{
									en_name = department["english_name"].GetString();
								}
								if (department.HasMember("chinese_name") && department["chinese_name"].IsString())
								{
									cn_name = StringUtil::utf8string2wstring(department["chinese_name"].GetString());
								}
								Department* p_department = new Department(department["id"].GetInt(), cn_name, en_name);
								departments.push_back(p_department);
							}
						}
					}
					if (info.HasMember("Equipments") && info["Equipments"].IsArray())
					{
						const rapidjson::Value& array = info["Equipments"];
						size_t len = array.Size();
						for (size_t i = 0; i < len; i++)
						{
							const rapidjson::Value& equipment = array[i];
							if (equipment.IsObject())
							{
								string name = "";
								if (equipment.HasMember("name") && equipment["name"].IsString())
								{
									name = equipment["name"].GetString();
								}
								Equipment* p_equipment = new Equipment(equipment["id"].GetInt(), name);
								equipments.push_back(p_equipment);
							}
						}
					}
				}
			}
		}
	}
	else
	{
		logError("Error in parsing hospital infos from webserver");
	}
}