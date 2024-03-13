#include "CCLogSystem.h"

#include <time.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

const int MAX_HANDLER = 5;

ConsoleLogHandler::ConsoleLogHandler(unsigned int levels) : CCLogHandler(levels)
{
}

ConsoleLogHandler::~ConsoleLogHandler()
{
}

bool ConsoleLogHandler::Handle(const char *pfilename, const char *pfunction, int line, const char *pstr)
{
#ifdef _WIN32
	char buffer[2048];

	time_t nowtime;
	nowtime = time(NULL);

	struct tm * timeinfo;
	timeinfo = localtime(&nowtime);

	char time_buffer[80];
	strftime(time_buffer, 80, "%Y-%m-%d %H:%M:%S", timeinfo);

	if (pfilename != NULL && pfunction != NULL)
	{
		snprintf(buffer, sizeof(buffer), "[%s][%s(%d)]%s(): %s\n", time_buffer, pfilename, line, pfunction, pstr);
	}
	else if (pfilename != NULL)
	{
		snprintf(buffer, sizeof(buffer), "[%s][%s(%d)]: %s\n", time_buffer, pfilename, line, pstr);
	}
	else if (pfunction != NULL)
	{
		snprintf(buffer, sizeof(buffer), "[%s]%s(): %s\n", time_buffer, pfunction, pstr);
	}
	else
	{
		snprintf(buffer, sizeof(buffer), "[%s] %s\n", time_buffer, pstr);
	}

	printf("%s", buffer);
#endif

	return true;
}

FileLogHandler::FileLogHandler(const char *logfile, unsigned int levels) : CCLogHandler(levels)
{
#ifdef _WIN32
	if (logfile != NULL)
		m_fp = fopen(logfile, "wt");
#endif
}

FileLogHandler::~FileLogHandler()
{
#ifdef _WIN32
	if (m_fp) fclose(m_fp);
#endif
}

bool FileLogHandler::Handle(const char *pfilename, const char *pfunction, int line, const char *pstr)
{
#ifdef _WIN32
	time_t nowtime;
	nowtime = time(NULL);

	struct tm * timeinfo;
	timeinfo = localtime(&nowtime);

	char time_buffer[80];
	strftime(time_buffer, 80, "%Y-%m-%d %H:%M:%S", timeinfo);

	if (pfilename != NULL && pfunction != NULL)
	{
		fprintf(m_fp, "[%s][%s(%d)]%s(): %s\n", time_buffer, pfilename, line, pfunction, pstr);
	}
	else if (pfilename != NULL)
	{
		fprintf(m_fp, "[%s][%s(%d)]: %s\n", time_buffer, pfilename, line, pstr);
	}
	else if (pfunction != NULL)
	{
		fprintf(m_fp, "[%s]%s(): %s\n", time_buffer, pfunction, pstr);
	}
	else
	{
		fprintf(m_fp, "[%s] %s\n", time_buffer, pstr);
	}
	fflush(m_fp);
#endif

	return true;
}

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
		
#ifdef _WIN32
		if (filename != nullptr) 
		{
			const char* newfname = strrchr(filename, '\\');// Windows_反斜杠 
			if (newfname != NULL) s_pFileName = newfname + 1;//去掉斜杠
		}
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