#include "uuid.h"

UUIDGenerator* UUIDGenerator::instance_ = NULL;

UUIDGenerator::UUIDGenerator()
{
	memset(&uuid, 0, sizeof(uuid));
}
UUIDGenerator* UUIDGenerator::getInstance()
{
	if (instance_ == NULL)
	{
		instance_ = new UUIDGenerator();
	}
	return instance_;
}

char* UUIDGenerator::getUUID()
{
	if (strlen(uuid) == 0)
	{
		MasterHardDiskSerial a;
		a.GetSerialNo(uuid);
	}
	return uuid;
}