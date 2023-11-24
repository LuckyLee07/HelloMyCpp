#include "CCLogHandler.h"
#include <time.h>
#include <stdio.h>
#include <string>

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