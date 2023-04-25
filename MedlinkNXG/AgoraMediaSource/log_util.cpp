#include "log_util.h"

using namespace std;

MyLog* MyLog::instance = NULL;
log4cpp::Category& MyLog::root = log4cpp::Category::getRoot().getInstance("SystemLog");

MyLog* MyLog::getInstance()
{
	if (instance == NULL)
	{
		instance = new MyLog();
	}
	return instance;
}

void MyLog::destroy()
{
	if (instance)
	{
		instance->root.shutdown();
		delete instance;
	}
}

MyLog::MyLog()
{
	log4cpp::PatternLayout* pLayout1 = new log4cpp::PatternLayout();
	pLayout1->setConversionPattern("%d: %p %c %x: %m%n");

	log4cpp::RollingFileAppender* rollfileAppender = new log4cpp::RollingFileAppender("MedLinkLog", "MedLink.log", 1000 * 1024, 10);
	rollfileAppender->setLayout(pLayout1);

	log4cpp::PatternLayout* pLayout2 = new log4cpp::PatternLayout();
	pLayout2->setConversionPattern("%d: %p %c %x: %m%n");

	log4cpp::Win32DebugAppender* win32DebugAppender = new log4cpp::Win32DebugAppender("MedLinkLog");
	win32DebugAppender->setLayout(pLayout2);

	//root = log4cpp::Category::getRoot().getInstance("RootName");
	root.addAppender(rollfileAppender);
	root.addAppender(win32DebugAppender);
	root.setPriority(log4cpp::Priority::DEBUG);
}

void MyLog::error(const char* msg)
{
	root.error(msg);
}

void MyLog::warn(const char* msg)
{
	root.warn(msg);
}

void MyLog::info(const char* msg)
{
	root.info(msg);
}

void MyLog::debug(const char* msg)
{
	root.debug(msg);
}

