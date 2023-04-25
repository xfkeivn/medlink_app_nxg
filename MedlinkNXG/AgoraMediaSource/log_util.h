#ifndef _MYLOG_H
#define _MYLOG_H

#include "log4cpp/Category.hh"
#include <log4cpp/Appender.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/Priority.hh>
#include <log4cpp/PatternLayout.hh>
#include <log4cpp/RollingFileAppender.hh>
#include <log4cpp/Win32DebugAppender.hh>

class MyLog
{
public:
	static MyLog* getInstance();
	static void destroy();

	void error(const char* msg);
	void warn(const char* msg);
	void info(const char* msg);
	void debug(const char* msg);

private:
	MyLog();

private:
	static MyLog* instance;
	static log4cpp::Category& root;
};

//*****************************************************
//ע�⣺
//�ļ��� __FILE__ ,������ __func__ ���к�__LINE__ �Ǳ�����ʵ�ֵ�
//����C++ͷ�ļ��ж���� 
//ǰ����������string���ͣ���__LINE__�����Σ�������ҪתΪstring����
//******************************************************

//���������ļ��к� ->ת��Ϊstring����
inline std::string int2string(int line) {
	std::ostringstream oss;
	oss << line;
	return oss.str();
}

#define filename(x) strrchr(x,'\\')?strrchr(x,'\\')+1:x

//����һ������־ǰ��� �ļ��� ������ �к� �ĺ궨��
//#define prefix(msg)  std::string("[ ").append(filename(__FILE__))\
//                     .append("( at line ").append(int2string(__LINE__)).append(" ) ] - ")\
//                     .append(msg).c_str()

//#define prefix(msg) std::string(msg).c_str()

#define prefix(msg)  std::string("[ ").append(filename(__FILE__)).append(" ] - ")\
                     .append(msg).c_str()

//����ÿ��ʹ��ʱд getInstance���
//ֻ��Ҫ���������ļ���д: #define _LOG4CPP_����������������ʹ��


//���̲��򻯺���������ʽ
#define logError(msg)	MyLog::getInstance()->error(prefix(msg))
#define logWarn(msg)	MyLog::getInstance()->warn(prefix(msg))
#define logInfo(msg)	MyLog::getInstance()->info(prefix(msg))
#define logDebug(msg)	MyLog::getInstance()->debug(prefix(msg))

#endif
