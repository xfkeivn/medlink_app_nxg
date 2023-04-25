#pragma once

#include "HardDriveSerialNumer.h"

class UUIDGenerator
{
private:
	UUIDGenerator();
	UUIDGenerator(const UUIDGenerator&) {};
	const UUIDGenerator& operator=(const UUIDGenerator&) {};

	char uuid[1024];
	static UUIDGenerator* instance_;
public:
	static UUIDGenerator* getInstance();
	char* getUUID();
};