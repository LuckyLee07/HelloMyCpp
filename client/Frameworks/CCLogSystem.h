#ifndef __CCLOG_SYSTEM_H__
#define __CCLOG_SYSTEM_H__

#include <stdio.h>

//--CCLogHandler--
class CCLogHandler
{
public:
	CCLogHandler(unsigned int levels) : m_LevelFlags(levels) {}
	virtual ~CCLogHandler() {}

	virtual bool Handle(const char *pfilename, const char *pfunction, int line, const char *pstr) = 0;

	unsigned int getLevels() { return m_LevelFlags; }

protected:
	unsigned int m_LevelFlags;
};

class ConsoleLogHandler : public CCLogHandler
{
public:
	ConsoleLogHandler(unsigned int levels);
	virtual ~ConsoleLogHandler();

	bool Handle(const char *pfilename, const char *pfunction, int line, const char *pstr) override;
};

class FileLogHandler : public CCLogHandler
{
public:
	FileLogHandler(const char *logfile, unsigned int levels);
	virtual ~FileLogHandler();

	bool Handle(const char *pfilename, const char *pfunction, int line, const char *pstr) override;

#ifdef _WIN32
private:
	FILE *m_fp;
#endif
};

namespace Fancy
{
	enum
	{
		LOGL_INFO = 1,
		LOGL_DEBUG = 2,
		LOGL_ERROR = 4,

		LOGL_ALLLEVEL = 64,
	};

	int CCLogSysInit();
	void CCLogSysRelease();
	void CCLogSetParam(const char *filename, const char *funcname, int line, unsigned int level);
	void CCLogMessage(const char *format, ...);

	int LogAddHandler(CCLogHandler *pHandler);
	int LogAddConsoleHandler(unsigned int levels);
	int LogAddFileHandler(const char *logfile, unsigned int levels);
}

#define __FILENAME__ __FILE__
// 相关的几个日志打印接口
#define	CCLUA_INFO		Fancy::CCLogSetParam(NULL, NULL, __LINE__, Fancy::LOGL_INFO),	Fancy::CCLogMessage
#define CCLUA_ERROR		Fancy::CCLogSetParam(NULL, NULL, __LINE__, Fancy::LOGL_ERROR),	Fancy::CCLogMessage

#define	CCLOG_INFO		Fancy::CCLogSetParam(__FILENAME__, __FUNCTION__, __LINE__, Fancy::LOGL_INFO),	Fancy::CCLogMessage
#define	CCLOG_DEBUG		Fancy::CCLogSetParam(__FILENAME__, __FUNCTION__, __LINE__, Fancy::LOGL_DEBUG),	Fancy::CCLogMessage
#define CCLOG_ERROR		Fancy::CCLogSetParam(__FILENAME__, __FUNCTION__, __LINE__, Fancy::LOGL_ERROR),	Fancy::CCLogMessage

#endif //__CCLOG_SYSTEM_H__
