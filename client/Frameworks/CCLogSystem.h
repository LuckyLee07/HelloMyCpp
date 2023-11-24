#ifndef __CCLOG_SYSTEM_H__
#define __CCLOG_SYSTEM_H__

class CCLogHandler;

namespace Fancy
{
	enum
	{
		LOGL_DEBUG = 1,
		LOGL_INFO = 2,
		LOGL_WARNING = 4,
		LOGL_SERVER = 8,
		LOGL_ALLLEVEL = 128,
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
#define	CCLOG_INFO		Fancy::CCLogSetParam(__FILENAME__, __FUNCTION__, __LINE__, Fancy::LOGL_INFO),	Fancy::CCLogMessage
#define	CCLOG_DEBUG		Fancy::CCLogSetParam(__FILENAME__, __FUNCTION__, __LINE__, Fancy::LOGL_DEBUG),	Fancy::CCLogMessage


#endif //__CCLOG_SYSTEM_H__
