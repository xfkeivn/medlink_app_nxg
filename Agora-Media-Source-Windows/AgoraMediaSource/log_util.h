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
//注意：
//文件名 __FILE__ ,函数名 __func__ ，行号__LINE__ 是编译器实现的
//并非C++头文件中定义的 
//前两个变量是string类型，且__LINE__是整形，所以需要转为string类型
//******************************************************

//整数类型文件行号 ->转换为string类型
inline std::string int2string(int line) {
	std::ostringstream oss;
	oss << line;
	return oss.str();
}

#define filename(x) strrchr(x,'\\')?strrchr(x,'\\')+1:x

//定义一个在日志前添加 文件名 函数名 行号 的宏定义
//#define prefix(msg)  std::string("[ ").append(filename(__FILE__))\
//                     .append("( at line ").append(int2string(__LINE__)).append(" ) ] - ")\
//                     .append(msg).c_str()

//#define prefix(msg) std::string(msg).c_str()

#define prefix(msg)  std::string("[ ").append(filename(__FILE__)).append(" ] - ")\
                     .append(msg).c_str()

//不用每次使用时写 getInstance语句
//只需要在主函数文件中写: #define _LOG4CPP_即可在整个程序中使用


//缩短并简化函数调用形式
#define logError(msg)	MyLog::getInstance()->error(prefix(msg))
#define logWarn(msg)	MyLog::getInstance()->warn(prefix(msg))
#define logInfo(msg)	MyLog::getInstance()->info(prefix(msg))
#define logDebug(msg)	MyLog::getInstance()->debug(prefix(msg))

#endif
