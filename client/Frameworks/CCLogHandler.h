#ifndef __CCLOG_HANDLER_H__
#define __CCLOG_HANDLER_H__

#include <stdio.h>

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


#endif //__CCLOG_HANDLER_H__
