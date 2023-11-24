#include "CCLogSystem.h"
#include "CCLogHandler.h"

#include <stddef.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

const int MAX_HANDLER = 5;

namespace Fancy
{
	static const char *s_pFileName;
	static const char *s_pFuncName;
	static int s_iLine;
	static unsigned int s_LogLevel;
	static bool s_isInited = false;

	static int s_nHander = 0;
	static CCLogHandler *s_pHandlers[MAX_HANDLER];

	int CCLogSysInit()
	{
		if (s_isInited) 
			return -1;
		
		s_pFileName = NULL;
		s_pFuncName = NULL;
		s_iLine = 0;
		s_LogLevel = 0;
		s_isInited = true;

		return 0;
	}

	void CCLogSysRelease()
	{
		s_pFileName = NULL;
		s_pFuncName = NULL;
		s_iLine = 0;
		s_LogLevel = 0;
		s_isInited = false;
	}

	void CCLogSetParam(const char *filename, const char *funcname, int line, unsigned int level)
	{
		s_iLine = line;
		s_LogLevel = level;
		s_pFuncName = funcname;
		s_pFileName = filename;
		
		const char* fullpath = filename;
#ifdef _WIN32
		const char* newfname = strrchr(fullpath, '\\');// Windows_反斜杠 
		if (newfname != NULL) s_pFileName = newfname + 1;//去掉斜杠
#endif
		/*
		static const char* brefix = "client";
		static int bresize = strlen(brefix)+1;//去掉斜杠

		const char* newfname = strstr(fullpath, brefix);
		if (newfname != NULL) s_pFileName = newfname + bresize;
		*/
	}

	void CCLogMessage(const char *format, ...)
	{
		if (format == NULL) format = "";

		va_list		argptr;
		char buffer[1024 * 256];

		va_start(argptr, format);
		vsnprintf(buffer, sizeof(buffer), format, argptr);
		va_end(argptr);

		for (int i = 0; i < s_nHander; i++)
		{
			CCLogHandler *pHandler = s_pHandlers[i];
			if (s_LogLevel & pHandler->getLevels())
			{
				pHandler->Handle(s_pFileName, s_pFuncName, s_iLine, buffer);
			}
		}
	}

	int LogAddHandler(CCLogHandler *pHandler)
	{
		if (pHandler == NULL) return -1;
		if (s_nHander == MAX_HANDLER) return -1;

		s_pHandlers[s_nHander++] = pHandler;

		return s_nHander - 1;
	}

	int LogAddConsoleHandler(unsigned int levels)
	{
		ConsoleLogHandler *pHandler = new ConsoleLogHandler(levels);
		return LogAddHandler(pHandler);
	}

	int LogAddFileHandler(const char *logfile, unsigned int levels)
	{
		FileLogHandler *pHandler = new FileLogHandler(logfile, levels);
		return LogAddHandler(pHandler);
	}
}