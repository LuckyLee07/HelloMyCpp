#ifndef __LOG_SYSTEM_H__
#define __LOG_SYSTEM_H__

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

	int LogSysInit();
	void LogRelease();
	void LogSetParam(const char *filename, const char *funcname, int line, unsigned int level);
	void LogMessage(const char *format, ...);

	int LogAddFileHandler(const char *logfile, unsigned int level);
	int LogAddConsoleHandler(unsigned int level);
}



#endif //__LOG_SYSTEM_H__
